import os
import cv2
import urllib
import requests
import numpy as np
from django.conf import settings

haarcascadeFrontalFacePath = "haarcascade_frontalface_alt2.xml"
face_detection_videocam = cv2.CascadeClassifier(cv2.data.haarcascades + haarcascadeFrontalFacePath)
face_detection_webcam = cv2.CascadeClassifier(cv2.data.haarcascades + haarcascadeFrontalFacePath)

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

class IPWebCam(object):
    def __init__(self):
        self.url = "http://192.168.188.147:8080/shot.jpg"
    
    def __del__(self):
        cv2.destroyAllWindows()
    
    def get_frame(self):
        imgResp = urllib.request.urlopen(self.url)
        imgNp = np.array(bytearray(imgResp.read()), dtype=np.uint8)
        img = cv2.imencode(imgNp, -1)

        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        faces_detected = face_detection_webcam.detectMultiScale(gray, scaleFactor=1.3, minNeighbors=5)

        for (x, y, w, h) in faces_detected:
            cv2.rectangle(img, pt1=(x, y), pt2=(x + w, y + h), color=(255, 0, 0), thickness=2)
        
        resize = cv2.resize(img, (640, 480), interpolation=cv2.INTER_LINEAR)
        frame_flip = cv2.flip(resize, 1)
        ret, jpeg = cv2.imencode('.jpg', frame_flip)
        return jpeg.tobytes()