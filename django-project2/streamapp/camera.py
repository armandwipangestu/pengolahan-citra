import os
import cv2
import urllib
import requests
import numpy as np
from django.conf import settings

haarcascadeFrontalFacePath = "haarcascade_frontalface_alt2.xml"
face_detection_videocam = cv2.CascadeClassifier(cv2.data.haarcascades + haarcascadeFrontalFacePath)
face_detection_webcam = cv2.CascadeClassifier(cv2.data.haarcascades + haarcascadeFrontalFacePath)
plate_detection = cv2.CascadeClassifier(os.path.join(settings.BASE_DIR, 'data/haarcascade_russian_plate_number.xml'))

frame_with_roi = None

class VideoCamera(object):
    def __init__(self):
        self.video = cv2.VideoCapture(0)

    def __del__(self):
        self.video.release()

    def get_frame(self):
        success, image = self.video.read()

        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        faces_detected = face_detection_videocam.detectMultiScale(gray, scaleFactor=1.3, minNeighbors=5)
        
        for (x, y, w, h) in faces_detected:
            cv2.rectangle(image, pt1=(x, y), pt2=(x + w, y + h), color=(255, 0, 0), thickness=2)
        
        frame_flip = cv2.flip(image, 1)
        ret, jpeg = cv2.imencode('.jpg', frame_flip)
        return jpeg.tobytes()
    
    def get_frame_plate(self):
        global frame_with_roi
        # self.video.set(3, 640)
        # self.video.set(4, 480)
        # self.video.set(10, 150)

        count = 1
        minArea = 500

        success, frame = self.video.read()
        if not success:
            print('Failed to grab frame')
            return False

        frameGray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        numberPlates = plate_detection.detectMultiScale(frameGray, scaleFactor=1.1, minNeighbors=4)

        for (x, w, y, h) in numberPlates:
            area = w * h
            if area > minArea:
                frame_with_roi = frame[y:y + h, x:x + w]
                cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)
                cv2.putText(frame, 'NumberPlate', (x, y - 5), cv2.FONT_HERSHEY_COMPLEX, 1, (0, 0, 255), 2)
        
        # frame_flip = cv2.flip(frame, 1)
        ret, jpeg_frame = cv2.imencode('.jpg', frame)

        if frame_with_roi is not None:
            ret, jpeg_roi = cv2.imencode('.jpg', frame_with_roi)
        else:
            jpeg_roi = None

        return jpeg_frame.tobytes(), jpeg_roi.tobytes() if jpeg_roi is not None else None

class IPWebCam(object):
    def __init__(self):
        self.url = "http://192.168.188.147:8080/shot.jpg"
    
    def __del__(self):
        cv2.destroyAllWindows()

    def get_frame(self):
        imgResp = urllib.request.urlopen(self.url)
        imgNp = np.array(bytearray(imgResp.read()),dtype=np.uint8)
        img= cv2.imdecode(imgNp,-1)
        # We are using Motion JPEG, but OpenCV defaults to capture raw images,
        # so we must encode it into JPEG in order to correctly display the
        # video stream
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        faces_detected = face_detection_webcam.detectMultiScale(gray, scaleFactor=1.3, minNeighbors=5)
        for (x, y, w, h) in faces_detected:
            cv2.rectangle(img, pt1=(x, y), pt2=(x + w, y + h), color=(255, 0, 0), thickness=2)
        
        resize = cv2.resize(img, (640, 480), interpolation = cv2.INTER_LINEAR) 
        frame_flip = cv2.flip(resize,1)
        ret, jpeg = cv2.imencode('.jpg', frame_flip)
        return jpeg.tobytes()