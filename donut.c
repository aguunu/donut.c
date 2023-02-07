#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define PI 3.141592

void x_rotation(float *x, float *y, float *z, float alpha) {
    float y1 = *y;
    float z1 = *z;

    *y = y1 * cosf(alpha) - z1 * sinf(alpha);
    *z = y1 * sinf(alpha) + z1 * cosf(alpha);
}

void y_rotation(float *x, float *y, float *z, float alpha) {
    float x1 = *x;
    float z1 = *z;

    *x = x1 * cosf(alpha) + z1 * sinf(alpha);
    *z = -x1 * sinf(alpha) + z1 * cosf(alpha);
}

void z_rotation(float *x, float *y, float *z, float alpha) {
    float x1 = *x;
    float y1 = *y;

    *x = x1 * cosf(alpha) - y1 * sinf(alpha);
    *y = x1 * sinf(alpha) + y1 * cosf(alpha);
}

int main() {
    int width = 32;
    int height = 32;
    char c_buffer[width * height];
    float z_buffer[width * height];

    float r1 = 1.0;
    float r2 = 2.0;
    float k2 = 4.0;
    float k1 = width * k2 / (4 * (r1 + r2));
    float a = 0.0;
    float b = 0.0;

    while (1) {
        memset(z_buffer, 0, 4 * width * height);
        memset(c_buffer, 32, width * height);

        a += 0.04;
        b += 0.03;

        for (float theta = 0.0; theta < PI * 2; theta += 0.058) {
            for (float phi = 0.0; phi < PI * 2; phi += 0.017) {
                // create circle of radius R1 centered in R2
                float x = r2 + r1 * cosf(theta);
                float y = r1 * sinf(theta);
                float z = 0;

                // create the donut by rotating around Y axis
                y_rotation(&x, &y, &z, phi);

                // spin the donut around the X and Z axes
                x_rotation(&x, &y, &z, a);
                z_rotation(&x, &y, &z, b);

                z += k2;

                float ooz = 1 / z;

                // project donut onto 2D screen
                int x_p = width / 2 + k1 * ooz * x;
                int y_p = height / 2 - k1 * ooz * y;

                // determine illumination by calculating surface normal
                float n1 = cosf(theta);
                float n2 = sinf(theta);
                float n3 = 0;

                y_rotation(&n1, &n2, &n3, phi);
                x_rotation(&n1, &n2, &n3, a);
                z_rotation(&n1, &n2, &n3, b);

                float luminance = n2 - n3;

                int buffer_index = x_p + width * y_p;

                if (luminance < 0 || z_buffer[buffer_index] > ooz) {
                    continue;
                }

                z_buffer[buffer_index] = ooz;
                int l_index = luminance * 8;
                c_buffer[buffer_index] = ".,-~:;=!*#$@"[l_index];
            }
        }

        printf("\e[H\e[2J\e[3J");

        for (int i = 0; i < width * height; i++) {
            putchar(i % width ? c_buffer[i] : 10);
        }

        usleep(16667);
    }
}