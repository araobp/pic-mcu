import twelite as tw

### Constants #####

### Serial port setting
BAUDRATE = 115200
SLAVE_1 = 1

THERMISTOR = tw.THERMISTOR
PIXELS = tw.PIXELS
DIFF = tw.DIFF
SUM_DIFF = tw.SUM_DIFF

class Interface:
    
    def __init__(self, port):
        self.port = port
        self.ser = tw.MasterNode(port, BAUDRATE)

    def read(self, cmd, ssub=None):
        return self.ser.read(dst=SLAVE_1, cmd=cmd)

    def close(self):
        self.ser.close()

