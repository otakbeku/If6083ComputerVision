#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"

void l1_normalize(image im)
{
    float l1_norm = 1 / (im.w * im.h);
    float sum = 0;
    // float check = 0;
    for (int c = 0; c < im.c; ++c)
    {
        for (int y = 0; y < im.h; ++y)
        {
            for (int x = 0; x < im.w; ++x)
            {
                // float value = get_pixel(im, x, y, c);
                sum += get_pixel(im, x, y, c);
            }
        }
    }
    for (int c = 0; c < im.c; ++c)
    {
        for (int y = 0; y < im.h; ++y)
        {
            for (int x = 0; x < im.w; ++x)
            {
                if (sum != 0)
                {
                    float value = get_pixel(im, x, y, c) / sum;
                    set_pixel(im, x, y, c, value);
                    // check += value;
                }
                else
                {
                    set_pixel(im, x, y, c, l1_norm);
                    // check += l1_norm;
                }
            }
        }
    }
    // printf("Total: %f\n", check);
}

image make_box_filter(int w)
{
    image filter = make_image(w, w, 1);
    for (int y = 0; y < w; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            set_pixel(filter, x, y, 0, 1);
        }
    }

    l1_normalize(filter);
    return filter;
}

image convolve_image(image im, image filter, int preserve)
{
    assert(filter.c == 1 || filter.c == im.c);
    image result = copy_image(im);
    if (!preserve)
    {
        result = make_image(im.w, im.h, 1);
    }

    for (int c = 0; c < im.c; ++c)
    {
        for (int y = 0; y < im.h; ++y)
        {
            for (int x = 0; x < im.w; ++x)
            {
                int index_not_preserve = y * im.w + x; // Posisi pixel;
                float q = 0;
                int cf = c;
                if (filter.c == 1)
                {
                    cf = 0;
                }

                for (int yf = 0; yf < filter.h; ++yf)
                {
                    for (int xf = 0; xf < filter.w; ++xf)
                    {
                        float x_image, y_image, im_px;
                        x_image = x - filter.w / 2;
                        y_image = y - filter.h / 2;
                        im_px = get_pixel(im, x_image + xf, y_image + yf, c);
                        q += get_pixel(filter, xf, yf, cf) * im_px;
                    }
                }
                if (preserve)
                {
                    set_pixel(result, x, y, c, q);
                }
                else
                {
                    result.data[index_not_preserve] += q;
                }
            }
        }
        // if (!preserve){set_pixel(result, x, y, c, q);}
    }

    return result;
}

image make_highpass_filter()
{
    image filter = make_box_filter(3);
    // [0 1 2]
    // [3 4 5]
    // [6 7 8]

    // highpass filter
    // [0  -1  0]
    // [-1  4 -1]
    // [0  -1  0]
    filter.data[0] = filter.data[2] = filter.data[6] = filter.data[8] = 0;
    filter.data[1] = filter.data[3] = filter.data[5] = filter.data[7] = -1;
    filter.data[4] = 4;
    return filter;
}

image make_sharpen_filter()
{
    image filter = make_box_filter(3);
    // [0 1 2]
    // [3 4 5]
    // [6 7 8]

    // sharpen filter
    // [0  -1  0]
    // [-1  5 -1]
    // [0  -1  0]
    filter.data[0] = filter.data[2] = filter.data[6] = filter.data[8] = 0;
    filter.data[1] = filter.data[3] = filter.data[5] = filter.data[7] = -1;
    filter.data[4] = 5;
    return filter;
}

image make_emboss_filter()
{
    image filter = make_box_filter(3);
    // [0 1 2]
    // [3 4 5]
    // [6 7 8]

    // emboss filter
    // [-2 -1  0]
    // [-1  1  1]
    // [ 0  1  2]
    filter.data[0] = -2;
    filter.data[1] = filter.data[3] = -1;
    filter.data[7] = filter.data[5] = filter.data[4] = 1;
    filter.data[6] = filter.data[2] = 0;
    filter.data[8] = 2;
    return filter;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{
    // TODO
    // Kernel size should be 6 times the size of sigma
    // But the number is odd
    // So it use the next highest odd number from 6x sigma
    int kernel_size = (int)ceil(sigma * 6) + 1;
    float pi = 3.14159265359;
    if (kernel_size % 2 == 0)
    {
        kernel_size += 1;
    }
    int x_center = kernel_size / 2;
    int y_center = kernel_size / 2;

    image filter = make_box_filter(kernel_size);
    for (int y = 0; y < kernel_size; ++y)
    {
        for (int x = 0; x < kernel_size; ++x)
        {
            float sq_sigma = sigma * sigma;
            float frac = 1 / ((2 * pi) * sq_sigma);
            float x_dist = x - x_center;
            float y_dist = y - y_center;
            float frac_2 = -1 * ((x_dist * x_dist) + (y_dist * y_dist)) / (2 * sq_sigma);
            frac_2 = exp(frac_2);
            set_pixel(filter, x, y, 0, frac * frac_2);
        }
    }
    l1_normalize(filter);
    return filter;
    // return make_image(1, 1, 1);
}

int get_image_size(image im)
{
    // return sizeof(im.data) / sizeof(int);
    return im.c * im.w * im.h;
}

image add_image(image a, image b)
{
    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image temp = make_image(a.w, a.h, a.c);

    for (int index = 0; index < get_image_size(temp); ++index)
    {
        temp.data[index] = a.data[index] + b.data[index];
    }
    return temp;
    // return make_image(1, 1, 1);
}

image sub_image(image a, image b)
{

    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image temp = make_image(a.w, a.h, a.c);

    for (int index = 0; index < get_image_size(temp); ++index)
    {
        temp.data[index] = a.data[index] - b.data[index];
    }
    return temp;
    // return make_image(1, 1, 1);
}

image make_gx_filter()
{
    image filter = make_box_filter(3);
    // [0 1 2]
    // [3 4 5]
    // [6 7 8]

    // filter gx
    // [-1 0 1]
    // [-2 0 2]
    // [-1 0 1]
    filter.data[0] = filter.data[6] = -1;
    filter.data[3] = -2;
    filter.data[1] = filter.data[4] = filter.data[7] = 0;
    filter.data[5] = 2;
    filter.data[2] = filter.data[8] = 1;
    return filter;
}

image make_gy_filter()
{
    image filter = make_box_filter(3);
    // [0 1 2]
    // [3 4 5]
    // [6 7 8]

    // filter gy
    // [-1 -2 -1]
    // [ 0  0  0]
    // [ 1  2  1]
    filter.data[0] = filter.data[2] = -1;
    filter.data[3] = filter.data[4] = filter.data[5] = 0;
    filter.data[1] = -2;
    filter.data[7] = 2;
    filter.data[6] = filter.data[8] = 1;
    return filter;
}

void feature_normalize(image im)
{
    float min_val = im.data[0];
    float max_val = im.data[0];
    for(int index = 0; index < get_image_size(im); ++index){
        if(im.data[index] > max_val) max_val = im.data[index];
        if(im.data[index] < min_val) min_val = im.data[index];
    }
    for(int index = 0; index < get_image_size(im); ++index){
        im.data[index] = (im.data[index] - min_val)/(max_val-min_val);
    }
}


image *sobel_image(image im)
{
    // allocate memmories
    image *ret = calloc(2, sizeof(image));

    image filter_gx = make_gx_filter();
    image filter_gy = make_gy_filter();
    image img_gx = convolve_image(im, filter_gx, 0);
    image img_gy = convolve_image(im, filter_gy, 0);

    ret[0] = make_image(im.w, im.h, 1);
    ret[1] = make_image(im.w, im.h, 1);

    for (int y = 0; y < ret[0].h; ++y)
    {
        for (int x = 0; x < ret[0].w; ++x)
        {
            float value_gx = get_pixel(img_gx, x, y, 0);
            float value_gy = get_pixel(img_gy, x, y, 0);
            float magnitude = sqrtf((value_gx * value_gx) + (value_gy * value_gy));
            float gradient = atan2(value_gy, value_gx);
            set_pixel(ret[0], x, y, 0, magnitude);
            set_pixel(ret[1], x, y, 0, gradient);
        }
    }
    return ret;
}

image colorize_sobel(image im, float sigma)
{
    image *ret = sobel_image(im);
    feature_normalize(ret[0]);
    feature_normalize(ret[1]);
    image temp = make_image(im.w, im.h, 3);
    for (int c = 0; c < temp.c; c++)
    {
        for (int y = 0; y < temp.h; y++)
        {
            for (int x = 0; x < temp.w; x++)
            {
                    set_pixel(temp, x, y, c, get_pixel(ret[0], x, y , c));
            }
        }
    }
    hsv_to_rgb(temp);
    if (!sigma){
        sigma = 1;
    }
    return convolve_image(temp, make_gaussian_filter(sigma), 1);
}