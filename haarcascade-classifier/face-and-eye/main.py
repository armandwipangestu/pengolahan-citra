import cv2

cap = cv2.VideoCapture(0)

# 1. Use model from manual data folder
# haarcascadeFrontalFacePath = "../data/haarcascades/haarcascade_frontalface_alt2.xml"
# frontalFaceCascade = cv2.CascadeClassifier(haarcascadeFrontalFacePath)

# 2. Use model from auto data folder from library opencv
haarcascadeFrontalFacePath = "haarcascade_frontalface_alt2.xml"
frontalFaceCascade = cv2.CascadeClassifier(cv2.data.haarcascades + haarcascadeFrontalFacePath)

# 1. Use model from manual data folder
# haarcascadeEyePath = "../data/haarcascades/haarcascade_eye.xml"
# eyeFaceCascade = cv2.CascadeClassifier(haarcascadeEyePath)

# 2. Use model from auto data folder from library opencv
haarcascadeEyePath = "haarcascade_eye.xml"
eyeFaceCascade = cv2.CascadeClassifier(cv2.data.haarcascades + haarcascadeEyePath)

while (True):
    ret, frame = cap.read()

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    faces = frontalFaceCascade.detectMultiScale(gray, 1.3, 5)

    for (x, w, y, h) in faces:
        font = cv2.FONT_HERSHEY_SIMPLEX
        cv2.putText(frame, 'Wajah Terdeteksi', (x, y - 10), font, 2, (255, 255, 255), 2, cv2.LINE_AA)

        frame = cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)
        roi_gray = gray[y:y + h, x:x + w]
        roi_color = frame[y: y + h, x:x + w]
        eyes = eyeFaceCascade.detectMultiScale(roi_gray)

        for (ex, ey, ew, eh) in eyes:
            cv2.rectangle(roi_color, (ex, ey), (ex + ew, ey + eh), (0, 255, 0), 2)

    cv2.imshow('Face & Eye Detection', frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()