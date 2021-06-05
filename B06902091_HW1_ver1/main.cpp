#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
int main()
{
    //Part 1
    Mat img;
    img = imread("lena.bmp", CV_LOAD_IMAGE_COLOR);
    Mat A = img.clone();
    Mat B = img.clone();
    Mat C = img.clone();

    Vec3b a, b;
    
    //1a upside
    for (int i = 0; i < img.rows/2; i++) {
        for (int j = 0; j < img.cols; j++) {
            a = A.at<Vec3b>(i, j);
            b = A.at<Vec3b>(img.rows - i - 1, j);
            A.at<Vec3b>(i, j) = b;
            A.at<Vec3b>(img.rows - i - 1, j) = a;
        }
    }
    
    //1b leftright
    for (int j = 0; j < img.cols / 2; j++) {
        for (int i = 0; i < img.rows; i++) {
            a = B.at<Vec3b>(i, j);
            b = B.at<Vec3b>(i, img.cols - j - 1);
            B.at<Vec3b>(i, j) = b;
            B.at<Vec3b>(i, img.cols - j - 1) = a;
        }
    }
    //1c mirror
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < i; j++) {
            if (i == j) {
                continue;
            }
            a = C.at<Vec3b>(i, j);
            b = C.at<Vec3b>(j, i);
            C.at<Vec3b>(i, j) = b;
            C.at<Vec3b>(j, i) = a;
        }   
    }

    //Part 2
    Mat D = img.clone();
    Mat E = img.clone();
    Mat F = img.clone();

    //2a rotate
    Point2f center(img.rows / 2, img.cols / 2);
    Mat rot = getRotationMatrix2D(center, -45, 1.0);
    warpAffine(img, D, rot, img.size());

    //2b shrink
    resize(E, E, Size(img.rows*0.5, img.cols*0.5), 0, 0, CV_INTER_LINEAR);

    //2c binarize
    threshold(F, F, 128, 255, THRESH_BINARY);

    imwrite("1a.jpg", A);
    imwrite("1b.jpg", B);
    imwrite("1c.jpg", C);
    imwrite("2a.jpg", D);
    imwrite("2b.jpg", E);
    imwrite("2c.jpg", F);
    return 0;
}
