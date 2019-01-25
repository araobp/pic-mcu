#
# Heatmap
#
# Reference: https://learn.adafruit.com/adafruit-amg8833-8x8-thermal-camera-sensor/raspberry-pi-thermal-camera
#

import numpy as np
import math
import seaborn as sns; sns.set()
from scipy.interpolate import griddata
from scipy.fftpack import dct, idct

import interface

### Constants #####

# Points and grids for interpolation of 2d image
POINTS = [(math.floor(n / 8), (n % 8)) for n in range(0, 64)]
GRID_X, GRID_Y = np.mgrid[0:7:32j, 0:7:32j]

import sklearn.preprocessing as pp

def dct_2d(image):
  return dct(dct(image.T, norm='ortho').T, norm='ortho')

def idct_2d(coef):
  return idct(idct(coef.T, norm='ortho').T, norm='ortho')

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
                    data = griddata(POINTS, data, (GRID_X, GRID_Y), method='cubic')
                    # image format
                    data_flip = np.flip(np.flip(data.reshape(32,32), axis=0), axis=1)
                else:
                    # image format
                    data_flip = np.flip(np.flip(data.reshape(8,8), axis=0), axis=1)

                axes[0].set_title('Heat map')
                
                if cmd == interface.DIFF:
                    vmin, vmax = -10.0, 10.0
                elif cmd == interface.MOTION_DETECTION:
                    vmin, vmax = -1, 1
                else:
                    vmin, vmax = None, None

                if self.grid_data:
                    sns.heatmap(data_flip, cmap=cmap, vmin=vmin, vmax=vmax, ax=axes[0], annot=False, cbar_ax=axes[1])
                elif cmd == interface.MOTION_DETECTION:                  
                    sns.heatmap(data_flip, cmap=cmap, vmin=vmin, vmax=vmax, ax=axes[0], annot=True, cbar=False)
                else:
                    sns.heatmap(data_flip, cmap=cmap, vmin=vmin, vmax=vmax, ax=axes[0], annot=True, cbar=False)              
                    axes[1].set_title('DCT')
                    coef = dct_2d(data_flip)
                    coef[0,0] = 0  # Remove DC
                    max = np.abs(coef).max()
                    sns.heatmap(coef, cmap='bwr', vmin=-max, vmax=max, ax=axes[1], annot=True, cbar=False)

            elif cmd == interface.MOTION_COUNT:
                data = data.reshape(1, 8)
                axes[0].set_title('motion count')
                vmin, vmax = -1, 1
                sns.heatmap(data, cmap=cmap, vmin=vmin, vmax=vmax, ax=axes[0], annot=True, cbar=False)
              
        except Exception as e:
            print(e)
            
        return data
