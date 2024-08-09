import cv2
import os
import easyocr
import base64
import json
import numpy as np
from django.shortcuts import render
from django.http.response import StreamingHttpResponse, HttpResponse, JsonResponse
from .camera import VideoCamera, IPWebCam
from django.conf import settings

last_frame_with_roi = None

def index(request):
    return render(request, 'home.html')

def gen(camera):
    while True:
        frame = camera.get_frame()
        yield (b'--frame\r\n'
                b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n\r\n')
            
def genPlate(camera):
    global last_frame_with_roi
    while True:
        full_frame, frame_with_roi = camera.get_frame_plate()
        if frame_with_roi is not None:
            last_frame_with_roi = frame_with_roi
        yield (b'--frame\r\n'
                b'Content-Type: image/jpeg\r\n\r\n' + full_frame + b'\r\n\r\n')
        
def video_feed(request):
    return StreamingHttpResponse(gen(VideoCamera()),
                                content_type='multipart/x-mixed-replace; boundary=frame')

def video_plate_feed(request):
    return StreamingHttpResponse(genPlate(VideoCamera()),
                                content_type='multipart/x-mixed-replace; boundary=frame')

def capture_roi(request):
    if last_frame_with_roi is not None:
        # Apply OCR on ROI
        reader = easyocr.Reader(["en"])
        gray_roi = cv2.cvtColor(cv2.imdecode(np.frombuffer(last_frame_with_roi, np.uint8), cv2.IMREAD_COLOR), cv2.COLOR_BGR2GRAY)
        result = reader.readtext(gray_roi)
        ocr_text = ' '.join([text for (_, text, _) in result])

        # encode roi image as jpeg
        ret, jpeg = cv2.imencode('.jpg', cv2.imdecode(np.frombuffer(last_frame_with_roi, np.uint8), cv2.IMREAD_COLOR))

        # convert jpeg image to base64 string
        image_base64 = base64.b64encode(jpeg.tobytes()).decode()

        return JsonResponse({
            'image': f'data:image/jpeg;base64,{image_base64}',
            'text': ocr_text
        })
        # return HttpResponse(last_frame_with_roi, content_type="image/jpeg")
    else:
        return JsonResponse({"error": "No ROI Available"}, status=404)
        # return HttpResponse("No ROI Available", status=404)
