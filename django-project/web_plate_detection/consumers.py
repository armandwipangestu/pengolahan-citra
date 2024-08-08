import cv2
import numpy as np
import base64
from channels.generic.websocket import WebsocketConsumer
import json

class DetectionConsumer(WebsocketConsumer):
    def connect(self):
        self.accept()

    def disconnect(self, close_code):
        pass

    def receive(self, text_data):
        data = json.loads(text_data)
        frame_data = data['image']
        frame = self.convert_data_uri_to_cv2_image(frame_data)
        detected_frame, plate_text = self.detect_plate(frame)
        self.send(text_data=json.dumps({
            'image': self.convert_cv2_image_to_data_uri(detected_frame),
            'text': plate_text
        }))

    def convert_data_uri_to_cv2_image(self, data_uri):
        encoded_data = data_uri.split(',')[1]
        nparr = np.frombuffer(base64.b64decode(encoded_data), np.uint8)
        img = cv2.imdecode(nparr, cv2.IMREAD_COLOR)
        return img
    
    def convert_cv2_image_to_data_uri(self, img):
        _, buffer = cv2.imencode('.jpg', img)
        jpg_as_text = base64.b64encode(buffer).decode('utf-8')
        return 'data:image/jpeg;base64,' + jpg_as_text
    
    def detect_plate(self, frame):
        plate_cascade = cv2.CascadeClassifier("../../haarcascade-classifier/data/haarcascades/haarcascade_russian_plate_number.xml")
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        plates = plate_cascade.detectMultiScale(gray, 1.1, 4)
        plate_text = ""

        for (x, y, w, h) in plates:
            cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)
            roi_gray = gray[y:y + h, x:x + w]
            plate_text = self.read_text_from_image(roi_gray)
            break

        return frame, plate_text
    
    def read_text_from_image(self, img):
        import easyocr
        reader = easyocr.Reader(['en'])
        result = reader.readtext(img)
        if result:
            return result[0][1]
        return ""