#
# Master node
#

import serial
import time

# Serial port
PORT = "COM9"
BAUDRATE = 115200

BYTE = 0xA0
RESPONSE_MSG_DISABLED = 0x07
TERMINATOR = 0xFF
EOT = 0x04

# Destination addresses of TWELITE-DIP slave nodes
SLAVE_1 = 0x01
SLAVE_2 = 0x02
SLAVE_3 = 0x03

# One-byte command to control slaves
THERMISTOR = ord('t')  #0x74: fetch room temperature data
PIXELS = ord('p')  #0x70: fetch pixels data as heatmap

# Data resolution
THERMISTOR_RESOLUTION = 0.0625
PIXELS_RESOLUTION = 0.25

# Byte to integer conversion (2's complement)
b2i = lambda data, idx: int.from_bytes([data[idx]], byteorder='big', signed=False)

TX_RETURN_CODE = ['OK', 'NG']
    
# Sequence number generator
class GenSeq:

    def __init__(self):
        self.seq = -1

    def __next__(self):
        self.seq += 1
        if self.seq > 0xff:
            self.seq = 0
        return self.seq

# TWELITE packet parser
class Parser:
    '''
    This is a packet parser for transmitting/receiving data over TWELITE.
    '''

    def __init__(self, port, baudrate):
        self.port  = port
        self.baudrate = baudrate
#        self.ser = serial.Serial(port, baudrate,
#                                timeout=3, inter_byte_timeout=3)
        self.genSeq = GenSeq()
        self.seq = 0
        self.cmd = None

    # Transmit data        
    def tx(self, dst, cmd):
        self.ser = serial.Serial(self.port, self.baudrate,
                                 timeout=3, inter_byte_timeout=3)
        self.cmd = cmd
        self.seq = next(self.genSeq)
        data = [dst, BYTE, 0x00, RESPONSE_MSG_DISABLED, TERMINATOR, cmd]
        #data = [dst, BYTE, 0x00, TERMINATOR, cmd]
        ck = data[0]
        for c in data[1:]:
            ck = ck ^ c
        len_ = len(data)
        cmd_twelite = bytes([0xA5, 0x5A, 0x80, len_, *data, ck])
        print([0xA5, 0x5A, 0x80, len_, *data, ck])
        self.ser.write(cmd_twelite)
        self.ser.flush()

    # Receive data
    def rx(self):
        print('=== RX ================')
        d = self.ser.read(5)  # 0xA5 0x5A 0x80 <len> <dst>
        print(d)
        len_ =  b2i(d, 3)
        dst = b2i(d, 4)
        d = self.ser.read(13)  # 0xA0 seq <4bytes> <4bytes> <LQI> 0x00 <len>
        print(d)
        seq = b2i(d, 1)
        lqi = b2i(d, 10)
        len_ = b2i(d, 12)
        print('header read...')
        print('dst: {}'.format(str(dst)))
        print('lqi: {}'.format(str(lqi)))
        print('len: {}'.format(str(len_)))

        data = []
        d = self.ser.read(len_)  # <data[]>
        if self.cmd == THERMISTOR:
            temp = b2i(d,1) * 256 + b2i(d, 0)  # MSB LSB
            temp = temp * THERMISTOR_RESOLUTION
            data.append(temp)
            print('data: {}'.format(data[0]))
        elif self.cmd == PIXELS:
            print('--- data ---')
            for i in range(len_):
                temp = b2i(d,i)*PIXELS_RESOLUTION
                data.append(temp)
                print(temp)
                
        d = self.ser.read(2)  # Checksum, EOT
        print(d)
        ck = b2i(d,0)
        print('cmd, {}, seq(tx): {}, seq(rx): {}'.format(str(self.cmd),
                                                         str(self.seq),
                                                         str(seq)))
        print('ck: {}'.format(str(ck)))
        self.ser.close()

if __name__ == '__main__':

    parser = Parser(PORT, BAUDRATE)

    while True:
        
        parser.tx(dst=SLAVE_1, cmd=PIXELS)
        parser.rx()
            
        parser.tx(dst=SLAVE_1, cmd=THERMISTOR)
        parser.rx()

        time.sleep(0.5)
