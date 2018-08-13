import serial
import time

#RAW_DATA_FILE = './{}_raw_data.csv'.format(time.time())
RAW_DATA_FILE = './raw_data.csv'
PORT = 'COM14'

ser = serial.Serial(PORT, 115200)

ser.write(b'd')
if __name__ == '__main__':
    with open(RAW_DATA_FILE, 'w') as f:
        while True:
            line = ser.readline().decode('ascii')
            f.write(line)
ser.close()
