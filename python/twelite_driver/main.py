'''
 "twelite" module test utility.

 Example usage (to send "t" to TWELITE destination node 1):
 $ python main.py -d 1 -p "t" COM5


 Loopback test setup on a single PC with two USB ports:

 $ python main.py -d 1 -p "Hello world!" COM5 --> [MONOSTICK on COM5] <- - - - - - - - - - - -+
                                                                                              |
 $ python main.py -d 0 -p "Guten Tag!" COM6 --> [TWELITE DIP on TWELITE Writer on COM6] < - - +

'''

import twelite as tw
import numpy as np
import time
import datetime
import argparse
import traceback
import sys

### Serial port setting in bps
BAUDRATE = 115200

### Argument parser
DESCRIPTION = '''
"twelite" module test utility.
'''
parser = argparse.ArgumentParser(description=DESCRIPTION)
parser.add_argument("port", help="Serial port identifier")
parser.add_argument("-d", "--dst", help="Destination node identifier", type=int)
parser.add_argument("-q", "--quality", help="Print out quality data (sequence number and LQI) as well", action='store_true')
parser.add_argument("-l", "--loop", help="The number of looping", default="10")
parser.add_argument("-D", "--delay", help="Delay in a loop (in msec)", default="0")
parser.add_argument("-p", "--payload", help="Payload", default=None)
args = parser.parse_args()

info = lambda seq, lqi: ' seq number: {}, LQI: {} ({} dBm)'.format(seq, lqi, tw.lqi2dbm(lqi))
def read_and_print_data(label, dst, master_node, cmd, quality_data=False):
    '''
    Read data from 8bit MCU and print the data.
    Note 1: 8bit MCU, as a co-processor of a sensor (slave node), is for
    reading data from the sensor and transmitting it to the master node.
    Note 2: the sequence number is generated in MasterNode class

    '''
    print('--- SLAVE {} ---'.format(dst))
    data = master_node.read(dst=dst, cmd=cmd, quality_data=quality_data)
    if quality_data:
        data, src, seq, lqi = data
        print(info(seq, lqi))
    else:
        print(data)
    print('<{}> '.format(label), end='')


if __name__ == '__main__':

    ### Passive mode
    with tw.MasterNode(args.port, BAUDRATE) as mn:
        while True:
            try:
                if (args.payload):
                    mn.write(args.dst, [ord(c) for c in args.payload])
                data, src, seq, lqi = mn.read(quality_data=True)
                print("data: {}, src: {}, seq: {}, lqi: {}".format(data, src, seq, lqi))
            except tw.TweliteException as e:
                print(e)
                pass
