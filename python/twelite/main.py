'''
 "twelite" module test utility.


 Example usage:
 $ python main.py COM9 2 1 -q

'''
import twelite as tw
import time
import argparse
import traceback

### Serial port setting
BAUDRATE = 115200

### Argument parser
parser = argparse.ArgumentParser()
parser.add_argument("port", help="Serial port identifier")
parser.add_argument("loop", help="The number of looping")
parser.add_argument("dst", help="Destination node identifier")
parser.add_argument("-m", "--performance_measurement", help="Performance measurement on 64 pixels data transmission over TWELITE", action='store_true')
parser.add_argument("-q", "--quality", help="Print out quality data (sequence number and LQI) as well", action='store_true')
parser.add_argument("-s", "--sum_diff_only", help="Print out sum diff only", action='store_true')
parser.add_argument("-r", "--return_run", help="Return 'r(un)' command in response to 'h(ello)'", action='store_true')
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
    print(' {}: '.format(label), end='')
    if type(data) == float:
        print('{} degrees Celsius'.format(data))
    else:
        for elm in data[:-1]:
            print('{},'.format(elm), end='')
        print('{} degrees Celsius'.format(data[-1]))
    print('')


if __name__ == '__main__':

    dst = int(args.dst)

    running = False
    
    with tw.MasterNode(args.port, BAUDRATE) as mn:

        start_time = time.time()
        err_cnt = 0
        
        for _ in range(int(args.loop)):

            try:
                if args.return_run:
                    if not running:
                        data = mn.wait(dst=dst)
                        if data == tw.HELLO:  # h(ello) 
                            mn.write(dst=dst, cmd=tw.RUN)  # r(un)
                            print('r(un) sent in response to h(ello)')
                            running = True
                        else:
                            print('wait timeout')
                    else:
                        read_and_print_data('room temperature', dst, tw.THERMISTOR, quality_data=args.quality)
                        time.sleep(3)
                elif args.performance_measurement and args.sum_diff_only:
                    data = mn.read(dst=dst, cmd=tw.SUM_DIFF, quality_data=False)
                elif args.performance_measurement:
                    data = mn.read(dst=dst, cmd=tw.PIXELS, quality_data=False)
                elif args.sum_diff_only:
                    read_and_print_data('pixels sum diff', dst, tw.SUM_DIFF, quality_data=args.quality)
                else:
                    ### Read room temperature data from 8bit MCU
                    read_and_print_data('room temperature', dst, tw.THERMISTOR, quality_data=args.quality)

                    ### Read 64 pixels data from 8bit MCU
                    read_and_print_data('64 pixels', dst, tw.PIXELS, quality_data=args.quality)

                    ### Read 64 pixels diff from 8bit MCU
                    read_and_print_data('64 pixels diff', dst, tw.DIFF, quality_data=args.quality)

                    ### Read 64 pixels sum diff from 8bit MCU
                    read_and_print_data('pixels sum diff', dst, tw.SUM_DIFF, quality_data=args.quality)

            except Exception as e:
                #print(e)
                traceback.print_exc()
                err_cnt += 1
                print('transmission error: {}'.format(err_cnt))
            
        stop_time = time.time()
        print('--- STATS ---')       
        print('Elapsed time: {:.3f} sec'.format(stop_time - start_time))
        print('Average interval: {:.1f} msec'.format((stop_time - start_time)/int(args.loop)*1000))
        print('Tranmission error: {} times'.format(err_cnt))
        
