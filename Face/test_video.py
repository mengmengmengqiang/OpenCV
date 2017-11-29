import cv2

"""
    detect face from an video.

"""


def detect():
    global img
    face_cascade = cv2.CascadeClassifier('./cascades/haarcascade_frontalface_default.xml')

    eye_cascade = cv2.CascadeClassifier('./cascades/haarcascade_eye.xml')

    camera = cv2.VideoCapture(0)

    if camera.isOpened():
        ret, frame = camera.read()
    else:
        ret = False
        print('Can\'t open the camera')

    while ret:
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        ret, frame = camera.read()
        if not ret:
            print('error to get frame')
        faces = face_cascade.detectMultiScale(gray, 1.3, 5)

        for (x,y,w,h) in faces:
            img = cv2.rectangle(frame, (x,y), (x+w,y+h), (255,0,0), 2)
            roi_gray = gray[y:y+h, x:x+w]
            eyes = eye_cascade.detectMultiScale(roi_gray, 1.03, 5, 0, (40, 40))

            for (ex,ey,ew,eh) in eyes:
                cv2.rectangle(img, (ex, ey), (ex+ew, ey+eh), (0, 255, 0), 2)
                cv2.imshow("camera", frame)
                if cv2.waitKey(int(1000 / 12)) & 0xff == ord("q"):
                    break

    camera.release()
    cv2.destroyAllWindows()


if __name__ == "__main__":
    detect()
