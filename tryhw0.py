from uwimg import *

# 1. Getting and setting pixels
print('Getting and setting pixels')
im = load_image("data/dog.jpg")
for row in range(im.h):
    for col in range(im.w):
        set_pixel(im, col, row, 0, 0)
save_image(im, "dog_no_red")

# 3. Grayscale image
print('Grayscale image')
im = load_image("data/colorbar.png")
graybar = rgb_to_grayscale(im)
save_image(graybar, "graybar")

# 4. Shift Image
print('Shift Image')
im = load_image("data/dog.jpg")
shift_image(im, 0, .4)
shift_image(im, 1, .4)
shift_image(im, 2, .4)
save_image(im, "overflow")

# 5. Clamp Image
print('Clamp Image')
clamp_image(im)
save_image(im, "doglight_fixed")

# 6-7. Colorspace and saturation
print('Colorspace and saturation')
im = load_image("data/dog.jpg")
rgb_to_hsv(im)
shift_image(im, 1, .2)
clamp_image(im)
hsv_to_rgb(im)
save_image(im, "dog_saturated")


# 8. Super duper extra credit
print('RGB to HCL')
im = load_image("data/dog.jpg")
rgb_to_hcl(im)
save_image(im, "dog_HCL")
hcl_to_rgb(im)
save_image(im, "dog_RGB_from_HCL")
# XYZ
im = load_image("data/dog.jpg")
rgb_to_xyz(im)
save_image(im, "dog_HCL")
xyz_to_rgb(im)
save_image(im, "dog_RGB_from_XYZ")





