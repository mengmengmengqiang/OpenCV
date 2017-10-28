import cv2

def reformat():
    image_jpg = cv2.imread('jieyi.jpg')
    cv2.imwrite('jieyi.png', image_jpg)
    #image_png = cv2.imread('jieyi.png')
    #cv2.imshow('jieyi.png', image_png)
    #cv2.waitKey(0)
    #cv2.destroyAllWindows()

reformat()