import cv2
import numpy as np
img = cv2.imread('jieyi.png')
print(img.item(150, 120, 0))
img.itemset( (150, 120, 0), 255)
print(img.item(150, 120, 0))

cv2.imshow('jieyi.png', img)

cv2.waitKey(0)
cv2.destroyAllWindows()