import cv2
img=cv2.imread("github_alpha.png", cv2.IMREAD_UNCHANGED)
cv2.imshow("github_alpha.png",img)
cv2.waitKey(0)
cv2.destroyAllWindows()