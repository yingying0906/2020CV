#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int img_rows;
int img_cols;

int matrix[64][64];

void binarize(Mat img) {
    for (int i = 0; i < img_rows; i++) {
        for (int j = 0; j < img_cols; j++) {
            if (img.at<uchar>(i, j) < 128) {//0~127
                img.at<uchar>(i, j) = 0;
            }
            else {
                img.at<uchar>(i, j) = 255;
            }
        }
    }
}

void downsample(Mat ori, Mat down) {
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            down.at<uchar>(i, j) = ori.at<uchar>(i * 8, j * 8);
        }
    }
}

char h(int b, int c, int d, int e) {
    if (b == c) {
        if (d != b || e != b) {
            return 'q';
        }
        else if (d == b && e == b) {
            return 'r';
        }
    }
    else {
        return 's';
    }
}

int f(int a1, int a2, int a3, int a4) {
    if (a1 == 'r' && a2 == 'r' && a3 == 'r' && a4 == 'r') {
        return 5;
    }
    else {
        int count = 0;
        if (a1 == 'q') {
            count++;
        }
        if (a2 == 'q') {
            count++;
        }
        if (a3 == 'q') {
            count++;
        }
        if (a4 == 'q') {
            count++;
        }
        return count;
    }
}

void yokoi(Mat down) {
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            if (down.at<uchar>(i, j) == 0) {
                matrix[i][j] = 0;
                continue;
            }

            //i. Obtain the value of x0~x8.
            int x[9] = { 0,0,0,0,0,0,0,0,0 };
            x[0] = down.at<uchar>(i, j);
            if (j != 63) {
                x[1] = down.at<uchar>(i, j + 1);
            }
            if (j != 0) {
                x[3] = down.at<uchar>(i, j - 1);
            }
            if (i != 0) {
                x[2] = down.at<uchar>(i - 1, j);
            }
            if (i != 63) {
                x[4] = down.at<uchar>(i + 1, j);
            }
            if (i != 63 && j != 63) {
                x[5] = down.at<uchar>(i + 1, j + 1);
            }
            if (i != 0 && j != 0) {
                x[7] = down.at<uchar>(i - 1, j - 1);
            }
            if (i != 0 && j != 63) {
                x[6] = down.at<uchar>(i - 1, j + 1);
            }
            if (i != 63 && j != 0) {
                x[8] = down.at<uchar>(i + 1, j - 1);
            }

            //ii. Calculate the value of a1~a4
            char a[4];
            a[0] = h(x[0], x[1], x[6], x[2]);
            a[1] = h(x[0], x[2], x[7], x[3]);
            a[2] = h(x[0], x[3], x[8], x[4]);
            a[3] = h(x[0], x[4], x[5], x[1]);

            //iii. Calculate the connectivity number
            matrix[i][j] = f(a[0], a[1], a[2], a[3]);;
        }
    }
}


int main() {
    Mat original;
    original = imread("lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    img_rows = original.rows;
    img_cols = original.cols;

    Mat down(64, 64, CV_8UC1);

    binarize(original);
    downsample(original, down);

    yokoi(down);

    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            if (matrix[i][j] == 0) {
                cout << " " << " ";
            }
            else {
                cout << matrix[i][j] << " ";
            }
        }
        cout << endl;
    }

    return 0;
}
