import os
import cv2
from ocrConvertImageToText import readImage

cwd = os.getcwd()

# Variable yang akan digunakan untuk deteksi plat nomor
plateCascade = cv2.CascadeClassifier("../data/haarcascades/haarcascade_russian_plate_number.xml")

cam = cv2.VideoCapture(0)

cam.set(3, 640)
cam.set(4, 480)
cam.set(10, 150)

count = 1
minArea = 500

while (True):
    ret, frame = cam.read()
    if not ret:
        print("Failed to grab frame")
        break

    frameGray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    numberPlates = plateCascade.detectMultiScale(frameGray, 1.1, 4)

    for (x, w, y, h) in numberPlates:
        area = w * h
        if area > minArea:
            # Membuat box pada ROI (Region of Interest)
            cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)
            # Menambahkan teks
            cv2.putText(frame, "NumberPlate", (x, y - 5), cv2.FONT_HERSHEY_COMPLEX, 1, (0, 0, 255), 2)
            imgRoi = frame[y:y + h, x:x + w]
            cv2.imshow("ROI", imgRoi)
    
    cv2.imshow("Hasil", frame)

    k = cv2.waitKey(1)
    if k%256 == 27:
        # ESC pressed
        print("Escape hit, clossing")
        break
    elif k & k%256 == 32:
        cv2.imwrite(cwd + '/result/data' + str(count) + ".jpg", imgRoi)
        cv2.rectangle(frame, (0, 200), (640, 300), (0, 255, 0), cv2.FILLED)
        cv2.putText(frame, "Gambar Tersimpan", (15, 256), cv2.FONT_HERSHEY_COMPLEX, 1.75, (0, 0, 255), 2)
        cv2.imshow("Hasil", frame)
        cv2.waitKey(500)
        count += 1

readImage()