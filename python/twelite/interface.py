import twelite_amg8833 as tw

### Serial port setting
BAUDRATE = 115200

THERMISTOR = tw.THERMISTOR
PIXELS = tw.PIXELS
DIFF = tw.DIFF
MOTION_DETECTION = tw.MOTION_DETECTION
MOTION_COUNT = tw.MOTION_COUNT
DUMP_SETTINGS = tw.DUMP_SETTINGS

class Interface:
    
    def __init__(self, port, dst):
        self.port = port
        self.dst = int(dst)
        self.ser = tw.MasterNode(port, BAUDRATE)

    def read(self, cmd, ssub=None):
        return self.ser.read(dst=self.dst, cmd=cmd)

    def close(self):
        self.ser.close()

