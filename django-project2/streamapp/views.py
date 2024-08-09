import cv2
import os
from django.shortcuts import render
from django.http.response import StreamingHttpResponse, HttpResponse
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
        return HttpResponse(last_frame_with_roi, content_type="image/jpeg")
    else:
        return HttpResponse("No ROI Available", status=404)
