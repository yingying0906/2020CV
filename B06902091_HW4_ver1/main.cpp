#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int img_rows;
int img_cols;

void binarize(Mat img, int white, int black){
    for (int i = 0; i < img_rows; i++) {
        for (int j = 0; j < img_cols; j++) {
            if (img.at<uchar>(i, j) < 128) {//0~127
                img.at<uchar>(i, j) = black;
            }
            else {
                img.at<uchar>(i, j) = white;
            }
        }
    }
}
void erosion(Mat bin, Mat ero) {
    for (int i = 2; i < img_rows-2; i++) {
        for (int j = 2; j < img_cols-2; j++) {
            int count = 0;
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
                        if (bin.at<uchar>(a, b) == 255) {
                            count++;
                        }
                    }
                }
            }
            if (count == 21) {
                ero.at<uchar>(i, j) = 255;
            }
            else {
                ero.at<uchar>(i, j) = 0;
            }
        }
    }
}

void dilation(Mat bin, Mat dil) {
    for (int i = 2; i < img_rows - 2; i++) {
        for (int j = 2; j < img_cols - 2; j++) {
            int flag = 0;
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
                        if (bin.at<uchar>(a, b) == 255) {
                            flag = 1;
                            break;
                        }
                    }
                }
                if (flag == 1) {
                    break;
                }
            }
            if (flag == 1) {
                dil.at<uchar>(i, j) = 255;
            }
            else {
                dil.at<uchar>(i, j) = 0;
            }
        }
    }
}

void ero_J(Mat bin, Mat output) {
    for (int i = 0; i < img_rows - 2; i++) {
        for (int j = 1; j < img_cols; j++) {
            int count = 0;
            for (int a = i; a < i + 2; a++) {
                for (int b = j - 1; b < j + 1; b++) {
                    if (a == i+1 && b == j - 1) {
                        continue;
                    }
                    else {
                        if (bin.at<uchar>(a, b) == 255) {
                            count++;
                        }
                    }
                }
            }
            if (count == 3) {
                output.at<uchar>(i, j) = 255;
            }
            else {
                output.at<uchar>(i, j) = 0;
            }
        }
    }
}
void ero_K(Mat bin, Mat output) {
    for (int i = 1; i < img_rows - 1; i++) {
        for (int j = 0; j < img_cols - 2; j++) {
            int count = 0;
            for (int a = i - 1; a < i + 1; a++) {
                for (int b = j; b < j + 2; b++) {
                    if (a == i && b == j) {
                        continue;
                    }
                    else {
                        if (bin.at<uchar>(a, b) == 255) {
                            count++;
                        }
                    }
                }
            }
            if (count == 3) {
                output.at<uchar>(i, j) = 255;
            }
            else {
                output.at<uchar>(i, j) = 0;
            }
        }
    }
}
int main() {
    Mat original;
    original = imread("lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    img_rows = original.rows;
    img_cols = original.cols;

    //binarize
    Mat bin = original.clone();
    binarize(bin, 255, 0);

    //(a) Dilation
    Mat dil = bin.clone();
    dilation(bin, dil);
    imwrite("a_dilation.jpg", dil);

    //(b) Erosion
    Mat ero = bin.clone();
    erosion(bin, ero);
    imwrite("b_erosion.jpg", ero);

    //Opening
    Mat open = bin.clone();
    dilation(ero, open);
    imwrite("c_open.jpg", open);

    //Closing
    Mat close = bin.clone();
    erosion(dil, close);
    imwrite("d_close.jpg", close);

    //hit and miss
    Mat J = bin.clone();          //J
    Mat bin_c = original.clone(); //A_com
    Mat K = bin_c.clone();        //K
    Mat hit(img_rows, img_cols, CV_8UC1, Scalar(0));

    binarize(bin_c, 0, 255);
    ero_J(bin, J);
    ero_K(bin_c, K);

    for (int i = 0; i < img_rows; i++) {
        for (int j = 0; j < img_cols; j++) {
            hit.at<uchar>(i, j) = (J.at<uchar>(i, j)* K.at<uchar>(i, j))/255;
        }
    }
    imwrite("e_hitandmiss.jpg", hit);

    return 0;
}
