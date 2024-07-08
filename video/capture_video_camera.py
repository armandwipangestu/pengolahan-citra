import numpy as np
import cv2

# Load Video from WebCam using 0, 1, etc for devices
# cap = cv2.VideoCapture(1)
# Load Video from File
cap = cv2.VideoCapture('assets/test-video.mp4')

while(True):
    # Capture frame-by-frame
    ret, frame = cap.read()
    
    # Out operations on the frame come here
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Display the resulting frame
    cv2.imshow('Frame', frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()