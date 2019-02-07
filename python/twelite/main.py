'''
 "twelite" module test utility.

 Example usage:
 $ python main.py COM9 2 1 -q

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
parser.add_argument("-d", "--dst", help="Destination node identifier")
parser.add_argument("-l", "--loop", help="The number of looping", default="10")
parser.add_argument("-q", "--quality", help="Print out quality data (sequence number and LQI) as well", action='store_true')
parser.add_argument("-p", "--performance_measurement", help="Performance measurement on 64 pixels data transmission over TWELITE", action='store_true')
parser.add_argument("-m", "--motion_detection", help="Column-wise motion detection", action='store_true')
parser.add_argument("-M", "--motion_count", help="Motion count on a specific row", action='store_true')
parser.add_argument("-D", "--delay", help="Delay in a loop (in msec)", default="0")
parser.add_argument("-n", "--enable_notify_motion", help="Enable notifications of motion count in passive mode", action='store_true')
parser.add_argument("-o", "--enable_notify_object", help="Enable notifications of object diff in passive mode", action='store_true')
parser.add_argument("-N", "--disable_notify", help="Disable notifications of motion count", action='store_true')
parser.add_argument("-c", "--motion_count_notifications", help="Motion count notifications", action='store_true')
parser.add_argument("-t", "--threshold", help="Calibrate motion detection threshold")
parser.add_argument("-T", "--threshold_description", help='Print "threashold" option description', action='store_true')
parser.add_argument("-s", "--dump_settings", help="Dump setting parameters", action='store_true')
args = parser.parse_args()

THRESHOLD_DESCRIPTION = '''
Description for "-t" or "--threshold" command option:

[Peak count threshold]
Diff larger than this value is ragarded as a moving object.

    0: PEAK_COUNT_THRESHOLD
    1: 0.25
    2: 0.5
    3: 0.75
    4: 1.0
    5: 1.25
    6: 1.5
    7: 1.75
    8: 2.0
    9: 2.25

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
        if len(data) == 64:
            data = data.reshape((8,8))
        elif len(data) == 128:
            data = data.reshape((16,8))
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
    len_ = len(data)
    if (len(data) == 8):
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
    elif (len(data) == 64):
        data = data.reshape((8,8))
        print ('[{}] src:{:2d}'.format(current.strftime("%H:%M:%S"), src))
        for row in data:
            for d in row:           
                print('{:4d} '.format(d), end='')
            print('')
        

if __name__ == '__main__':

    ### Description
    if args.threshold_description:
        print(THRESHOLD_DESCRIPTION)
        sys.exit(0)

    ### Passive mode
    if args.motion_count_notifications:

        with tw.MasterNode(args.port, BAUDRATE) as mn:
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
        delay = float(args.delay)/1000.0  # msec -> sec

        with tw.MasterNode(args.port, BAUDRATE, retry=3, timeout=1) as mn:

            if args.dump_settings:
                data = mn.read(dst=dst, cmd=tw.DUMP_SETTINGS)
                print(data)
                sys.exit(0)

            if args.threshold:
                mn.write(dst=dst, cmd=ord(args.threshold))
                sys.exit(0)

            if args.disable_notify:
                mn.write(dst=dst, cmd=tw.DISABLE_NOTIFY)
                sys.exit(0)

            if args.enable_notify_motion:
                mn.write(dst=dst, cmd=tw.ENABLE_NOTIFY_MOTION)
                sys.exit(0)

            if args.enable_notify_object:
                mn.write(dst=dst, cmd=tw.ENABLE_NOTIFY_OBJECT)
                sys.exit(0)

            start_time = time.time()
            err_cnt = 0
            cnt = 0
            
            for _ in range(int(args.loop)):

                time.sleep(delay)                    

                try:
                    if args.performance_measurement:
                        ### Read 64 pixels data only
                        data = mn.read(dst=dst, cmd=tw.PIXELS, quality_data=False)
                        cnt += 1
                        if (cnt % 100) == 0:
                            print("({}) {:.0f}".format(cnt, time.time()))
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
        
