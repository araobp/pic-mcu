'''
  <<< Using TWELITE with infrared array sensor "AMG8833" and 8bit MCU >>>

Architecture:
- Infrared array sensor "AMG8833" and 8bit MCU are attached to each slave node.
- And an application on the 8bit MCU works as i2c-to-uart gateway. 

 [AMG8833]-i2c-[8bit MCU]-uart-[TWELITE slave] - - - [TWELITE master] -(*)- PC

This python module provides the interface (*) described in the above.

Reference: https://mono-wireless.com/jp/products/TWE-APPS/App_Uart/mode_format.html

'''

import serial
import numpy as np

### TWELITE packet structure
BYTE = 0xA0
EOT = 0x04
# Options
RESPONSE_MSG_DISABLED = 0x07
ACK_ENABLED = 0x01
RESEND = 0x02
TERMINATOR = 0xFF
# The number of resend in case of communication error
NUM_RETRY = 3

### One-byte command to fetch data from a remote node
# Note: these commands have nothing to do with TWELITE itself.
THERMISTOR = ord('t')
PIXELS = ord('p')
DIFF = ord('d')
MOTION_DETECTION = ord('m')
MOTION_COUNT = ord('M')
ENABLE_NOTIFY_MOTION = ord('n')
ENABLE_NOTIFY_OBJECT = ord('o')
DISABLE_NOTIFY = ord('N')
DUMP_SETTINGS = ord('s')

### AMG8833 data resolution
THERMISTOR_RESOLUTION = 0.0625
PIXELS_RESOLUTION = 0.25

## Serial read timeout
TIMEOUT = 0.3  # 300msec

### Byte to integer conversion (2's complement)
b2ui = lambda data, idx: int.from_bytes([data[idx]], byteorder='big', signed=False)
b2i = lambda data, idx: int.from_bytes([data[idx]], byteorder='big', signed=True)

# [AMG8833-specific] Data conversion into degrees Celsius unit
def _conv_data_amg8833(cmd, d):
    len_ = len(d)
    if cmd == THERMISTOR:
        data = (b2ui(d,1) * 256 + b2ui(d, 0)) * THERMISTOR_RESOLUTION
    elif cmd == PIXELS:
        data = np.zeros(len_)
        for i in range(len_):
            data[i] = b2ui(d,i)*PIXELS_RESOLUTION
    elif cmd == DIFF:
        data = np.zeros(len_)
        for i in range(len_):
            data[i] = b2i(d,i)*PIXELS_RESOLUTION
    elif cmd == MOTION_DETECTION or cmd == MOTION_COUNT or cmd is None:
        data = np.zeros(len_, dtype=int)
        for i in range(len_):
            data[i] = b2i(d,i)
    elif cmd == DUMP_SETTINGS:
        data = np.zeros(len_, dtype=int)
        for i in range(len_):
            data[i] = b2ui(d,i)
    return data

# LQI to dBm conversion
def lqi2dbm(lqi):
    '''
    Convert LQI into dBm
    '''
    return (7.0 * lqi - 1970) / 20.0

class GenSeq:
    '''
    Sequence number generator (0 - 255 range).
    '''

    def __init__(self):
        self.seq = 0

    def __next__(self):
        self.seq += 1
        if self.seq > 0xff:
            self.seq = 0
        return self.seq

class TweliteException(Exception):

    def __init__(self, message):
        super().__init__(message)

class MasterNode:
    '''
    This is a packet parser for transmitting/receiving data over TWELITE.
    '''

    def __init__(self, port, baudrate, retry=NUM_RETRY, timeout=TIMEOUT):
        self.port  = port
        self.baudrate = baudrate
        self.genSeq = GenSeq()
        self.seq = 0
        self.cmd = None
        self.retry = retry 
        self.timeout = timeout
        self.ser = serial.Serial(self.port, self.baudrate, timeout=timeout)
        
    def __enter__(self):
        return self

    def __exit__(self, *args):
        self.ser.close()

    def close(self):
        self.ser.close()
        
    # Transmit data        
    def _tx(self, dst, cmd):
        self.cmd = cmd
        seq = next(self.genSeq)
        data = [dst, BYTE, seq, RESPONSE_MSG_DISABLED,
                ACK_ENABLED, RESEND, self.retry, TERMINATOR, cmd]
        ck = data[0]
        for c in data[1:]:
            ck = ck ^ c  # XOR for calculating checksum
        len_ = len(data)
        cmd_twelite = bytes([0xA5, 0x5A, 0x80, len_, *data, ck])
        #print([0xA5, 0x5A, 0x80, len_, *data, ck])
        self.ser.write(cmd_twelite)
        self.ser.flush()

    # Receive data
    def _rx(self):
        d = self.ser.read(5)  # 0xA5 0x5A 0x80 <len> <src>
        if len(d) == 0:
            data, seq, lqi = d, 0, 0  # d is b'' in this case
            raise TweliteException('read timeout: {:.1f} sec passed'.format(self.timeout))
        else:
            #print(d)
            len_ =  b2ui(d, 3)
            src = b2ui(d, 4)
            d = self.ser.read(13)  # 0xA0 seq <4bytes> <4bytes> <LQI> 0x00 <len>
            #print(d)
            seq = b2ui(d, 1)
            lqi = b2ui(d, 10)
            len_ = b2ui(d, 12)
            #print('src: {}, len: {}, lqi: {}'.format(str(src), str(len_), str(lqi)))

            d = self.ser.read(len_)  # <data[]>
            #print(d)
            data = _conv_data_amg8833(self.cmd, d)
                    
            d = self.ser.read(2)  # Checksum, EOT
            #ck = b2i(d,0)
            #print(data)
        
        return (data, src, seq, lqi)
        
    # Write data
    def write(self, dst, cmd):
        self._tx(dst, cmd)

    # Read data: tx then rx
    def read(self, dst=None, cmd=None, quality_data=False):
        if dst and cmd:
            self._tx(dst, cmd)
        resp = self._rx()
        if quality_data:
            return resp
        else:
            return resp[0]
