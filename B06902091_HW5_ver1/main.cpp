#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int img_rows;
int img_cols;

void erosion(Mat ori, Mat ero) {
    for (int i = 2; i < img_rows - 2; i++) {
        for (int j = 2; j < img_cols - 2; j++) {
            int minimum = 999;
            for (int a = i - 2; a < i + 3; a++) {
                for (int b = j - 2; b < j + 3; b++) {
                    if (a == i - 2 && b == j - 2) {
                        continue;
                    }
                    else if (a == i - 2 && b == j + 2) {
                        continue;
                    }
                    else if (a == i + 2 && b == j - 2) {
                        continue;
                    }
                    else if (a == i + 2 && b == j + 2) {
                        continue;
                    }
                    else {
                        if (ori.at<uchar>(a, b) <= minimum) {
                            minimum = ori.at<uchar>(a, b);
                        }
                    }
                }
            }
            ero.at<uchar>(i, j) = minimum;
        }
    }
}

void dilation(Mat ori, Mat dil) {
    for (int i = 2; i < img_rows - 2; i++) {
        for (int j = 2; j < img_cols - 2; j++) {
            int maximum = -10;
            for (int a = i - 2; a < i + 3; a++) {
                for (int b = j - 2; b < j + 3; b++) {
                    if (a == i - 2 && b == j - 2) {
                        continue;
                    }
                    else if (a == i - 2 && b == j + 2) {
                        continue;
                    }
                    else if (a == i + 2 && b == j - 2) {
                        continue;
                    }
                    else if (a == i + 2 && b == j + 2) {
                        continue;
                    }
                    else {
                        if (ori.at<uchar>(a, b) >= maximum) {
                            maximum = ori.at<uchar>(a, b);
                        }
                    }
                }
            }
            dil.at<uchar>(i, j) = maximum;
        }
    }
}

int main() {
    Mat original;
    original = imread("lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    img_rows = original.rows;
    img_cols = original.cols;

    //(a) Dilation
    Mat dil = original.clone();
    dilation(original, dil);
    imwrite("a_dilation.jpg", dil);

    //(b) Erosion
    Mat ero = original.clone();
    erosion(original, ero);
    imwrite("b_erosion.jpg", ero);

    //Opening
    Mat open = original.clone();
    dilation(ero, open);
    imwrite("c_open.jpg", open);

    //Closing
    Mat close = original.clone();
    erosion(dil, close);
    imwrite("d_close.jpg", close);

    return 0;
}
