import cv2


filename = 'C:/temp/OpenGL/EmguCV/ClientAndServer/images/screenShot.bmp'
color_image = cv2.imread(filename, cv2.IMREAD_COLOR)
grayscale_image = cv2.imread(filename, cv2.IMREAD_GRAYSCALE)
thresh = 127
binary_image = cv2.threshold(grayscale_image, thresh, 255, cv2.THRESH_BINARY)[1]
contours, hierarchy = cv2.findContours(binary_image, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

image_with_contours = color_image.copy()
cv2.drawContours(image=image_with_contours, contours=contours, contourIdx=-1, color=(0, 255, 0), thickness=2, lineType=cv2.LINE_AA)

tt = contours[1]
print(tt)

# cv2.imshow("image_with_contours", image_with_contours)
# cv2.waitKey(0)
# cv2.destroyAllWindows()


# import cv2
#
# filename = 'C:/temp/OpenGL/EmguCV/ClientAndServer/images/screenShot.bmp'
# image = cv2.imread(filename)
#
# # B, G, R channel splitting
# blue, green, red = cv2.split(image)
#
# # detect contours using blue channel and without thresholding
# contours1, hierarchy1 = cv2.findContours(image=blue, mode=cv2.RETR_TREE, method=cv2.CHAIN_APPROX_NONE)
#
# # draw contours on the original image
# image_contour_blue = image.copy()
# cv2.drawContours(image=image_contour_blue, contours=contours1, contourIdx=-1, color=(0, 255, 0), thickness=2, lineType=cv2.LINE_AA)
# # see the results
# cv2.imshow('Contour detection using blue channels only', image_contour_blue)
# cv2.waitKey(0)
# cv2.imwrite('blue_channel.jpg', image_contour_blue)
# cv2.destroyAllWindows()

