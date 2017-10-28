import cv2
import numpy as np

img = cv2.imread('github.png')

r, g, b = cv2.split(img)

# 按照图片通道的格式再生成一个alpha通道
a = np.zeros(img[:,:,0].shape, dtype=img.dtype)
for i in range(r.shape[0]):
    for j in range(r.shape[1]):
        if r[i, j]==0 and b[i, j]==0 and g[i, j]==0:
            a[i,j] = 250


print(img)
merged = cv2.merge([r, g, b, a]) # png带alpha通道的图片格式是RGBA
print(merged)

cv2.imwrite('github_alpha.png', merged)
img = cv2.imread('github_alpha.png', cv2.IMREAD_UNCHANGED)
print(img.shape)
assert isinstance(img, object)
cv2.imshow('github_alpha.png', img)
k = cv2.waitKey(0)