from uwimg import *
import os

def delete_all_generated_images():
    file_list = ['corners.jpg', 'matches.jpg', 'easy_panorama.jpg',
    'rainier_panorama_1.jpg', 'rainier_panorama_2.jpg', 'rainier_panorama_3.jpg',
    'rainier_panorama_4.jpg', 'rainier_panorama_5.jpg', 'field_panorama_1.jpg',
    'field_panorama_2.jpg', 'field_panorama_3.jpg', 'field_panorama_4.jpg',
    'field_panorama_5.jpg'
    ]
    for filename in file_list:
        if os.path.isfile(filename):
            os.remove(filename)              
            print(f'{filename} deleted')
    print()


def draw_corners():
    print('draw_corners')
    im = load_image("data/Rainier1.png")
    detect_and_draw_corners(im, 2, 50, 3)
    save_image(im, "corners")
    print('draw_corners done')

def draw_matches():
    print('draw_matches')
    a = load_image("data/Rainier1.png")
    b = load_image("data/Rainier2.png")
    m = find_and_draw_matches(a, b, 2, 50, 3)
    save_image(m, "matches")
    print('draw_matches done')

def easy_panorama():
    print('easy_panorama')
    im1 = load_image("data/Rainier1.png")
    im2 = load_image("data/Rainier2.png")
    pan = panorama_image(im1, im2, thresh=50)
    save_image(pan, "easy_panorama")
    print('easy_panorama done')

def rainier_panorama():
    print('rainier_panorama')
    im1 = load_image("data/Rainier1.png")
    im2 = load_image("data/Rainier2.png")
    im3 = load_image("data/Rainier3.png")
    im4 = load_image("data/Rainier4.png")
    im5 = load_image("data/Rainier5.png")
    im6 = load_image("data/Rainier6.png")
    pan = panorama_image(im1, im2, thresh=5)
    print('\trainer pan1')
    save_image(pan, "rainier_panorama_1")
    pan2 = panorama_image(pan, im5, thresh=5)
    print('\trainer pan2')
    save_image(pan2, "rainier_panorama_2")
    pan3 = panorama_image(pan2, im6, thresh=5)
    print('\trainer pan3')
    save_image(pan3, "rainier_panorama_3")
    pan4 = panorama_image(pan3, im3, thresh=5)
    print('\trainer pan4')
    save_image(pan4, "rainier_panorama_4")
    pan5 = panorama_image(pan4, im4, thresh=5)
    print('\trainer pan5')
    save_image(pan5, "rainier_panorama_5")
    print('rainier_panorama done')


def field_panorama():
    print('field_panorama')
    im1 = load_image("data/field1.jpg")
    im2 = load_image("data/field2.jpg")
    im3 = load_image("data/field3.jpg")
    im4 = load_image("data/field4.jpg")
    im5 = load_image("data/field5.jpg")
    im6 = load_image("data/field6.jpg")
    im7 = load_image("data/field7.jpg")
    im8 = load_image("data/field8.jpg")

    im1 = cylindrical_project(im1, 1200)
    print('\tcylindrical project 1')
    im2 = cylindrical_project(im2, 1200)
    print('\tcylindrical project 2')
    im3 = cylindrical_project(im3, 1200)
    print('\tcylindrical project 3')
    im4 = cylindrical_project(im4, 1200)
    print('\tcylindrical project 4')
    im5 = cylindrical_project(im5, 1200)
    print('\tcylindrical project 5')
    im6 = cylindrical_project(im6, 1200)
    print('\tcylindrical project 6')
    im7 = cylindrical_project(im7, 1200)
    print('\tcylindrical project 7')
    im8 = cylindrical_project(im8, 1200)
    print('\tcylindrical project 8')
    save_image(im1, "cylindrical_projection")

    pan = panorama_image(im5, im6, thresh=2, iters=50000, inlier_thresh=3)
    save_image(pan, "field_panorama_1")
    pan2 = panorama_image(pan, im7, thresh=2, iters=50000, inlier_thresh=3)
    save_image(pan2, "field_panorama_2")
    pan3 = panorama_image(pan2, im8, thresh=2, iters=50000, inlier_thresh=3)
    save_image(pan3, "field_panorama_3")
    pan4 = panorama_image(pan3, im4, thresh=2, iters=50000, inlier_thresh=3)
    save_image(pan4, "field_panorama_4")
    pan5 = panorama_image(pan4, im3, thresh=2, iters=50000, inlier_thresh=3)
    save_image(pan5, "field_panorama_5")
    # pan = panorama_image(im5, im6, thresh=2, iters=50000, inlier_thresh=3)
    # print('\tcylindrical project-pan 1')
    # save_image(pan, "field_panorama_1")
    # pan2 = panorama_image(pan, im4, thresh=2, iters=50000, inlier_thresh=3)
    # print('\tcylindrical project-pan 2')
    # save_image(pan2, "field_panorama_2")
    # pan3 = panorama_image(pan2, im3, thresh=2, iters=50000, inlier_thresh=3)
    # print('\tcylindrical project-pan 3')
    # save_image(pan3, "field_panorama_3")
    # pan4 = panorama_image(pan3, im7, thresh=2, iters=50000, inlier_thresh=3)
    # print('\tcylindrical project-pan 4')
    # save_image(pan4, "field_panorama_4")
    # pan5 = panorama_image(pan4, im8, thresh=2, iters=50000, inlier_thresh=3)
    # print('\tcylindrical project-pan 5')
    # save_image(pan5, "field_panorama_5")
    # print('field_panorama done')
    
delete_all_generated_images()
draw_corners()
draw_matches()
easy_panorama()
rainier_panorama()
field_panorama()

