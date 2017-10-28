import cv2
import numpy as np

import reformat

reformat.reformat()
img = cv2.imread('jieyi.png')
print(img.shape)
print(img.size)
my_roi = img[200:600, 200:600]
img[600:1000, 600:1000] = my_roi
cv2.imshow('ROI', img)
cv2.waitKey(0)
cv2.destroyAllWindows()