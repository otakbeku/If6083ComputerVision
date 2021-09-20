#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

#define PI 3.14159265

// Nama: Harits Abdurrohman
// NIM: 23520047

float get_pixel(image im, int x, int y, int c)
{
    // Asumsi. Jika dilihat dari load_image.c
    // matriks 3D diubah menjadi matriks 1D, sehingga
    // untuk akses ke pixelnya itu perlu mengalikan
    // nilai x, y, dan c

    if (x < 0)
    {
        x = 0;
    }
    else
    {
        x = x;
    }

    if (y < 0)
    {
        y = 0;
    }
    else
    {
        y = y;
    }

    if (c < 0)
    {
        c = 0;
    }
    else
    {
        c = c;
    }

    // Mengecek ukuran dari gambarnya
    if (x >= im.w)
    {
        x = im.w - 1;
    }
    else
    {
        x = x;
    }

    if (y >= im.h)
    {
        y = im.h - 1;
    }
    else
    {
        y = y;
    }

    if (c >= im.c)
    {
        c = im.c - 1;
    }
    else
    {
        c = c;
    }

    int index = x + (y * im.w) + (c * im.h * im.w);
    float value = im.data[index];
    return value;
}

void set_pixel(image im, int x, int y, int c, float v)
{
    if (x < 0)
    {
        x = 0;
    }
    else
    {
        x = x;
    }

    if (y < 0)
    {
        y = 0;
    }
    else
    {
        y = y;
    }

    if (c < 0)
    {
        c = 0;
    }
    else
    {
        c = c;
    }

    // Mengecek ukuran dari gambarnya
    if (x >= im.w)
    {
        x = im.w - 1;
    }
    else
    {
        x = x;
    }

    if (y >= im.h)
    {
        y = im.h - 1;
    }
    else
    {
        y = y;
    }

    if (c >= im.c)
    {
        c = im.c - 1;
    }
    else
    {
        c = c;
    }

    int index = x + (y * im.w) + (c * im.h * im.w);
    im.data[index] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    float *data_source = im.data;
    float *data_dest = copy.data;
    unsigned long values = sizeof(float) * im.w * im.h * im.c;
    memcpy(data_dest, data_source, values);
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    // somehow it works
    for (int y = 0; y < im.h; ++y)
    {
        for (int x = 0; x < im.w; ++x)
        {
            float r = get_pixel(im, x, y, 0);
            float g = get_pixel(im, x, y, 1);
            float b = get_pixel(im, x, y, 2);
            gray.data[y * im.w + x] = (r * 0.299) + (0.587 * g) + (0.114 * b);
        }
    }

    return gray;
}

void shift_image(image im, int c, float v)
{
    if (c >= im.c)
    {
        return;
    }
    if (c < 0)
    {
        return;
    }

    for (int y = 0; y < im.h; ++y)
    {
        for (int x = 0; x < im.w; ++x)
        {
            int index = x + (y * im.w) + (c * im.h * im.w);
            im.data[index] += v;
        }
    }
}

void clamp_image(image im)
{
    for (int c = 0; c < im.c; ++c)
    {
        for (int y = 0; y < im.h; ++y)
        {
            for (int x = 0; x < im.w; ++x)
            {
                int index = x + (y * im.w) + (c * im.h * im.w);
                float value = im.data[index];
                if (value <= 0.0)
                {
                    value = 0.0;
                }
                else if (value >= 1.0)
                {
                    value = 1.0;
                }
                im.data[index] = value;
            }
        }
    }
}

// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c);
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c);
}

/*
Konversi dari RGB ke HSV mengikuti rumus pada readme
*/

void rgb_to_hsv(image im)
{
    for (int y = 0; y < im.h; ++y)
    {
        for (int x = 0; x < im.w; ++x)
        {
            float R = get_pixel(im, x, y, 0);
            float G = get_pixel(im, x, y, 1);
            float B = get_pixel(im, x, y, 2);

            float V = three_way_max(R, G, B);
            float m = three_way_min(R, G, B);

            float C = V - m;
            float S = 0;
            if (V != 0)
            {
                S = C / V;
            }

            float h_hat = 0;
            if (C != 0)
            {
                if (V == R)
                {
                    h_hat = (G - B) / C;
                }
                else if (V == G)
                {
                    h_hat = (B - R) / C;
                    h_hat += 2;
                }
                else if (V == B)
                {
                    h_hat = (R - G) / C;
                    h_hat += 4;
                }
            }

            float hue = 0;
            if (h_hat < 0)
            {
                hue = h_hat / 6;
                hue += 1;
            }
            else
            {
                hue = h_hat / 6;
            }

            set_pixel(im, x, y, 0, hue);
            set_pixel(im, x, y, 1, S);
            set_pixel(im, x, y, 2, V);
        }
    }
}

// based on http://4.bp.blogspot.com/-BIcNZkeJHn0/T94CP7e0F3I/AAAAAAAAAP8/ZX7vnhdklV8/s1600/hsvtorgb.png
void hsv_to_rgb(image im)
{
    for (int col = 0; col < im.h; ++col)
    {
        for (int row = 0; row < im.w; ++row)
        {
            float h = get_pixel(im, row, col, 0);
            float S = get_pixel(im, row, col, 1);
            float V = get_pixel(im, row, col, 2);
            float chroma = S * V;
            float m = V - chroma;
            float h_temp = h * 6;
            float x = chroma * (1 - fabs(fmod(h_temp, 2.0) - 1));
            float r, g, b;
            if (h_temp <= 1)
            {
                r = chroma + m;
                g = x + m;
                b = m;
            }
            else if (h_temp <= 2)
            {
                r = x + m;
                g = chroma + m;
                b = m;
            }
            else if (h_temp <= 3)
            {
                r = m;
                g = chroma + m;
                b = x + m;
            }
            else if (h_temp <= 4)
            {
                r = m;
                g = x + m;
                b = chroma + m;
            }
            else if (h_temp <= 5)
            {
                r = x + m;
                g = m;
                b = chroma + m;
            }
            else if (h_temp <= 6)
            {
                r = chroma + m;
                g = m;
                b = x + m;
            }
            else
            {
                r = m;
                g = m;
                b = m;
            }

            set_pixel(im, row, col, 0, r);
            set_pixel(im, row, col, 1, g);
            set_pixel(im, row, col, 2, b);
        }
    }
}

void scale_image(image im, int c, float v)
{
    for (int col = 0; col < im.h; ++col)
    {
        for (int row = 0; row < im.w; ++row)
        {
            float value = get_pixel(im, row, col, c);
            value *= v;
            set_pixel(im, row, col, c, value);
        }
    }
}
/*
Konversi dari RGB ke HCL
source:
http://www.brucelindbloom.com/index.html?Eqn_RGB_to_XYZ.html
http://www.brucelindbloom.com/index.html?Eqn_XYZ_to_Luv.html
http://www.brucelindbloom.com/index.html?Eqn_Lab_to_LCH.html
*/
float inverse_L_star_companding(float V)
{
    float k = 903.3;
    float v;
    if (V <= 0.08)
    {
        v = (100 * V) / k;
    }
    else
    {
        v = pow(((V + 0.16) / 1.16), 3);
    }
    return v;
}

float L_star_companding(float V)
{
    float k = 903.3;
    float epsilon = 0.008856;
    float v;
    if (V <= epsilon)
    {
        v = (V * k) / 100;
    }
    else
    {
        v = 1.16 * pow((V - 0.16), 1 / 3);
    }
    return v;
}

void rgb_to_xyz(image im)
{
    /**
     * Implementasi menggunakan CIE RGB
     * Untuk matriks yang digunakan http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
     * 	
        0.4887180  0.3106803  0.2006017
        0.1762044  0.8129847  0.0108109
        0.0000000  0.0102048  0.9897952
     **/
    // inverse gamma companding
    float gamma = 2.2;
    for (int col = 0; col < im.h; ++col)
    {
        for (int row = 0; row < im.w; ++row)
        {
            float R = get_pixel(im, row, col, 0);
            // R = inverse_L_star_companding(R);
            R = pow(R, gamma);
            float G = get_pixel(im, row, col, 1);
            // G = inverse_L_star_companding(G);
            G = pow(G, gamma);
            float B = get_pixel(im, row, col, 2);
            // B = inverse_L_star_companding(B);
            B = pow(B, gamma);

            float X = (R * 0.4887180) + (G * 0.3106803) + (B * 0.2006017);
            float Y = (R * 0.1762044) + (G * 0.8129847) + (B * 0.0108109);
            float Z = (R * 0.0000000) + (G * 0.0102048) + (B * 0.9897952);
            set_pixel(im, row, col, 0, X);
            set_pixel(im, row, col, 1, Y);
            set_pixel(im, row, col, 2, Z);
        }
    }
}

void xyz_to_luv(image im)
/*
* source: https://ninedegreesbelow.com/photography/xyz-rgb.html#:~:text=The%20color%20white%20in%20any,the%20ICC%20profile%20reference%20white.
*/
{

    float ref_white_x = 0.4887180 + 0.3106803 + 0.2006017;
    float ref_white_y = 0.1762044 + 0.8129847 + 0.0108109;
    float ref_white_z = 0.0000000 + 0.0102048 + 0.9897952;
    float epsilon = 0.008856; // CIE standard
    float k = 903.3;          // CIE standard
    for (int col = 0; col < im.h; ++col)
    {
        for (int row = 0; row < im.w; ++row)
        {
            float X = get_pixel(im, row, col, 0);
            float Y = get_pixel(im, row, col, 1);
            float Z = get_pixel(im, row, col, 2);

            float yr = Y / ref_white_y;

            float u_hat = 4 * X / (X + (15 * Y) + (3 * Z));
            float u_hat_ref = 4 * ref_white_x / (ref_white_x + (15 * ref_white_y) + (3 * ref_white_z));

            float v_hat = 9 * X / (X + (15 * Y) + (3 * Z));
            float v_hat_ref = 9 * ref_white_x / (ref_white_x + (15 * ref_white_y) + (3 * ref_white_z));

            float L;
            if (yr > epsilon)
            {
                L = 116 * pow(yr, 1 / 3);
            }
            else
            {
                L = k * yr;
            }
            float U = 13 * L * (u_hat - u_hat_ref);
            float V = 13 * L * (v_hat - v_hat_ref);
            set_pixel(im, row, col, 0, L);
            set_pixel(im, row, col, 1, U);
            set_pixel(im, row, col, 2, V);
        }
    }
}

void luv_to_hcl(image im)
{
    float val = 180.0 / PI;
    for (int col = 0; col < im.h; ++col)
    {
        for (int row = 0; row < im.w; ++row)
        {
            float L = get_pixel(im, row, col, 0);
            float U = get_pixel(im, row, col, 1);
            float V = get_pixel(im, row, col, 2);

            float C = sqrt(pow(U, 2) + pow(V, 2));
            float H;
            // Nilai dari H ini dalam degree
            // sedangkan fungsi dari C, returnnya itu dalam bentuk radians
            H = atan2(V, U) * val;
            set_pixel(im, row, col, 0, H);
            set_pixel(im, row, col, 1, C);
            set_pixel(im, row, col, 2, L);
        }
    }
}

void rgb_to_hcl(image im)
{
    rgb_to_xyz(im);
    xyz_to_luv(im);
    luv_to_hcl(im);
}

/**
 * Bagian untuk konversi dari HCL ke RGB
 **/

void hcl_to_luv(image im)
{
    float val = PI / 180;
    for (int col = 0; col < im.h; ++col)
    {
        for (int row = 0; row < im.w; ++row)
        {
            float H = get_pixel(im, row, col, 0);
            float C = get_pixel(im, row, col, 1);
            float L = get_pixel(im, row, col, 2);

            float U = C * cos(H * val);
            float V = C * sin(H * val);

            set_pixel(im, row, col, 0, L);
            set_pixel(im, row, col, 1, U);
            set_pixel(im, row, col, 2, V);
        }
    }
}

// source: http://www.brucelindbloom.com/index.html?Eqn_Luv_to_XYZ.html
void luv_to_xyz(image im)
{
    float ref_white_x = 0.4887180 + 0.3106803 + 0.2006017;
    float ref_white_y = 0.1762044 + 0.8129847 + 0.0108109;
    float ref_white_z = 0.0000000 + 0.0102048 + 0.9897952;
    float epsilon = 0.008856; // CIE standard
    float k = 903.3;          // CIE standard
    for (int col = 0; col < im.h; ++col)
    {
        for (int row = 0; row < im.w; ++row)
        {
            float L = get_pixel(im, row, col, 0);
            float U = get_pixel(im, row, col, 1);
            float V = get_pixel(im, row, col, 2);

            float u_zero = 4 * ref_white_x / (ref_white_x + (15 * ref_white_y) + (3 * ref_white_z));

            float v_zero = 9 * ref_white_x / (ref_white_x + (15 * ref_white_y) + (3 * ref_white_z));

            float c = -1 / 3;

            float Y;
            if (L > k * epsilon)
            {
                Y = pow(((L + 16) / 116), 3);
            }
            else
            {
                Y = L / k;
            }

            float a = (1 / 3) * (((52 * L) / (U + (13 * L * u_zero))) - 1);
            float b = -5 * Y;
            float d = Y * (((39 * L) / (V + (13 * L * v_zero))) - 5);

            float X = (d - b) / (a - c);
            float Z = (X * a) + b;

            set_pixel(im, row, col, 0, X);
            set_pixel(im, row, col, 1, Y);
            set_pixel(im, row, col, 2, Z);
        }
    }
}

// source: http://www.brucelindbloom.com/index.html?Eqn_XYZ_to_RGB.html
void xyz_to_rgb(image im)
{
    /**
     * Matriks inverse 
     * 
     * 2.3706743 -0.9000405 -0.4706338
      -0.5138850  1.4253036  0.0885814
       0.0052982 -0.0146949  1.0093968
     **/
    // gamma companding
    float gamma = 2.2;
    for (int col = 0; col < im.h; ++col)
    {
        for (int row = 0; row < im.w; ++row)
        {
            float X = get_pixel(im, row, col, 0);
            float Y = get_pixel(im, row, col, 1);
            float Z = get_pixel(im, row, col, 2);

            float R = (X * 2.3706743) + (Y * -0.9000405) + (Z * -0.4706338);
            R = pow(R, 1 / gamma);
            // R = L_star_companding(R);
            float G = (X * -0.5138850) + (Y * 1.4253036) + (Z * 0.0885814);
            G = pow(G, 1 / gamma);
            // G = L_star_companding(G);
            float B = (X * 0.0052982) + (Y - 0.0146949) + (Z * 1.0093968);
            B = pow(B, 1 / gamma);
            // B = L_star_companding(B);

            set_pixel(im, row, col, 0, R);
            set_pixel(im, row, col, 1, G);
            set_pixel(im, row, col, 2, B);
        }
    }
}

void hcl_to_rgb(image im)
{
    hcl_to_luv(im);
    luv_to_xyz(im);
    xyz_to_rgb(im);
}