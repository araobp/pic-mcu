import serial
import time

#RAW_DATA_FILE = './{}_raw_data.csv'.format(time.time())
RAW_DATA_FILE = './raw_data.csv'

ser = serial.Serial('COM5', 115200)

if __name__ == '__main__':

    with open(RAW_DATA_FILE, 'w') as f:
        line = ser.readline().decode('ascii')
        f.write('cnt,gx,gy,gz,ax,ay,az\n')
        while True:
            line = ser.readline().decode('ascii')
            #print(line, end='', flush=True)
            f.write(line)
