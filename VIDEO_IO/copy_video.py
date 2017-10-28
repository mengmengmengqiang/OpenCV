import cv2

videoCapture = cv2.VideoCapture('video.avi')
fps = videoCapture.gei(cv2.CAP_PROP_FPS)
size = (int(videoCapture.get(cv2.CAP_PROP_FRAME_WIDTH)),
        int(videoCapture.get(cv2.CAP_PROP_FRAME_HEIGHT)))

videoWriter = cv2.VideoWriter('outvideo.avi', cv2.VideoWriter_fourcc('I', '4', '2', '0'), fps, size)

success, frame = videoCapture.read()

while success:
    videoWriter.write(frame)
    success, frame = videoCapture.read()