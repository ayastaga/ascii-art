#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#define WINDOW_HEIGHT 22
#define WINDOW_WIDTH 80
#define WINDOW_AREA (WINDOW_HEIGHT * WINDOW_WIDTH)
#define theta_spacing 0.001
#define K1 15
#define K2 2.35
#define M_PI 3.14

void matrixMultiplication(float m1[1][3], float m2[3][3], float result[1][3]);

int main() {
    float A = 0, B = 0;
    char output[WINDOW_AREA];
    float zbuffer[WINDOW_AREA];    
    printf("\x1b[2J"); 
    while (1){
        memset(output, ' ', WINDOW_AREA);
        memset(zbuffer, 0, WINDOW_AREA * sizeof(float));

        float sinA = sin(A), sinB = sin(B);
        float cosA = cos(A), cosB = cos(B);
        for (float theta = 0; theta < 2*M_PI; theta += theta_spacing){
            // 3d parametric equation
            float xParam = cos(3 * theta);
            float yParam  = sin(4 * theta);
            float zParam = sin(5 * theta);

            float paramMatrix[1][3] = {
                {xParam, yParam, zParam},
            };

            float yRotationMatrix[3][3] = {
                {cosA, 0, sinA},
                {0, 1, 0},
                {-sinA, 0, cosA},
            };


            float yRotResult[1][3];
            matrixMultiplication(paramMatrix, yRotationMatrix, yRotResult);


            float x = yRotResult[0][0];
            float y = yRotResult[0][1];
            float z = yRotResult[0][2];
            float denominator = 1 / (z + K2);

            int projX = WINDOW_WIDTH / 2 + 2 * K1 * x * denominator;
            int projY = WINDOW_HEIGHT / 2 + K1 * y * denominator;
            int bufferIndex = projX + WINDOW_WIDTH * projY;

            float lightVector[3] = {0.0, 1.0, -1.0};
            float dot = 0;
            for (int i = 0; i < 3; i++) {
                dot += yRotResult[0][i] * lightVector[i];
            }
            int L = 8 * dot;

            if (projY >= 0 && projY < WINDOW_HEIGHT &&
                projX >= 0 && projX < WINDOW_WIDTH &&
                denominator > zbuffer[bufferIndex]) {
                
                zbuffer[bufferIndex] = denominator;
                output[bufferIndex] = ".,-~:;=!*#$@"[2];
            }
        }

        printf("\x1b[H");
        for (int i = 0; i < WINDOW_AREA; i++) {
            putchar(i % WINDOW_WIDTH ? output[i] : '\n');
        }
        A += 0.04; // angles for animation
        B += 0.02;

        usleep(10000);
    }

    return 0;
}

void matrixMultiplication(float m1[1][3], float m2[3][3], float result[1][3]){
    for (int i = 0; i < 1; i++){
        for (int j = 0; j < 3; j++){
            result[i][j] = 0;
            for (int k = 0; k < 3; k++){
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}