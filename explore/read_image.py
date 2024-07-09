import numpy as np
import cv2
from matplotlib import pyplot as plt

img = cv2.imread('assets/me.jpg', 0)

# cv2.imshow('Image Grayscale', img)

# key = cv2.waitKey(0)

# if key == 27:
#     cv2.destroyAllWindows()
# elif key == ord('s'):
#     cv2.imwrite('assets/me-grayscale.jpg', img)
#     cv2.destroyAllWindows()

plt.imshow(img, cmap = 'gray', interpolation='bicubic')
plt.xticks([]), plt.yticks([])
plt.show()