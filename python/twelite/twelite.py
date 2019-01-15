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

### TWELITE packet structure
BYTE = 0xA0
EOT = 0x04
# Options
RESPONSE_MSG_DISABLED = 0x07
ACK_ENABLED = 0x01
RESEND = 0x02
TERMINATOR = 0xFF
# The number of resend in case of communication error
NUM_RETRY = 10

### One-byte command to fetch data from a remote node
# Note: these commands have nothing to do with TWELITE itself.
THERMISTOR = ord('t')  #0x74: fetch room temperature data
PIXELS = ord('p')  #0x70: fetch pixels data as heatmap

### AMG8833 data resolution
THERMISTOR_RESOLUTION = 0.0625
PIXELS_RESOLUTION = 0.25

### Byte to integer conversion (2's complement)
b2i = lambda data, idx: int.from_bytes([data[idx]], byteorder='big', signed=False)

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

class MasterNode:
    '''
    This is a packet parser for transmitting/receiving data over TWELITE.
    '''

    def __init__(self, port, baudrate):
        self.port  = port
        self.baudrate = baudrate
        self.genSeq = GenSeq()
        self.seq = 0
        self.cmd = None
        
    def __enter__(self):
        self.ser = serial.Serial(self.port, self.baudrate, timeout=0.3)
        return self

    def __exit__(self, *args):
        self.ser.close()
        
    # Transmit data        
    def tx(self, dst, cmd, seq=None):
        self.cmd = cmd
        if seq is None:
            seq = next(self.genSeq)
        data = [dst, BYTE, seq, RESPONSE_MSG_DISABLED,
                ACK_ENABLED, RESEND, NUM_RETRY, TERMINATOR, cmd]
        ck = data[0]
        for c in data[1:]:
            ck = ck ^ c  # XOR for calculating checksum
        len_ = len(data)
        cmd_twelite = bytes([0xA5, 0x5A, 0x80, len_, *data, ck])
        #print([0xA5, 0x5A, 0x80, len_, *data, ck])
        self.ser.write(cmd_twelite)

    # Receive data
    def rx(self):
        d = self.ser.read(5)  # 0xA5 0x5A 0x80 <len> <dst>
        #print(d)
        len_ =  b2i(d, 3)
        dst = b2i(d, 4)
        d = self.ser.read(13)  # 0xA0 seq <4bytes> <4bytes> <LQI> 0x00 <len>
        #print(d)
        seq = b2i(d, 1)
        lqi = b2i(d, 10)
        len_ = b2i(d, 12)
        #print('dst: {}, len: {}, lqi: {}'.format(str(dst), str(len_), str(lqi)))

        data = []
        d = self.ser.read(len_)  # <data[]>
        #print(d)
        if self.cmd == THERMISTOR:
            temp = b2i(d,1) * 256 + b2i(d, 0)  # MSB LSB
            temp = temp * THERMISTOR_RESOLUTION
            data.append(temp)
        elif self.cmd == PIXELS:
            for i in range(len_):
                temp = b2i(d,i)*PIXELS_RESOLUTION
                data.append(temp)
            
        d = self.ser.read(2)  # Checksum, EOT
        #ck = b2i(d,0)
        return (data, len_, seq, lqi)

    # Fetch data: tx then rx
    def fetch(self, dst, cmd, seq=None):
        self.tx(dst, cmd, seq)
        return self.rx()        


