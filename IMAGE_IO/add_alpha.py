import cv2
import numpy as np

img = cv2.imread('github.png')

r, g, b = cv2.split(img)

# 按照图片通道的格式再生成一个alpha通道
a = np.zeros(img[:,:,0].shape, dtype=img.dtype)
#g[:, :] = 255
for i in range(r.shape[0]):
    for j in range(r.shape[1]):
        if r[i, j]==255 and b[i, j]==255 and g[i, j]==255:
            a[i,j] = 0
        else:
            a[i,j] = 255


#print(img)
#merged1 = cv2.merge([r, g, b])
merged2 = cv2.merge([a, a, a, a]) # png带alpha通道的图片格式是RGBA
print(merged2)

#cv2.imwrite('github_green.png', merged1)
cv2.imwrite('github_alpha.png', merged2)
#image = cv2.imread('github_alpha.png', cv2.IMREAD_UNCHANGED)

#assert isinstance(img, object)
#cv2.imshow('github_alpha.png', image)
#k = cv2.waitKey(0)