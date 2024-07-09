import numpy as np
import cv2

img = np.zeros((512, 512, 3), np.uint8)

img = cv2.line(img, (0, 0), (511, 511), (255, 0, 0), 5)
img = cv2.rectangle(img, (384, 0), (510, 128), (0, 255, 0), 3)

font = cv2.FONT_HERSHEY_SIMPLEX
cv2.putText(img, 'Arman: Hello OpenCV', (10, 500), font, 1, (255, 255, 255), 2, cv2.LINE_AA)

cv2.imshow('Drawing Function', img)

key = cv2.waitKey(0)
if key == 27:
    cv2.destroyAllWindows()
elif key == ord('s'):
    cv2.imwrite('assets/drawing-function.jpg', img)
    cv2.destroyAllWindows()