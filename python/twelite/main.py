'''
 "twelite" module test utility.

 Example usage:
 $ python main.py COM9 2 1 -q

'''
import twelite as tw
import time
import argparse
import traceback

### Serial port setting in bps
BAUDRATE = 115200

### Argument parser
parser = argparse.ArgumentParser()
parser.add_argument("port", help="Serial port identifier")
parser.add_argument("loop", help="The number of looping")
parser.add_argument("dst", help="Destination node identifier")
parser.add_argument("-p", "--performance_measurement", help="Performance measurement on 64 pixels data transmission over TWELITE", action='store_true')
parser.add_argument("-q", "--quality", help="Print out quality data (sequence number and LQI) as well", action='store_true')
parser.add_argument("-m", "--motion_detection", help="Column-wise motion detection", action='store_true')
parser.add_argument("-M", "--motion_count", help="Motion count on a specific row", action='store_true')
parser.add_argument("-d", "--delay", help="Delay in a loop (in msec)", default="0")
args = parser.parse_args()

info = lambda seq, lqi: ' seq number: {}, LQI: {} ({} dBm)'.format(seq, lqi, tw.lqi2dbm(lqi))
def read_and_print_data(label, slave_id, cmd, quality_data=False):
    '''
    Read data from 8bit MCU and print the data.
    Note 1: 8bit MCU, as a co-processor of TWELITE (slave node), is for
    reading data from AMG8833 and transmitting it to the master node.
    Note 2: the sequence number is generated in MasterNode class

    '''
    print('--- SLAVE {} ---'.format(slave_id))
    data = mn.read(dst=slave_id, cmd=cmd, quality_data=quality_data)
    if quality_data:
        data, seq, lqi = data
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


if __name__ == '__main__':

    dst = int(args.dst)

    running = False
    
    with tw.MasterNode(args.port, BAUDRATE) as mn:

        start_time = time.time()
        err_cnt = 0

        delay = float(args.delay)/1000.0  # msec -> sec
        
        for _ in range(int(args.loop)):

            time.sleep(delay)                    

            try:
                if args.performance_measurement:
                    data = mn.read(dst=dst, cmd=tw.PIXELS, quality_data=False)
                elif args.motion_detection:
                    read_and_print_data('motion detection', dst, tw.MOTION_DETECTION, quality_data=args.quality)
                elif args.motion_count:
                    read_and_print_data('motion count', dst, tw.MOTION_COUNT, quality_data=args.quality)
                else:
                    #time.sleep(0.5)
                    ### Read room temperature data
                    read_and_print_data('room temperature', dst, tw.THERMISTOR, quality_data=args.quality)

                    ### Read 64 pixels data
                    read_and_print_data('64 pixels', dst, tw.PIXELS, quality_data=args.quality)

                    ### Read 64 pixels diff
                    read_and_print_data('64 pixels diff', dst, tw.DIFF, quality_data=args.quality)
                    
                    ### Read 64 pixels motion detection
                    read_and_print_data('motion detection', dst, tw.MOTION_DETECTION, quality_data=args.quality)

                    ### Read motion count
                    read_and_print_data('motion detection', dst, tw.MOTION_COUNT, quality_data=args.quality)

            except Exception as e:
                err_cnt += 1
                print('[{}] {}'.format(str(err_cnt), str(e)))
                #traceback.print_exc()
            
        stop_time = time.time()
        print('--- STATS ---')       
        print('Elapsed time: {:.3f} sec'.format(stop_time - start_time))
        print('Average interval: {:.1f} msec'.format((stop_time - start_time)/int(args.loop)*1000))
        print('Tranmission error: {} times'.format(err_cnt))
        
