#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define PI 3.141592

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
        float cos_a = cosf(a);
        float sin_a = sinf(a);
        float cos_b = cosf(b);
        float sin_b = sinf(b);

        for (float theta = 0.0; theta < PI * 2; theta += 0.058) {
            for (float phi = 0.0; phi < PI * 2; phi += 0.017) {
                float cos_theta = cosf(theta);
                float sin_theta = sinf(theta);

                float cos_phi = cosf(phi);
                float sin_phi = sinf(phi);

                // compute donut
                float x = (r2 + r1 * cos_theta) *
                              (cos_b * cos_phi + sin_a * sin_b * sin_phi) -
                          (r1 * sin_theta) * cos_a * sin_b;
                float y = (r2 + r1 * cos_theta) *
                              (sin_b * cos_phi - sin_a * cos_b * sin_phi) +
                          (r1 * sin_theta) * cos_a * cos_b;
                float z = k2 + cos_a * (r2 + r1 * cos_theta) * sin_phi +
                          (r1 * sin_theta) * sin_a;

                float ooz = 1 / z;

                // project donut onto 2D screen
                int x_p = width / 2 + k1 * ooz * x;
                int y_p = height / 2 - k1 * ooz * y;

                // determine illumination by calculating surface normal
                float luminance =
                    cos_phi * cos_theta * sin_b - cos_a * cos_theta * sin_phi -
                    sin_a * sin_theta +
                    cos_b * (cos_a * sin_theta - cos_theta * sin_a * sin_phi);

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