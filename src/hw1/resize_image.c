#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    float value = get_pixel(im, round(x), round(y), c);
    return value;
}

image nn_resize(image im, int w, int h)
{
    /* Mengikuti slide dari kelas
    titik (0, 0)
    a*x_baru + b = interpolasi_x
    titik (w, h)
    a*(w - x_baru) + b = interpolasi_x

        a = (interpolasi_x - b)/x_baru
        b = interpolasi_x - (a*x_baru)

    karena di titik awal (-0.5, -0.5) -> (-0.5, -0.5)
    maka:
    a*-0.5 + b = -0.5
    */

    image resized_img = make_image(w, h, im.c);
    // float ax = (float)im.w/w;
    // float bx = 0.5*ax - 0.5;
    // float ay = (float)im.h/h;

    // Buat mengecek nilai input lebih besar dari image atau nggak
    float ratio_x = 0.0;
    float ratio_y = 0.0;
    if (w < im.w)
    {
        ratio_x = (float)im.w / w;
    }
    else
    {
        ratio_x = (float)w / im.w;
    }

    if (h < im.h)
    {
        ratio_y = (float)im.h / h;
    }
    else
    {
        ratio_y = (float)h / im.h;
    }

    float bx = 0.5 * ratio_x - 0.5;
    float by = 0.5 * ratio_y - 0.5;

    // printf("resize: (%d, %d) -> (%d, %d)\n", im.w, im.h, w, h);
    // printf("ratio x: %f\n", ratio_x);
    // printf("ratio y: %f\n", ratio_y);

    for (int c = 0; c < im.c; ++c)
    {
        for (int y = 0; y < h; ++y)
        {
            for (int x = 0; x < w; ++x)
            {
                float interpolasi_x = ratio_x * x + bx;
                float interpolasi_y = ratio_y * y + by;
                set_pixel(resized_img, y, x, c, nn_interpolate(im, interpolasi_x, interpolasi_y, c));
            }
        }
    }
    return resized_img;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    
    // int x1 = round(x) - 0.5;
    // int x2 = round(x) + 0.5;
    // int y1 = round(y) - 0.5;
    // int y2 = round(y) + 0.5;
    int x1 = x - 0.5;
    int x2 = x + 0.5;
    int y1 = y - 0.5;
    int y2 = y + 0.5;
    
    float V1 = get_pixel(im, x1, y1, c);
    float V2 = get_pixel(im, x2, y1, c);
    float V3 = get_pixel(im, x1, y2, c);
    float V4 = get_pixel(im, x2, y2, c);

    float d1 = x - x1;
    float d2 = x2 - x;
    float d3 = y - y1;
    float d4 = y2 - y;

    float A1 = d2*d4;
    float A2 = d1*d4;
    float A3 = d2*d3;
    float A4 = d1*d3;
    
    float q = V1*A1 + V2*A2 + V3*A3 + V4*A4;
    return q;
}

image bilinear_resize(image im, int w, int h)
{
    image resized_img = make_image(w, h, im.c);
    // float ax = (float)im.w/w;
    // float bx = 0.5*ax - 0.5;
    // float ay = (float)im.h/h;

    // Buat mengecek nilai input lebih besar dari image atau nggak
    float ratio_x = 0.0;
    float ratio_y = 0.0;
    // if (w < im.w)
    // {
    //     ratio_x = (float)im.w / w;
    // }
    // else
    // {
    //     ratio_x = (float)w / im.w;
    // }

    // if (h < im.h)
    // {
    //     ratio_y = (float)im.h / h;
    // }
    // else
    // {
    //     ratio_y = (float)h / im.h;
    // }

    float bx = 0.5 * ratio_x - 0.5;
    float by = 0.5 * ratio_y - 0.5;

    ratio_x = (float)im.w / w;
    ratio_y = (float)im.h / h;

    // printf("resize: (%d, %d) -> (%d, %d)\n", im.w, im.h, w, h);
    // printf("ratio x: %f\n", ratio_x);
    // printf("ratio y: %f\n", ratio_y);

    for (int c = 0; c < im.c; ++c)
    {
        for (int y = 0; y < h; ++y)
        {
            for (int x = 0; x < w; ++x)
            {
                float interpolasi_x = ratio_x * x + bx;
                float interpolasi_y = ratio_y * y + by;
                set_pixel(resized_img, y, x, c, nn_interpolate(im, interpolasi_x, interpolasi_y, c));
            }
        }
    }
    return resized_img;
}
