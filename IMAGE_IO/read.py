import cv2
import numpy

img = numpy.zeros((300,300), dtype=numpy.uint8)
#print(img)
img = cv2.cvtColor(img, cv2.COLOR_GRAY2BGR)
print(img)
print(img.shape)
cv2.imshow("test", img)