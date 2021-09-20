import cv2


image = cv2.imread('data/dog.jpg')
hcl_image = cv2.cvtColor(image, cv2.COLOR_BGR2XYZ )
cv2.imwrite('dog_xyz_cv2.jpg', hcl_image)