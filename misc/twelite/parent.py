# Test code for parent node

import serial
import time

PORT = "COM9"
BAUDRATE = 115200

THERMISTOR = ord('t')  #0x74
PIXELS = ord('p')  #0x70

CHILD_1 = 0x01

THERMISTOR_RESOLUTION = 0.0625
PIXELS_RESOLUTION = 0.25

b2i = lambda data, idx: int.from_bytes([data[idx]], byteorder='big', signed=False)

# Sequence number generator
class GenSeq:

    def __init__(self):
        self.seq = -1

    def __next__(self):
        self.seq += 1
        if self.seq > 255:
            self.seq = 0
        return self.seq

# TWELITE packet parser
class Parser:

    def __init__(self, port, baudrate):
        self.ser = serial.Serial(port, baudrate,
                                 timeout=3, inter_byte_timeout=3)
        self.genSeq = GenSeq()
        self.seq = 0
        self.cmd = None
        
    def tx(self, dst, cmd):
        len_ = 6  # One byte payload (5 + 1)
        self.cmd = cmd
        self.seq = next(self.genSeq)
        data = [dst, 0xA0, self.seq, 0x01, 0xFF, cmd]
        ck = data[0]
        for c in data[1:]:
            ck = ck ^ c
        print(ck)
        cmd_twelite = bytes([0xA5, 0x5A, 0x80, len_, *data, ck])
        print([0xA5, 0x5A, 0x80, len_, *data, ck])
        self.ser.write(cmd_twelite)

    def rx(self):
        d = self.ser.read(4)  # 0xA5 0x5A 0x80 <len>
        len_ =  b2i(d, 3)
        d = self.ser.read(5)  # <dst> 0xA0 seq 0x01 0xFF
        dst = b2i(d, 0)
        seq = b2i(d, 2)
        len_ = len_ - 5
        data = []
        d = self.ser.read(len_)  # <data[]>
        if self.cmd == THERMISTOR:
            temp = b2i(d,1) * 256 + b2i(d, 0)  # MSB LSB
            temp = temp * THERMISTOR_RESOLUTION
            data.append(temp)
        elif self.cmd == PIXELS:           
            for i in range(len_):
                data.append[b2i(d,i)*PIXELS_RESOLUTION]
        else:
            for i in range(len_):
                data.append[b2i(d,i)]            
        d = self.ser.read(2)  # Checksum, EOT
        ck = b2i(d,0)
        print('dst: {}'.format(str(dst)))
        print('len: {}'.format(str(len_)))
        print('cmd, {}, seq(tx): {}, seq(rx): {}'.format(str(self.cmd),
                                                         str(self.seq),
                                                         str(seq)))
        print('data:')
        for i in range(len_):
            print('{} '.format(str(data[i])))
        print('ck: {}'.format(str(ck)))

if __name__ == '__main__':

    parser = Parser(PORT, BAUDRATE)
    
    while True:
        '''
        parser.tx(dst=CHILD_1, cmd=THERMISTOR)
        parser.rx()
        time.sleep(0.5)
        '''
        parser.tx(dst=CHILD_1, cmd=PIXELS)
        parser.rx()
        time.sleep(3)
