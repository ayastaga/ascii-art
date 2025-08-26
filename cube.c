#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define WINDOW_WIDTH 80
#define WINDOW_HEIGHT 22
#define AREA (WINDOW_WIDTH * WINDOW_HEIGHT)
#define K1 40
#define K2 100
#define spacing 0.6
#define cubeWidth 10
#define horizontalOffset 5

float A = 0, B = 0, C = 0;
float zBuffer[AREA];
char output[AREA];

void matrixMultiplication(float m1[][3], float m2[][3], float result[1][3], int n);

void calculateForSurface(float cubeX, float cubeY, float cubeZ, int ch) {
    float vector[1][3] = {
        {cubeX, cubeY, cubeZ}
    };

    float xRotationMatrix[3][3] = {
        {1, 0, 0},
        {0, cos(A), sin(A)},
        {0, -sin(A), cos(A)}
    };

    float yRotationMatrix[3][3] = {
        {cos(B), 0, sin(B)},
        {0, 1, 0},
        {-sin(B), 0, cos(B)},
    };

    float zRotationMatrix[3][3] = {
        {cos(C), sin(C), 0},
        {-sin(C), cos(C), 0},
        {0, 0, 1}
    };

    // Rx (A) & Ry (B)
    float afterXY[3][3];
    matrixMultiplication(xRotationMatrix, yRotationMatrix, afterXY, 3);

    // Rab & Rz (C)
    float afterZ[3][3];
    matrixMultiplication(afterXY, zRotationMatrix, afterZ, 3);

    // Rabc & [i,j,k]
    float rotatedPoint[1][3];
    matrixMultiplication(vector, afterZ, rotatedPoint, 1);

    float x = rotatedPoint[0][0];
    float y = rotatedPoint[0][1];
    float z = rotatedPoint[0][2];
    float denominator = 1 / (z + K2);

    int xp = (WINDOW_WIDTH / 2 + horizontalOffset + K1 * denominator * x * 2);
    int yp = (WINDOW_HEIGHT / 2 + K1 * denominator * y);

    int index = xp + yp * WINDOW_WIDTH;
    if (index >= 0 && index < WINDOW_WIDTH * WINDOW_HEIGHT) {
    if (denominator > zBuffer[index]) {
        zBuffer[index] = denominator;
        output[index] = ch;
    }
    }
}

int main() {
  printf("\x1b[2J");
  while (1) {
    memset(output, ' ', WINDOW_WIDTH * WINDOW_HEIGHT);
    memset(zBuffer, 0, WINDOW_WIDTH * WINDOW_HEIGHT * 4);
      
    for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += spacing) {
      for (float cubeY = -cubeWidth; cubeY < cubeWidth;
           cubeY += spacing) {
        calculateForSurface(cubeX, cubeY, -cubeWidth, '@');
        calculateForSurface(cubeWidth, cubeY, cubeX, '$');
        calculateForSurface(-cubeWidth, cubeY, -cubeX, '~');
        calculateForSurface(-cubeX, cubeY, cubeWidth, '#');
        calculateForSurface(cubeX, -cubeWidth, -cubeY, ';');
        calculateForSurface(cubeX, cubeWidth, cubeY, '+');
      }
    }
    
    printf("\x1b[H");
    for (int k = 0; k < WINDOW_WIDTH * WINDOW_HEIGHT; k++) {
      putchar(k % WINDOW_WIDTH ? output[k] : 10);
    }

    A += 0.05;
    B += 0.05;
    C += 0.01;
    
    usleep(8000 * 2);
  }
  
  return 0;
}

void matrixMultiplication(float m1[][3], float m2[][3], float result[1][3], int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 3; j++) {
            result[i][j] = 0;
            for (int k = 0; k < 3; k++) {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}