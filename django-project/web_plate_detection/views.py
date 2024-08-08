# detection/views.py
from django.shortcuts import render
from django.http import JsonResponse
from django.http.response import StreamingHttpResponse
import cv2
import base64
import numpy as np
from .plate_detection import detect_plate
# from streamapp.camera import VideoCamera, IPWebCam
from .views2 import VideoCamera

def index(request):
	return render(request, 'index.html')

def detect(request):
    if request.method == "POST":
        data = request.POST.get('image')
        img_data = base64.b64decode(data.split(',')[1])

        # np_img = np.fromstring(img_data, np.uint8)
        np_img = np.frombuffer(img_data, np.uint8)
        # frame = cv2.imdecode(np_img, cv2.IMREAD_COLOR)
        frame = cv2.imread('result/test2.jpeg')

        # cv2.imshow("Hasil", frame)

        plate_text = detect_plate(frame)

        return JsonResponse({"text": plate_text})
    return JsonResponse({"error": "Invalid request"}, status=400)

def gen(camera):
    while True:
        frame = camera.get_frame()
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n\r\n')

def video_feed(request):
    return StreamingHttpResponse(gen(VideoCamera()), 
                                 content_type='multipart/x-mixed-replace; boundary=frame')

# def webcam_feed(request):
#     return StreamingHttpResponse(gen(IPWebCam()), 
#                                  content_type='multipart/x-mixed-replace; boundary=frame')