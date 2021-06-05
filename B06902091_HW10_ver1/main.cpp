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

void laplacian(Mat ori, Mat output, int threshold, int kernel[3][3], int constant) {
    //laplacian output
    Mat tmp(img_rows, img_cols, CV_8UC1, Scalar(255));
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
            int result = 0;
            for (int a = 0; a < 3; a++) {
                for (int b = 0; b < 3; b++) {
                    result += array[a][b] * kernel[a][b];
                }
            }
            result /= constant;

            if (result >= threshold) {
                tmp.at<uchar>(i, j) = 1;
            }
            else if (result <= (threshold * -1)) {
                tmp.at<uchar>(i, j) = 100;  //-1 -> 100
            }
            else {
                tmp.at<uchar>(i, j) = 0;
            }
        }
    }
    
    //zero crossing
    for (int i = 0; i < img_rows - 1; i++) {
        for (int j = 0; j < img_cols - 1; j++) {
            if (tmp.at<uchar>(i, j) == 100 || tmp.at<uchar>(i, j) == 0) {
                output.at<uchar>(i, j) = 255;
                continue;
            }
            int array[3][3];
            if (i == 0 && j == 0) {
                array[0][0] = tmp.at<uchar>(i, j);
                array[0][1] = tmp.at<uchar>(i, j);
                array[1][0] = tmp.at<uchar>(i, j);
                array[1][1] = tmp.at<uchar>(i, j);

                array[1][2] = tmp.at<uchar>(i, j + 1);
                array[2][1] = tmp.at<uchar>(i + 1, j);
                array[2][2] = tmp.at<uchar>(i + 1, j + 1);

                array[0][2] = tmp.at<uchar>(i, j + 1);
                array[2][0] = tmp.at<uchar>(i + 1, j);

            }
            else if (i == 0 && j != 0) {
                array[1][0] = tmp.at<uchar>(i, j - 1);
                array[1][1] = tmp.at<uchar>(i, j);
                array[1][2] = tmp.at<uchar>(i, j + 1);

                array[2][0] = tmp.at<uchar>(i + 1, j - 1);
                array[2][1] = tmp.at<uchar>(i + 1, j);
                array[2][2] = tmp.at<uchar>(i + 1, j + 1);

                array[0][0] = tmp.at<uchar>(i, j - 1);
                array[0][1] = tmp.at<uchar>(i, j);
                array[0][2] = tmp.at<uchar>(i, j + 1);

            }
            else if (j == 0 && i != 0) {
                array[0][1] = tmp.at<uchar>(i - 1, j);
                array[1][1] = tmp.at<uchar>(i, j);
                array[2][1] = tmp.at<uchar>(i + 1, j);

                array[0][2] = tmp.at<uchar>(i - 1, j + 1);
                array[1][2] = tmp.at<uchar>(i, j + 1);
                array[2][2] = tmp.at<uchar>(i + 1, j + 1);

                array[0][0] = tmp.at<uchar>(i - 1, j);
                array[1][0] = tmp.at<uchar>(i, j);
                array[2][0] = tmp.at<uchar>(i + 1, j);
            }
            else {
                array[0][0] = tmp.at<uchar>(i - 1, j - 1);
                array[0][1] = tmp.at<uchar>(i - 1, j);
                array[0][2] = tmp.at<uchar>(i - 1, j + 1);
                array[1][0] = tmp.at<uchar>(i, j - 1);
                array[1][1] = tmp.at<uchar>(i, j);
                array[1][2] = tmp.at<uchar>(i, j + 1);
                array[2][0] = tmp.at<uchar>(i + 1, j - 1);
                array[2][1] = tmp.at<uchar>(i + 1, j);
                array[2][2] = tmp.at<uchar>(i + 1, j + 1);
            }
            int flag = 0;
            for (int a = 0; a < 3; a++) {
                for (int b = 0; b < 3; b++) {
                    if (a == 1 && b == 1) {
                        continue;
                    }
                    if (array[a][b] == 100) {
                        flag = 1;
                    }
                }
            }
            if (flag == 1) {
                output.at<uchar>(i, j) = 0;
            }
            else {
                output.at<uchar>(i, j) = 255;
            }
        }
    }
}

void gaussian(Mat ori, Mat output, int threshold, int kernel[11][11], int constant) {
    //laplacian output
    Mat tmp(img_rows, img_cols, CV_8UC1, Scalar(255));

    for (int i = 5; i < img_rows - 5; i++) {
        for (int j = 5; j < img_cols - 5; j++) {
            int result = 0;
            int array[11][11];
            for (int a = 0; a < 11; a++) {
                for (int b = 0; b < 11; b++) {
                    result += ori.at<uchar>(i - 5 + a, j - 5 + b) * kernel[a][b];
                }
            }

            result /= constant;

            if (result >= threshold) {
                tmp.at<uchar>(i, j) = 1;
            }
            else if (result <= (threshold * -1)) {
                tmp.at<uchar>(i, j) = 100;  //-1 -> 100
            }
            else {
                tmp.at<uchar>(i, j) = 0;
            }
        }
    }

    //zero crossing
    for (int i = 5; i < img_rows - 5; i++) {
        for (int j = 5; j < img_cols - 5; j++) {
            if (tmp.at<uchar>(i, j) == 100 || tmp.at<uchar>(i, j) == 0) {
                output.at<uchar>(i, j) = 255;
                continue;
            }
            int flag = 0;
            for (int a = 4; a < 7; a++) {
                for (int b = 4; b < 7; b++) {
                    if (a == 5 && b == 5) {
                        continue;
                    }
                    if (tmp.at<uchar>(i - 5 + a, j - 5 + b) == 100) {
                        flag = 1;
                    }
                }
            }
            if (flag == 1) {
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

    int kernel1[3][3] = { 0,1,0,1,-4,1,0,1,0 };
    Mat laplacian_o(img_rows, img_cols, CV_8UC1, Scalar(255));
    laplacian(original, laplacian_o, 15, kernel1, 1);
    imwrite("1_laplacian_1.jpg", laplacian_o);

    int kernel2[3][3] = { 1,1,1,1,-8,1,1,1,1 };
    Mat laplacian_2o(img_rows, img_cols, CV_8UC1, Scalar(255));
    laplacian(original, laplacian_2o, 15, kernel2, 3);
    imwrite("2_laplacian_2.jpg", laplacian_2o);

    int kernel3[3][3] = { 2,-1,2,-1,-4,-1,2,-1,2 };
    Mat laplacian_3o(img_rows, img_cols, CV_8UC1, Scalar(255));
    laplacian(original, laplacian_3o, 20, kernel3, 3);
    imwrite("3_laplacian_3.jpg", laplacian_3o);

    int kernel4[11][11] = {
        0,0,0,-1,-1,-2,-1,-1,0,0,0
        ,0,0,-2,-4,-8,-9,-8,-4,-2,0,0
        ,0,-2,-7,-15,-22,-23,-22,-15,-7,-2,0
        ,-1,-4,-15,-24,-14,-1,-14,-24,-15,-4,-1
        , -1,-8,-22,-14,52,103,52,-14,-22,-8,-1
        ,-2,-9,-23,-1,103,178,103,-1,-23,-9,-2
        ,-1,-8,-22,-14,52,103,52,-14,-22,-8,-1
        ,-1,-4,-15,-24,-14,-1,-14,-24,-15,-4,-1
        ,0,-2,-7,-15,-22,-23,-22,-15,-7,-2,0
        ,0,0,-2,-4,-8,-9,-8,-4,-2,0,0
        ,0,0,0,-1,-1,-2,-1,-1,0,0,0 };

    Mat laplacian_4o(img_rows, img_cols, CV_8UC1, Scalar(255));
    gaussian(original, laplacian_4o, 3000, kernel4, 1);
    imwrite("4_laplacian_4.jpg", laplacian_4o);

    int kernel5[11][11] = {
     -1, -3,-4,-6,-7,-8,-7,-6,-4,-3,-1
    ,-3,-5,-8,-11,-13,-13,-13,-11,-8,-5,-3
    ,-4,-8,-12,-16,-17,-17,-17,-16,-12,-8,-4
    ,-6,-11,-16,-16,0,15,0,-16,-16,-11,-6
    ,-7,-13,-17,0,85,160,85,0,-17,-13,-7
    ,-8,-13,-17,15,160,283,160,15,-17,-13,-8
    ,-7,-13,-17,0,85,160,85,0,-17,-13,-7
    ,-6,-11,-16,-16,0,15,0,-16,-16,-11,-6
    ,-4,-8,-12,-16,-17,-17,-17,-16,-12,-8,-4
    ,-3,-5,-8,-11,-13,-13,-13,-11,-8,-5,-3
    ,-1,-3,-4,-6,-7,-8,-7,-6,-4,-3,-1};

    Mat laplacian_5o(img_rows, img_cols, CV_8UC1, Scalar(255));
    gaussian(original, laplacian_5o, 1, kernel5, 1);
    imwrite("5_laplacian_5.jpg", laplacian_5o);



    return 0;
}
