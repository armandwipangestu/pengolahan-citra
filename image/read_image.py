import numpy as np
import cv2
from matplotlib import pyplot as plt

# Load an color image in grayscale
img = cv2.imread('assets/me.jpg', 0)
# Load an colorful image (default)
img2 = cv2.imread('assets/me.jpg', 1)

# Using OpenCV
cv2.imshow('Image Grayscale', img)
cv2.imshow('Image Colorful', img2)

key = cv2.waitKey(0)
if key == 27: # wait for ESC key to exit window
    cv2.destroyAllWindows()
elif key == ord('s'): # wait for 's' key to save and exit
    # Write file from colorful image to grayscale
    cv2.imwrite('assets/me-grayscale.jpg', img)
    cv2.destroyAllWindows()

# Using Matplotlib
# plt.imshow(img, cmap = 'gray', interpolation='bicubic')
# plt.xticks([]), plt.yticks([]) # to hide tick values on X and Y axis
# plt.show()