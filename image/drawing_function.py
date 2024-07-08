import numpy as np
import cv2

# Create a black image
img = np.zeros((512, 512, 3), np.uint8)

# Draw a diagonal blue line with thickness of 5 px
img = cv2.line(img, (0, 0), (511, 511), (255, 0, 0), 5)

img = cv2.rectangle(img, (384, 0), (510, 128), (0, 255, 0), 3)

img = cv2.circle(img, (447, 63), 63, (0, 0, 255), -1)

img = cv2.ellipse(img, (256, 256), (100, 50), 0, 0, 180, 255, -1)

pts = np.array([[10, 5], [20, 30], [70, 20], [50, 10]], np.int32)
pts = pts.reshape((-1, 1, 2))
img = cv2.polylines(img, [pts], True, (0, 255, 255))

font = cv2.FONT_HERSHEY_SIMPLEX
cv2.putText(img, 'Arman: Hello OpenCV', (10, 500), font, 1, (255, 255, 255), 2, cv2.LINE_AA)

cv2.imshow('Drawing Function', img)

key = cv2.waitKey(0)
if key == 27: # wait for ESC key to exit window
    cv2.destroyAllWindows()
elif key == ord('s'): # wait for 's' key to save and exit
    # Write file from colorful image to grayscale
    cv2.imwrite('assets/drawing-function.jpg', img)
    cv2.destroyAllWindows()