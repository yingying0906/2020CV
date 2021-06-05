#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <algorithm>
#include <vector>

using namespace cv;
using namespace std;

int img_rows;
int img_cols;

void roberts(Mat ori, Mat output, int threshold) {
    for (int i = 0; i < img_rows-1; i++) {
        for (int j = 0; j < img_cols-1; j++) {
            int r1 = ori.at<uchar>(i + 1, j + 1) - ori.at<uchar>(i, j);
            int r2 = ori.at<uchar>(i + 1, j) - ori.at<uchar>(i, j + 1);
            int gradient = sqrt(r1 * r1 + r2 * r2);
            if (gradient >= threshold) {
                output.at<uchar>(i, j) = 0;
            }
            else {
                output.at<uchar>(i, j) = 255;
            }
        }
    }
}

void prewitt(Mat ori, Mat output, int threshold) {
    for (int i = 0; i < img_rows - 1; i++) {
        for (int j = 0; j < img_cols - 1; j++) {
            int array[3][3];
            if (i == 0 && j == 0) {
                array[0][0] = ori.at<uchar>(i, j);
                array[0][1] = ori.at<uchar>(i, j);
                array[1][0] = ori.at<uchar>(i, j);
                array[1][1] = ori.at<uchar>(i, j);

                array[1][2] = ori.at<uchar>(i, j+1);
                array[2][1] = ori.at<uchar>(i+1, j);
                array[2][2] = ori.at<uchar>(i+1, j+1);

                array[0][2] = ori.at<uchar>(i, j+1);
                array[2][0] = ori.at<uchar>(i+1, j);

            }
            else if (i == 0 && j != 0) {
                array[1][0] = ori.at<uchar>(i, j-1);
                array[1][1] = ori.at<uchar>(i, j);
                array[1][2] = ori.at<uchar>(i, j+1);

                array[2][0] = ori.at<uchar>(i+1, j - 1);
                array[2][1] = ori.at<uchar>(i+1, j);
                array[2][2] = ori.at<uchar>(i+1, j + 1);

                array[0][0] = ori.at<uchar>(i, j - 1);
                array[0][1] = ori.at<uchar>(i, j);
                array[0][2] = ori.at<uchar>(i, j + 1);

            }
            else if (j == 0 && i != 0) {
                array[0][1] = ori.at<uchar>(i - 1, j);
                array[1][1] = ori.at<uchar>(i, j);
                array[2][1] = ori.at<uchar>(i + 1, j);

                array[0][2] = ori.at<uchar>(i - 1, j+1);
                array[1][2] = ori.at<uchar>(i, j+1);
                array[2][2] = ori.at<uchar>(i + 1, j+1);

                array[0][0] = ori.at<uchar>(i - 1, j);
                array[1][0] = ori.at<uchar>(i, j);
                array[2][0] = ori.at<uchar>(i + 1, j);
            }
            else {
                array[0][0] = ori.at<uchar>(i - 1, j - 1);
                array[0][1] = ori.at<uchar>(i - 1, j);
                array[0][2] = ori.at<uchar>(i - 1, j + 1);
                array[1][0] = ori.at<uchar>(i, j - 1);
                array[1][1] = ori.at<uchar>(i, j);
                array[1][2] = ori.at<uchar>(i, j + 1);
                array[2][0] = ori.at<uchar>(i + 1, j - 1);
                array[2][1] = ori.at<uchar>(i + 1, j);
                array[2][2] = ori.at<uchar>(i + 1, j + 1);
            }
            int p1 = (array[0][0] + array[0][1] + array[0][2]) * -1 + array[2][0] + array[2][1] + array[2][2];
            int p2 = (array[0][0] + array[1][0] + array[2][0]) * -1 + array[0][2] + array[1][2] + array[2][2];
            int gradient = sqrt(p1 * p1 + p2 * p2);
            if (gradient >= threshold) {
                output.at<uchar>(i, j) = 0;
            }
            else {
                output.at<uchar>(i, j) = 255;
            }
        }
    }
}

void sobel(Mat ori, Mat output, int threshold) {
    for (int i = 0; i < img_rows - 1; i++) {
        for (int j = 0; j < img_cols - 1; j++) {
            int array[3][3];
            if (i == 0 && j == 0) {
                array[0][0] = ori.at<uchar>(i, j);
                array[0][1] = ori.at<uchar>(i, j);
                array[1][0] = ori.at<uchar>(i, j);
                array[1][1] = ori.at<uchar>(i, j);

                array[1][2] = ori.at<uchar>(i, j + 1);
                array[2][1] = ori.at<uchar>(i + 1, j);
                array[2][2] = ori.at<uchar>(i + 1, j + 1);

                array[0][2] = ori.at<uchar>(i, j + 1);
                array[2][0] = ori.at<uchar>(i + 1, j);

            }
            else if (i == 0 && j != 0) {
                array[1][0] = ori.at<uchar>(i, j - 1);
                array[1][1] = ori.at<uchar>(i, j);
                array[1][2] = ori.at<uchar>(i, j + 1);

                array[2][0] = ori.at<uchar>(i + 1, j - 1);
                array[2][1] = ori.at<uchar>(i + 1, j);
                array[2][2] = ori.at<uchar>(i + 1, j + 1);

                array[0][0] = ori.at<uchar>(i, j - 1);
                array[0][1] = ori.at<uchar>(i, j);
                array[0][2] = ori.at<uchar>(i, j + 1);

            }
            else if (j == 0 && i != 0) {
                array[0][1] = ori.at<uchar>(i - 1, j);
                array[1][1] = ori.at<uchar>(i, j);
                array[2][1] = ori.at<uchar>(i + 1, j);

                array[0][2] = ori.at<uchar>(i - 1, j + 1);
                array[1][2] = ori.at<uchar>(i, j + 1);
                array[2][2] = ori.at<uchar>(i + 1, j + 1);

                array[0][0] = ori.at<uchar>(i - 1, j);
                array[1][0] = ori.at<uchar>(i, j);
                array[2][0] = ori.at<uchar>(i + 1, j);
            }
            else {
                array[0][0] = ori.at<uchar>(i - 1, j - 1);
                array[0][1] = ori.at<uchar>(i - 1, j);
                array[0][2] = ori.at<uchar>(i - 1, j + 1);
                array[1][0] = ori.at<uchar>(i, j - 1);
                array[1][1] = ori.at<uchar>(i, j);
                array[1][2] = ori.at<uchar>(i, j + 1);
                array[2][0] = ori.at<uchar>(i + 1, j - 1);
                array[2][1] = ori.at<uchar>(i + 1, j);
                array[2][2] = ori.at<uchar>(i + 1, j + 1);
            }
            int s1 = - array[0][0] - (array[0][1]*2) - array[0][2] + array[2][0] + (array[2][1]*2) + array[2][2];
            int s2 = - array[0][0] - (array[1][0]*2) - array[2][0] + array[0][2] + (array[1][2]*2) + array[2][2];
            int gradient = sqrt(s1 * s1 + s2 * s2);
            if (gradient >= threshold) {
                output.at<uchar>(i, j) = 0;
            }
            else {
                output.at<uchar>(i, j) = 255;
            }
        }
    }
}

void frei(Mat ori, Mat output, int threshold) {
    for (int i = 0; i < img_rows - 1; i++) {
        for (int j = 0; j < img_cols - 1; j++) {
            int array[3][3];
            if (i == 0 && j == 0) {
                array[0][0] = ori.at<uchar>(i, j);
                array[0][1] = ori.at<uchar>(i, j);
                array[1][0] = ori.at<uchar>(i, j);
                array[1][1] = ori.at<uchar>(i, j);

                array[1][2] = ori.at<uchar>(i, j + 1);
                array[2][1] = ori.at<uchar>(i + 1, j);
                array[2][2] = ori.at<uchar>(i + 1, j + 1);

                array[0][2] = ori.at<uchar>(i, j + 1);
                array[2][0] = ori.at<uchar>(i + 1, j);

            }
            else if (i == 0 && j != 0) {
                array[1][0] = ori.at<uchar>(i, j - 1);
                array[1][1] = ori.at<uchar>(i, j);
                array[1][2] = ori.at<uchar>(i, j + 1);

                array[2][0] = ori.at<uchar>(i + 1, j - 1);
                array[2][1] = ori.at<uchar>(i + 1, j);
                array[2][2] = ori.at<uchar>(i + 1, j + 1);

                array[0][0] = ori.at<uchar>(i, j - 1);
                array[0][1] = ori.at<uchar>(i, j);
                array[0][2] = ori.at<uchar>(i, j + 1);

            }
            else if (j == 0 && i != 0) {
                array[0][1] = ori.at<uchar>(i - 1, j);
                array[1][1] = ori.at<uchar>(i, j);
                array[2][1] = ori.at<uchar>(i + 1, j);

                array[0][2] = ori.at<uchar>(i - 1, j + 1);
                array[1][2] = ori.at<uchar>(i, j + 1);
                array[2][2] = ori.at<uchar>(i + 1, j + 1);

                array[0][0] = ori.at<uchar>(i - 1, j);
                array[1][0] = ori.at<uchar>(i, j);
                array[2][0] = ori.at<uchar>(i + 1, j);
            }
            else {
                array[0][0] = ori.at<uchar>(i - 1, j - 1);
                array[0][1] = ori.at<uchar>(i - 1, j);
                array[0][2] = ori.at<uchar>(i - 1, j + 1);
                array[1][0] = ori.at<uchar>(i, j - 1);
                array[1][1] = ori.at<uchar>(i, j);
                array[1][2] = ori.at<uchar>(i, j + 1);
                array[2][0] = ori.at<uchar>(i + 1, j - 1);
                array[2][1] = ori.at<uchar>(i + 1, j);
                array[2][2] = ori.at<uchar>(i + 1, j + 1);
            }
            int f1 = -array[0][0] - (array[0][1] * sqrt(2)) - array[0][2] + array[2][0] + (array[2][1] * sqrt(2)) + array[2][2];
            int f2 = -array[0][0] - (array[1][0] * sqrt(2)) - array[2][0] + array[0][2] + (array[1][2] * sqrt(2)) + array[2][2];
            int gradient = sqrt(f1 * f1 + f2 * f2);
            if (gradient >= threshold) {
                output.at<uchar>(i, j) = 0;
            }
            else {
                output.at<uchar>(i, j) = 255;
            }
        }
    }
}

void kirsch(Mat ori, Mat output, int threshold) {
    for (int i = 0; i < img_rows - 1; i++) {
        for (int j = 0; j < img_cols - 1; j++) {
            int array[3][3];
            if (i == 0 && j == 0) {
                array[0][0] = ori.at<uchar>(i, j);
                array[0][1] = ori.at<uchar>(i, j);
                array[1][0] = ori.at<uchar>(i, j);
                array[1][1] = ori.at<uchar>(i, j);

                array[1][2] = ori.at<uchar>(i, j + 1);
                array[2][1] = ori.at<uchar>(i + 1, j);
                array[2][2] = ori.at<uchar>(i + 1, j + 1);

                array[0][2] = ori.at<uchar>(i, j + 1);
                array[2][0] = ori.at<uchar>(i + 1, j);

            }
            else if (i == 0 && j != 0) {
                array[1][0] = ori.at<uchar>(i, j - 1);
                array[1][1] = ori.at<uchar>(i, j);
                array[1][2] = ori.at<uchar>(i, j + 1);

                array[2][0] = ori.at<uchar>(i + 1, j - 1);
                array[2][1] = ori.at<uchar>(i + 1, j);
                array[2][2] = ori.at<uchar>(i + 1, j + 1);

                array[0][0] = ori.at<uchar>(i, j - 1);
                array[0][1] = ori.at<uchar>(i, j);
                array[0][2] = ori.at<uchar>(i, j + 1);

            }
            else if (j == 0 && i != 0) {
                array[0][1] = ori.at<uchar>(i - 1, j);
                array[1][1] = ori.at<uchar>(i, j);
                array[2][1] = ori.at<uchar>(i + 1, j);

                array[0][2] = ori.at<uchar>(i - 1, j + 1);
                array[1][2] = ori.at<uchar>(i, j + 1);
                array[2][2] = ori.at<uchar>(i + 1, j + 1);

                array[0][0] = ori.at<uchar>(i - 1, j);
                array[1][0] = ori.at<uchar>(i, j);
                array[2][0] = ori.at<uchar>(i + 1, j);
            }
            else {
                array[0][0] = ori.at<uchar>(i - 1, j - 1);
                array[0][1] = ori.at<uchar>(i - 1, j);
                array[0][2] = ori.at<uchar>(i - 1, j + 1);
                array[1][0] = ori.at<uchar>(i, j - 1);
                array[1][1] = ori.at<uchar>(i, j);
                array[1][2] = ori.at<uchar>(i, j + 1);
                array[2][0] = ori.at<uchar>(i + 1, j - 1);
                array[2][1] = ori.at<uchar>(i + 1, j);
                array[2][2] = ori.at<uchar>(i + 1, j + 1);
            }
            int k[8];
            //array[][]
            k[0] = (-3) * (array[0][0] + array[0][1] + array[1][0] + array[2][0] + array[2][1]) + (5) * (array[0][2] + array[1][2] + array[2][2]);
            k[1] = (-3) * (array[0][0] + array[1][0] + array[2][0] + array[2][1] + array[2][2]) + (5) * (array[0][1] + array[0][2] + array[1][2]);
            k[2] = (-3) * (array[1][0] + array[1][2] + array[2][0] + array[2][1] + array[2][2]) + (5) * (array[0][0] + array[0][1] + array[0][2]);
            k[3] = (-3) * (array[2][0] + array[2][1] + array[2][2] + array[1][2] + array[0][2]) + (5) * (array[0][0] + array[0][1] + array[1][0]);
            k[4] = (-3) * (array[0][1] + array[0][2] + array[2][1] + array[2][2] + array[1][2]) + (5) * (array[0][0] + array[1][0] + array[2][0]);
            k[5] = (-3) * (array[0][0] + array[0][1] + array[0][2] + array[1][2] + array[2][2]) + (5) * (array[1][0] + array[2][0] + array[2][1]);
            k[6] = (-3) * (array[0][0] + array[0][1] + array[0][2] + array[1][0] + array[1][2]) + (5) * (array[2][0] + array[2][1] + array[2][2]);
            k[7] = (-3) * (array[0][0] + array[0][1] + array[0][2] + array[1][0] + array[2][0]) + (5) * (array[2][1] + array[2][2] + array[1][2]);


            int gradient = -1;
            for (int a = 0; a < 8; a++) {
                if (k[a] > gradient) {
                    gradient = k[a];
                }
            }
            if (gradient >= threshold) {
                output.at<uchar>(i, j) = 0;
            }
            else {
                output.at<uchar>(i, j) = 255;
            }
        }
    }
}

void robinson(Mat ori, Mat output, int threshold) {
    for (int i = 0; i < img_rows - 1; i++) {
        for (int j = 0; j < img_cols - 1; j++) {
            int array[3][3];
            if (i == 0 && j == 0) {
                array[0][0] = ori.at<uchar>(i, j);
                array[0][1] = ori.at<uchar>(i, j);
                array[1][0] = ori.at<uchar>(i, j);
                array[1][1] = ori.at<uchar>(i, j);

                array[1][2] = ori.at<uchar>(i, j + 1);
                array[2][1] = ori.at<uchar>(i + 1, j);
                array[2][2] = ori.at<uchar>(i + 1, j + 1);

                array[0][2] = ori.at<uchar>(i, j + 1);
                array[2][0] = ori.at<uchar>(i + 1, j);

            }
            else if (i == 0 && j != 0) {
                array[1][0] = ori.at<uchar>(i, j - 1);
                array[1][1] = ori.at<uchar>(i, j);
                array[1][2] = ori.at<uchar>(i, j + 1);

                array[2][0] = ori.at<uchar>(i + 1, j - 1);
                array[2][1] = ori.at<uchar>(i + 1, j);
                array[2][2] = ori.at<uchar>(i + 1, j + 1);

                array[0][0] = ori.at<uchar>(i, j - 1);
                array[0][1] = ori.at<uchar>(i, j);
                array[0][2] = ori.at<uchar>(i, j + 1);

            }
            else if (j == 0 && i != 0) {
                array[0][1] = ori.at<uchar>(i - 1, j);
                array[1][1] = ori.at<uchar>(i, j);
                array[2][1] = ori.at<uchar>(i + 1, j);

                array[0][2] = ori.at<uchar>(i - 1, j + 1);
                array[1][2] = ori.at<uchar>(i, j + 1);
                array[2][2] = ori.at<uchar>(i + 1, j + 1);

                array[0][0] = ori.at<uchar>(i - 1, j);
                array[1][0] = ori.at<uchar>(i, j);
                array[2][0] = ori.at<uchar>(i + 1, j);
            }
            else {
                array[0][0] = ori.at<uchar>(i - 1, j - 1);
                array[0][1] = ori.at<uchar>(i - 1, j);
                array[0][2] = ori.at<uchar>(i - 1, j + 1);
                array[1][0] = ori.at<uchar>(i, j - 1);
                array[1][1] = ori.at<uchar>(i, j);
                array[1][2] = ori.at<uchar>(i, j + 1);
                array[2][0] = ori.at<uchar>(i + 1, j - 1);
                array[2][1] = ori.at<uchar>(i + 1, j);
                array[2][2] = ori.at<uchar>(i + 1, j + 1);
            }
            int r[8];

            r[0] = -(array[0][0]) - (array[1][0] *2) - (array[2][0]) + (array[0][2]) + (array[1][2] *2) + (array[2][2]);
            r[1] = -(array[1][0]) - (array[2][0] * 2) - (array[2][1]) + (array[0][1]) + (array[0][2] * 2) + (array[1][2]);
            r[2] = -(array[2][0]) - (array[2][1] * 2) - (array[2][2]) + (array[0][0]) + (array[0][1] * 2) + (array[0][2]);
            r[3] = -(array[2][1]) - (array[2][2] * 2) - (array[1][2]) + (array[1][0]) + (array[0][0] * 2) + (array[0][1]);
            r[4] = -(array[0][2]) - (array[1][2] * 2) - (array[2][2]) + (array[0][0]) + (array[1][0] * 2) + (array[2][0]);
            r[5] = -(array[0][1]) - (array[0][2] * 2) - (array[1][2]) + (array[1][0]) + (array[2][0] * 2) + (array[2][1]);
            r[6] = -(array[0][0]) - (array[0][1] * 2) - (array[0][2]) + (array[2][0]) + (array[2][1] * 2) + (array[2][2]);
            r[7] = -(array[1][0]) - (array[0][0] * 2) - (array[0][1]) + (array[2][1]) + (array[2][2] * 2) + (array[1][2]);

            int gradient = -1;
            for (int a = 0; a < 8; a++) {
                if (r[a] > gradient) {
                    gradient = r[a];
                }
            }
            if (gradient >= threshold) {
                output.at<uchar>(i, j) = 0;
            }
            else {
                output.at<uchar>(i, j) = 255;
            }
        }
    }
}

void nevatia(Mat ori, Mat output, int threshold) {
    for (int i = 2; i < img_rows - 2; i++) {
        for (int j = 2; j < img_cols - 2; j++) {
            int array[5][5];
            for (int a = 0; a < 5; a++) {
                for (int b = 0; b < 5; b++) {
                    array[a][b] = ori.at<uchar>(i - 2 + a, j - 2 + b);
                }
            }

            int n0[5][5] = { {100,100,100,100,100}
                            ,{100,100,100,100,100}
                            ,{0,0,0,0,0}
                            ,{-100,-100,-100,-100,-100}
                            ,{-100,-100,-100,-100,-100} };
            int n1[5][5] = { {100,100,100,100,100}
                            ,{100,100,100,78,-32}
                            ,{100,92,0,-92,-100}
                            ,{32,-78,-100,-100,-100}
                            ,{-100,-100,-100,-100,-100} };
            int n2[5][5] = { {100,100,100,32,-100}
                            ,{100,100,92,-78,-100}
                            ,{100,100,0,-100,-100}
                            ,{100,78,-92,-100,-100}
                            ,{100,-32,-100,-100,-100} };
            int n3[5][5] = { {-100,-100,0,100,100}
                            ,{-100,-100,0,100,100}
                            ,{-100,-100,0,100,100}
                            ,{-100,-100,0,100,100}
                            ,{-100,-100,0,100,100} };
            int n4[5][5] = { {-100,32,100,100,100}
                            ,{-100,-78,92,100,100}
                            ,{-100,-100,0,100,100}
                            ,{-100,-100,-92,78,100}
                            ,{-100,-100,-100,-32,100} };
            int n5[5][5] = { {100,100,100,100,100}
                            ,{-32,78,100,100,100}
                            ,{-100,-92,0,92,100}
                            ,{-100,-100,-100,-78,32}
                            ,{-100,-100,-100,-100,-100} };

            int result[6] = {0,0,0,0,0,0};
            for (int x = 0; x < 5; x++) {
                for (int y = 0; y < 5; y++) {
                    result[0] += array[x][y] * n0[x][y];
                }
            }
            for (int x = 0; x < 5; x++) {
                for (int y = 0; y < 5; y++) {
                    result[1] += array[x][y] * n1[x][y];
                }
            }
            for (int x = 0; x < 5; x++) {
                for (int y = 0; y < 5; y++) {
                    result[2] += array[x][y] * n2[x][y];
                }
            }
            for (int x = 0; x < 5; x++) {
                for (int y = 0; y < 5; y++) {
                    result[3] += array[x][y] * n3[x][y];
                }
            }
            for (int x = 0; x < 5; x++) {
                for (int y = 0; y < 5; y++) {
                    result[4] += array[x][y] * n4[x][y];
                }
            }
            for (int x = 0; x < 5; x++) {
                for (int y = 0; y < 5; y++) {
                    result[5] += array[x][y] * n5[x][y];
                }
            }

            int gradient = -1;
            for (int a = 0; a < 6; a++) {
                if (result[a] > gradient) {
                    gradient = result[a];
                }
            }
            if (gradient >= threshold) {
                output.at<uchar>(i, j) = 0;
            }
            else {
                output.at<uchar>(i, j) = 255;
            }
        }
    }
}

int main() {
    Mat original;
    original = imread("lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    img_rows = original.rows;
    img_cols = original.cols;

    Mat robert_o(img_rows, img_cols, CV_8UC1, Scalar(255));
    roberts(original, robert_o, 12);
    imwrite("1_roberts.jpg", robert_o);

    Mat prewitt_o(img_rows, img_cols, CV_8UC1, Scalar(255));
    prewitt(original, prewitt_o, 24);
    imwrite("2_prewitt.jpg", prewitt_o);

    Mat sobel_o(img_rows, img_cols, CV_8UC1, Scalar(255));
    sobel(original, sobel_o, 38);
    imwrite("3_sobel.jpg", sobel_o);

    Mat frei_o(img_rows, img_cols, CV_8UC1, Scalar(255));
    frei(original, frei_o, 30);
    imwrite("4_frei.jpg", frei_o);

    Mat kirsch_o(img_rows, img_cols, CV_8UC1, Scalar(255));
    kirsch(original, kirsch_o, 135);
    imwrite("5_kirsch.jpg", kirsch_o);

    Mat robinson_o(img_rows, img_cols, CV_8UC1, Scalar(255));
    robinson(original, robinson_o, 43);
    imwrite("6_robinson.jpg", robinson_o);

    Mat nevatia_o(img_rows, img_cols, CV_8UC1, Scalar(255));
    nevatia(original, nevatia_o, 12500);
    imwrite("7_nevatia.jpg", nevatia_o);

    return 0;
}
