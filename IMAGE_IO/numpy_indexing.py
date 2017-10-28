import cv2
import numpy as np
img = cv2.imread('jieyi.png')
img[:,:,2] = 255
cv2.imwrite('jieyi.png', img)
cv2.imshow('jieyi.png', img)
cv2.waitKey(0)
cv2.destroyAllWindows()