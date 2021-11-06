#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#include "matrix.h"
#include <time.h>

// Frees an array of descriptors.
// descriptor *d: the array.
// int n: number of elements in array.
void free_descriptors(descriptor *d, int n)
{
    int i;
    for (i = 0; i < n; ++i)
    {
        free(d[i].data);
    }
    free(d);
}

// Create a feature descriptor for an index in an image.
// image im: source image.
// int i: index in image for the pixel we want to describe.
// returns: descriptor for that index.
descriptor describe_index(image im, int i)
{
    int w = 5;
    descriptor d;
    d.p.x = i % im.w;
    d.p.y = i / im.w;
    d.data = calloc(w * w * im.c, sizeof(float));
    d.n = w * w * im.c;
    int c, dx, dy;
    int count = 0;
    // If you want you can experiment with other descriptors
    // This subtracts the central value from neighbors
    // to compensate some for exposure/lighting changes.
    for (c = 0; c < im.c; ++c)
    {
        float cval = im.data[c * im.w * im.h + i];
        for (dx = -w / 2; dx < (w + 1) / 2; ++dx)
        {
            for (dy = -w / 2; dy < (w + 1) / 2; ++dy)
            {
                float val = get_pixel(im, i % im.w + dx, i / im.w + dy, c);
                d.data[count++] = cval - val;
            }
        }
    }
    return d;
}

// Marks the spot of a point in an image.
// image im: image to mark.
// ponit p: spot to mark in the image.
void mark_spot(image im, point p)
{
    int x = p.x;
    int y = p.y;
    int i;
    for (i = -9; i < 10; ++i)
    {
        set_pixel(im, x + i, y, 0, 1);
        set_pixel(im, x, y + i, 0, 1);
        set_pixel(im, x + i, y, 1, 0);
        set_pixel(im, x, y + i, 1, 0);
        set_pixel(im, x + i, y, 2, 1);
        set_pixel(im, x, y + i, 2, 1);
    }
}

// Marks corners denoted by an array of descriptors.
// image im: image to mark.
// descriptor *d: corners in the image.
// int n: number of descriptors to mark.
void mark_corners(image im, descriptor *d, int n)
{
    int i;
    for (i = 0; i < n; ++i)
    {
        mark_spot(im, d[i].p);
    }
}

// Creates a 1d Gaussian filter.
// float sigma: standard deviation of Gaussian.
// returns: single row image of the filter.
image make_1d_gaussian(float sigma)
{
    // TODO: optional, make separable 1d Gaussian.
    // return make_image(1, 1, 1);

    int i, offset;
    int w = 6 * sigma;
    w = (w > 0) ? ((w % 2 == 1) ? w : w + 1) : 1;
    offset = w / 2;
    image gaussian_filter = make_image(1, w, 1);
    for (i = 0 - offset; i < w - offset; ++i)
    {
        float v = 1 / sqrt((TWOPI * sigma * sigma)) * exp((-i * i) / (2 * sigma * sigma));
        set_pixel(gaussian_filter, 0, i + offset, 0, v);
    }
    return gaussian_filter;
}

// Smooths an image using separable Gaussian filter.
// image im: image to smooth.
// float sigma: std dev. for Gaussian.
// returns: smoothed image.
image smooth_image(image im, float sigma)
{
    if (0)
    {
        image g = make_gaussian_filter(sigma);
        image s = convolve_image(im, g, 1);
        free_image(g);
        return s;
    }
    else
    {
        // TODO: optional, use two convolutions with 1d gaussian filter.
        // If you implement, disable the above if check.
        // return copy_image(im);
        image g = make_1d_gaussian(sigma);
        image s0 = convolve_image(im, g, 1);
        g.w = g.h;
        g.h = 1;
        image s1 = convolve_image(s0, g, 1);
        free_image(g);
        free_image(s0);
        return s1;
    }
}

// Calculate the structure matrix of an image.
// image im: the input image.
// float sigma: std dev. to use for weighted sum.
// returns: structure matrix. 1st channel is Ix^2, 2nd channel is Iy^2,
//          third channel is IxIy.
image structure_matrix(image im, float sigma)
{
    image S = make_image(im.w, im.h, 3);
    // TODO: calculate structure matrix for im.
    image gx_filter = make_gx_filter();
    image gy_filter = make_gy_filter();
    image temp = make_image(im.w, im.h, 3);

    image image_gx = convolve_image(im, gx_filter, 0);
    image image_gy = convolve_image(im, gy_filter, 0);

    for (int y = 0; y < im.h; y++)
    {
        for (int x = 0; x < im.w; x++)
        {
            float IxIx = get_pixel(image_gx, x, y, 0) * get_pixel(image_gx, x, y, 0);
            float IyIy = get_pixel(image_gy, x, y, 0) * get_pixel(image_gy, x, y, 0);
            float IxIy = get_pixel(image_gx, x, y, 0) * get_pixel(image_gy, x, y, 0);
            // Channel 0 = IxIx
            // Channel 1 = IyIy
            // Channel 2 = IxIy
            set_pixel(temp, x, y, 0, IxIx);
            set_pixel(temp, x, y, 1, IyIy);
            set_pixel(temp, x, y, 2, IxIy);
        }
    }
    S = smooth_image(temp, sigma);
    // feature_normalize(S);
    free_image(gx_filter);
    free_image(gy_filter);
    free_image(temp);
    free_image(image_gx);
    free_image(image_gy);

    return S;
}

// Estimate the cornerness of each pixel given a structure matrix S.
// image S: structure matrix for an image.
// returns: a response map of cornerness calculations.
image cornerness_response(image S)
{
    image R = make_image(S.w, S.h, 1);
    // TODO: fill in R, "cornerness" for each pixel using the structure matrix.
    // We'll use formulation det(S) - alpha * trace(S)^2, alpha = .06.
    // from http://6.869.csail.mit.edu/sp11/notes/slideNotesCh14.pdf
    // alpha range= .04 ~ .06
    float alpha = 0.06;
    float det, trace, value, v_xx, v_yy, v_xy;
    det = trace = value = 0;
    for (int y = 0; y < S.h; y++)
    {
        for (int x = 0; x < S.w; x++)
        {
            v_xx = get_pixel(S, x, y, 0);
            v_yy = get_pixel(S, x, y, 1);
            v_xy = get_pixel(S, x, y, 2);

            det = v_xx * v_yy - v_xy * v_xy;
            trace = v_xx + v_yy;
            value = det - alpha * (trace * trace);

            set_pixel(R, x, y, 0, value);
        }
    }
    return R;
}

// Perform non-max supression on an image of feature responses.
// image im: 1-channel image of feature responses.
// int w: distance to look for larger responses.
// returns: image with only local-maxima responses within w pixels.
image nms_image(image im, int w)
{
    image r = copy_image(im);
    // TODO: perform NMS on the response map.
    // for every pixel in the image:
    //     for neighbors within w:
    //         if neighbor response greater than pixel response:
    //             set response to be very low (I use -999999 [why not 0??])
    int checked = 0;
    for (int y = 0; y < im.h; y++)
    {
        for (int x = 0; x < im.w; x++)
        {
            float pixel_resp = get_pixel(im, x, y, 0);

            int end_x_within_w = w+1;

            int end_y_within_w = w+1;

            for (int start_y_within_w = -w; start_y_within_w < end_y_within_w; start_y_within_w++)
            {
                for (int start_x_within_w = -w; start_x_within_w < end_x_within_w; start_x_within_w++)
                {
                    //  check if neighbor response greater than pixel response
                    float neighbor_resp = get_pixel(im, x+start_x_within_w, y+start_y_within_w, 0);

                    if (neighbor_resp > pixel_resp)
                    {
                        set_pixel(r, x, y, 0, -999999);
                        checked = 1;
                        continue;
                        // goto this_way;
                    }
                }
                if (checked)
                {
                    // checked = 0;
                    continue;
                }
            }
            // this_way:;
        }
    }

    return r;
}

// Perform harris corner detection and extract features from the corners.
// image im: input image.
// float sigma: std. dev for harris.
// float thresh: threshold for cornerness.
// int nms: distance to look for local-maxes in response map.
// int *n: pointer to number of corners detected, should fill in.
// returns: array of descriptors of the corners in the image.
descriptor *harris_corner_detector(image im, float sigma, float thresh, int nms, int *n)
{
    // Calculate structure matrix
    image S = structure_matrix(im, sigma);

    // Estimate cornerness
    image R = cornerness_response(S);

    // Run NMS on the responses
    image Rnms = nms_image(R, nms);

    //TODO: count number of responses over threshold
    int count = 0; // change this
    for (int y = 0; y < Rnms.h; y++)
    {
        for (int x = 0; x < Rnms.w; x++)
        {
            float value = get_pixel(Rnms, x, y, 0);
            if (value > thresh)
            {
                count++;
            }
        }
    }

    *n = count; // <- set *n equal to number of corners in image.
    descriptor *d = calloc(count, sizeof(descriptor));
    //TODO: fill in array *d with descriptors of corners, use describe_index.
    int index = 0;
    for (int i = 0; i < Rnms.h * Rnms.w; i++)
    {
        float value = Rnms.data[i];
        if (value > thresh)
        {
            d[index] = describe_index(im, i);
            index++;
        }
    }

    free_image(S);
    free_image(R);
    free_image(Rnms);
    return d;
}

// Find and draw corners on an image.
// image im: input image.
// float sigma: std. dev for harris.
// float thresh: threshold for cornerness.
// int nms: distance to look for local-maxes in response map.
void detect_and_draw_corners(image im, float sigma, float thresh, int nms)
{
    int n = 0;
    descriptor *d = harris_corner_detector(im, sigma, thresh, nms, &n);
    mark_corners(im, d, n);
}