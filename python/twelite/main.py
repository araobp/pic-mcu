'''
 "twelite" module test utility.

 Example usage:
 $ python main.py COM9 2 1 -q


Motion detection calibration:

    Peak count threshold
    0: 0.5
    1: 1.0
    2: 1.5
    3: 2.0
    4: 2.5
    5: 3.0

    Object resolution
    6: 1
    7: 2
    8: 3
    9: 4

'''
import twelite_amg8833 as tw
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
"twelite_amg8833" module test utility.
'''
parser = argparse.ArgumentParser(description=DESCRIPTION)
parser.add_argument("port", help="Serial port identifier")
parser.add_argument("-l", "--loop", help="The number of looping", default="10")
parser.add_argument("-d", "--dst", help="Destination node identifier")
parser.add_argument("-p", "--performance_measurement", help="Performance measurement on 64 pixels data transmission over TWELITE", action='store_true')
parser.add_argument("-q", "--quality", help="Print out quality data (sequence number and LQI) as well", action='store_true')
parser.add_argument("-m", "--motion_detection", help="Column-wise motion detection", action='store_true')
parser.add_argument("-M", "--motion_count", help="Motion count on a specific row", action='store_true')
parser.add_argument("-D", "--delay", help="Delay in a loop (in msec)", default="0")
parser.add_argument("-c", "--calibrate", help="Calibrate motion detection parameters")
parser.add_argument("-n", "--enable_notify", help="Enable notifications of motion count in passive mode", action='store_true')
parser.add_argument("-N", "--disable_notify", help="Disable notifications of motion count", action='store_true')
parser.add_argument("-P", "--calibration_parameter_description", help="Print calibration paramter description", action='store_true')
parser.add_argument("-C", "--motion_count_notifications", help="Motion count notifications", action='store_true')
args = parser.parse_args()

CALIBRATION_PARAM_DESC = '''
Description for "-c" or "--calibrate option" command option:

    Peak count threshold (diff larger than this value is ragarded as edge of a moving object)
    0: 0.5
    1: 1.0
    2: 1.5
    3: 2.0
    4: 2.5
    5: 3.0

    Object resolution (a peak with this width row-wise is ragarded as a moving object) 
    6: 1
    7: 2
    8: 3
    9: 4
'''

info = lambda seq, lqi: ' seq number: {}, LQI: {} ({} dBm)'.format(seq, lqi, tw.lqi2dbm(lqi))
def read_and_print_data(label, dst, master_node, cmd, quality_data=False):
    '''
    Read data from 8bit MCU and print the data.
    Note 1: 8bit MCU, as a co-processor of TWELITE (slave node), is for
    reading data from AMG8833 and transmitting it to the master node.
    Note 2: the sequence number is generated in MasterNode class

    '''
    print('--- SLAVE {} ---'.format(dst))
    data = master_node.read(dst=dst, cmd=cmd, quality_data=quality_data)
    if quality_data:
        data, src, seq, lqi = data
        print(info(seq, lqi))
    print('<{}> '.format(label), end='')
    if cmd == tw.THERMISTOR:
        print('{} degrees Celsius'.format(data))
    elif cmd == tw.PIXELS or cmd == tw.DIFF:
        print('')
        data = data.reshape((8,8))
        for row in data:
            for d in row:           
                print('{:4.1f} '.format(d), end='')
            print('')
    elif cmd == tw.MOTION_DETECTION:
        print('')
        data = data.reshape((8,8))
        for row in data:
            for d in row:           
                print('{:4d} '.format(d), end='')
            print('')
    elif cmd == tw.MOTION_COUNT:
        print('')
        for d in data:
            print('{:4d} '.format(d), end='')
        print('')

def read_and_print_motion_count(master_node, dst):
    data = master_node.read(dst=dst, cmd=tw.MOTION_COUNT)
    if np.count_nonzero(data) > 0:
        current = datetime.datetime.now()
        print ('[{}] src:{:2d} |'.format(current.strftime("%H:%M:%S"), dst), end='')
        for d in data:
            print('{:2d} '.format(d), end='')
        print('|')

def print_motion_count_notifications(data):
    current = datetime.datetime.now()
    print ('[{}] src:{:2d} |'.format(current.strftime("%H:%M:%S"), src), end='')
    for d in data:
        if d == 0:
            print('  ', end='')
        elif d == 1:
            print(' F', end='')
        elif d == -1:
            print(' B', end='')
        #print('{:2d} '.format(d), end='')
    print('|')

if __name__ == '__main__':

    ### Description
    if args.calibration_parameter_description:
        print(CALIBRATION_PARAM_DESC)
        sys.exit(0)

    ### Passive mode
    if args.motion_count_notifications:

        with tw.MasterNode(args.port, BAUDRATE, timeout=3.0) as mn:
            while True:
                try:
                    data, src, seq, lqi = mn.read(quality_data=True)
                    print_motion_count_notifications(data)
                except tw.TweliteException as e:
                    #print(e)
                    pass

    ### Reactive mode
    else:

        dst = int(args.dst)

        with tw.MasterNode(args.port, BAUDRATE) as mn:

            if args.calibrate:
                mn.write(dst=dst, cmd=ord(args.calibrate))
                sys.exit(0)

            if args.disable_notify:
                mn.write(dst=dst, cmd=tw.DISABLE_NOTIFY)
                sys.exit(0)

            if args.enable_notify:
                mn.write(dst=dst, cmd=tw.ENABLE_NOTIFY)
                sys.exit(0)            

            start_time = time.time()
            err_cnt = 0
            delay = float(args.delay)/1000.0  # msec -> sec
            
            for _ in range(int(args.loop)):

                time.sleep(delay)                    

                try:
                    if args.performance_measurement:
                        ### Read 64 pixels data only
                        data = mn.read(dst=dst, cmd=tw.PIXELS, quality_data=False)
                    elif args.motion_detection:
                        ### Read 64 pixels motion detection only                        
                        read_and_print_data('motion detection', dst, mn, tw.MOTION_DETECTION, quality_data=args.quality)
                    elif args.motion_count:
                        ### Read motion count only
                        read_and_print_motion_count(mn, dst)
                    else:
                        ### Read room temperature data
                        read_and_print_data('room temperature', dst, mn, tw.THERMISTOR, quality_data=args.quality)

                        ### Read 64 pixels data
                        read_and_print_data('64 pixels', dst, mn, tw.PIXELS, quality_data=args.quality)

                        ### Read 64 pixels diff
                        read_and_print_data('64 pixels diff', dst, mn, tw.DIFF, quality_data=args.quality)
                        
                        ### Read 64 pixels motion detection
                        read_and_print_data('motion detection', dst, mn, tw.MOTION_DETECTION, quality_data=args.quality)

                        ### Read motion count
                        read_and_print_data('motion count', dst, mn, tw.MOTION_COUNT, quality_data=args.quality)

                except Exception as e:
                    err_cnt += 1
                    print('[{}] {}'.format(str(err_cnt), str(e)))
                    #traceback.print_exc()
                
            stop_time = time.time()
            print('--- STATS ---')       
            print('Elapsed time: {:.3f} sec'.format(stop_time - start_time))
            print('Average interval: {:.1f} msec'.format((stop_time - start_time)/int(args.loop)*1000))
            print('Tranmission error: {} times'.format(err_cnt))
        
