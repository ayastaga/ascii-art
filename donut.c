#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#define WINDOW_WIDTH 80
#define WINDOW_HEIGHT 22
#define WINDOW_AREA (WINDOW_HEIGHT * WINDOW_WIDTH)
#define theta_spacing 0.07
#define phi_spacing 0.02
#define M_PI 3.14
#define r 1
#define R 2
#define K1 15
#define K2 5

int main() {
    float A = 0, B = 0;
    float zbuffer[WINDOW_AREA];
    char output[WINDOW_AREA];

    printf("\x1b[2theta");
    while (1) {
        memset(output, 32, WINDOW_AREA); // set memory
        memset(zbuffer, 0, WINDOW_AREA * 4); // set memory
        float sinA = sin(A), cosA = cos(A); // precompute A
        float sinB = sin(B), cosB = cos(B); // precompute B

        for(float theta = 0; theta < 2*M_PI; theta += theta_spacing) {
            float sintheta = sin(theta), costheta = cos(theta); // precompute THETA

            for(float phi = 0; phi < 2*M_PI; phi+= phi_spacing) {
                float sinphi = sin(phi), cosphi = cos(phi); // precompute PHI

                // circle equations
                float circlex = R + r*costheta;
                float circley = r*sintheta;
                // float circlez = 0; --> unused

                // equation of torus
                float factored_term = sinphi * circlex * cosA - circley * sinA;
                float x = cosphi * circlex * cosB - factored_term * sinB;
                float y = cosphi * circlex * sinB + factored_term * cosB;
                float z = sinphi * circlex * sinA + sintheta * cosA;
                float denominator = 1 / (z + K2);

                // projected coordinates
                int xp = 40 + 2*K1 * x * denominator;
                int yp = 12 + K1 * y * denominator;
                int o = xp + WINDOW_WIDTH * yp;
                
                // equation of normal
                int N = 8 * ((sintheta * sinA - sinphi * costheta * cosA) * cosB - sinphi * costheta * sinA - sintheta * cosA - cosphi * costheta * sinB);
                
                // checks if yp & xp is within range
                if(WINDOW_HEIGHT > yp && yp > 0 && xp > 0 && WINDOW_WIDTH > xp && denominator > zbuffer[o]) {
                    zbuffer[o] = denominator;

                    // mapping illumination to ascii characters
                    output[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
                }
            }
        }
        printf("\x1b[H");
        for(int k = 0; k < 1761; k++) {
            putchar(k % WINDOW_WIDTH ? output[k] : 10);
            // changes speed of rotation
            A += 0.00004; 
            B += 0.00002;
        }
        usleep(0); // rendering speed; original: 30000
    }
    return 0;
}