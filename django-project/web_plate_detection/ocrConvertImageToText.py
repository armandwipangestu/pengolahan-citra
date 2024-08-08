# detection/ocrConvertImageToText.py
import cv2
import easyocr

def read_image_from_frame(frame):
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    reader = easyocr.Reader(["en"])
    result = reader.readtext(gray)
    
    if result:
        return result[0][1]  # Mengembalikan text dari hasil OCR
    return "No text found"
