#
# This is a sample code to use "twelite" module.
#

import twelite as tw
import time

# Serial port
PORT = "COM9"
BAUDRATE = 115200

# Destination addresses of TWELITE-DIP slave nodes
SLAVE_1 = 0x01
SLAVE_2 = 0x02
SLAVE_3 = 0x03
SLAVE_4 = 0x04

if __name__ == '__main__':

    with tw.MasterNode(PORT, BAUDRATE) as mn:

        while True:
            try:
                print('--- SLAVE {} ---'.format(SLAVE_1))

                data, len_, seq, lqi, ck = mn.fetch(dst=SLAVE_1, cmd=tw.THERMISTOR)
                print(' length: {}, seq number: {}, LQI: {}'.format(len_, seq, lqi))                 
                print(' room temperature: {} degrees Celsius'.format(str(data[0])))
                print('')
                data, len_, seq, lqi, ck  = mn.fetch(dst=SLAVE_1, cmd=tw.PIXELS)
                print(' length: {}, seq number: {}, LQI: {}'.format(len_, seq, lqi))                 
                print(' pixels: ', end='')
                for d in data[:-1]:
                    print('{},'.format(str(d)), end='')
                print('{} degrees Celsius'.format(str(data[-1])))
                print('')
            except Exception as e:
                print(e)
                time.sleep(1)
            
            time.sleep(0.3)
