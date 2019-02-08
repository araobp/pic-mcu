#
# Heatmap
#
# Reference: https://learn.adafruit.com/adafruit-amg8833-8x8-thermal-camera-sensor/raspberry-pi-thermal-camera
#

import numpy as np
import math
import seaborn as sns; sns.set()
from scipy.interpolate import griddata

import interface

### Constants #####

# Points and grids for interpolation of 2d image
POINTS = [(math.floor(n / 8), (n % 8)) for n in range(0, 64)]
GRID_X, GRID_Y = np.mgrid[0:7:32j, 0:7:64j]

POINTS_TWIN = [(j, i) for j in range(8) for i in range(16)]
GRID_X_TWIN, GRID_Y_TWIN = np.mgrid[0:7:32j, 0:15:64j]

import sklearn.preprocessing as pp

###################

class GUI:

    def __init__(self, interface, grid_data):
        self.interface = interface
        self.grid_data = grid_data
        
    # Use matplotlib to plot the output from the device
    def plot(self, axes, cmd, cmap=None, ssub=None):

        for ax in axes:
            ax.get_xaxis().set_ticks([])
            ax.get_yaxis().set_ticks([])

        data = None
        try:
            data = self.interface.read(cmd)
            if cmd == interface.PIXELS or cmd == interface.DIFF or cmd == interface.MOTION_DETECTION:

                if self.grid_data:
                    if len(data) == 64:
                        data = griddata(POINTS, data, (GRID_X, GRID_Y), method='cubic')
                        # image format
                        data_flip = np.flip(np.flip(data.reshape(32,32), axis=0), axis=1)
                    elif len(data) == 128:
                        data = griddata(POINTS_TWIN, data, (GRID_X_TWIN, GRID_Y_TWIN), method='cubic')
                        # image format
                        data_flip = np.flip(np.flip(data.reshape(32,64), axis=0), axis=1)
                else:
                    # image format
                    if len(data) == 64:
                        data_flip = np.flip(np.flip(data.reshape(8,8), axis=0), axis=1)
                    elif len(data) == 128:
                        data_flip = np.flip(np.flip(data.reshape(8,16), axis=0), axis=1)
                      
                axes[0].set_title('Heat map')
                
                if cmd == interface.DIFF:
                    vmin, vmax = -10.0, 10.0
                elif cmd == interface.MOTION_DETECTION:
                    vmin, vmax = -1, 1
                else:
                    vmin, vmax = None, None

                if cmd == interface.MOTION_DETECTION:                  
                    sns.heatmap(data_flip, cmap=cmap, vmin=vmin, vmax=vmax, ax=axes[0], annot=True, cbar=False)
                else:
                    sns.heatmap(data_flip, cmap=cmap, vmin=vmin, vmax=vmax, ax=axes[0], annot=False, cbar_ax=axes[1])

            elif cmd == interface.MOTION_COUNT:
                data = data.reshape(1, 8)
                axes[0].set_title('motion count')
                vmin, vmax = -1, 1
                sns.heatmap(data, cmap=cmap, vmin=vmin, vmax=vmax, ax=axes[0], annot=True, cbar=False)
              
        except Exception as e:
            print(e)
            
        return data
