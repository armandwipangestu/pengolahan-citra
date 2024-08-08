# detection/plate_detection.py
import os
import cv2
from .ocrConvertImageToText import read_image_from_frame

# plate_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + "haarcascade_russian_plate_number.xml")
# plate_cascade = cv2.CascadeClassifier("../../haarcascade-classifier/data/haarcascades/haarcascade_russian_plate_number.xml")
# plate_cascade = cv2.CascadeClassifier("haarcascade_russian_plate_number.xml")
cascade_path = os.path.join(os.path.dirname(__file__), 'haarcascade_russian_plate_number.xml')
plate_cascade = cv2.CascadeClassifier(cascade_path)

haarcascadeFrontalFacePath = "haarcascade_frontalface_alt2.xml"
frontalFaceCascade = cv2.CascadeClassifier(cv2.data.haarcascades + haarcascadeFrontalFacePath)

haarcascadeEyePath = "haarcascade_eye.xml"
eyeFaceCascade = cv2.CascadeClassifier(cv2.data.haarcascades + haarcascadeEyePath)

min_area = 500

def detect_plate(frame):
    frame_gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    number_plates = plate_cascade.detectMultiScale(frame_gray, 1.1, 4)

    for (x, y, w, h) in number_plates:
        area = w * h
        if area > min_area:
            roi = frame[y:y+h, x:x+w]
            text = read_image_from_frame(roi)
            return text
    return "No plate detected"

class VideoCamera(object):
    def __init__(self):
        self.video = cv2.VideoCapture(0)

    def __del__(self):
        self.video.release()

    def get_frame(self):
        success, image = self.video.read()

        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        face_detected = frontalFaceCascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=4)
        for (x, y, w, h) in face_detected:
            cv2.rectangle(image, pt1=(x, y), pt2=(x + w, y + h), color=(255, 0, 0), thickness=2)
        frame_flip = cv2.flip(image, 1)
        ret, jpeg = cv2.imencode('.jpg', frame_flip)
        return jpeg.tobytes()
