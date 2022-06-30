import cv2


filename = '../../../images/screenShot.bmp'
color_image = cv2.imread(filename, cv2.IMREAD_COLOR)
grayscale_image = cv2.imread(filename, cv2.IMREAD_GRAYSCALE)
thresh = 127
binary_image = cv2.threshold(grayscale_image, thresh, 255, cv2.THRESH_BINARY)[1]
contours, hierarchy = cv2.findContours(binary_image, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

image_with_contours = color_image.copy()
cv2.drawContours(image=image_with_contours, contours=contours, contourIdx=-1, color=(0, 255, 0), thickness=2, lineType=cv2.LINE_AA)

tt = contours[1]
print(tt)

cv2.imshow("image_with_contours", image_with_contours)
cv2.waitKey(0)
cv2.destroyAllWindows()