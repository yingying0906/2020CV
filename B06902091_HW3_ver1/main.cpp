#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

Mat histogram(Mat img) {
    //data collection
    int value[256] = { 0 };
    int max = 0;

    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            value[img.at<uchar>(i, j)] ++;
        }
    }

    //highest data
    for (int i = 0; i < 256; i++) {
        if (value[i] >= max) {
            max = value[i];
        }
    }

    //graph
    Mat graph(768, 768, CV_8UC1, Scalar(0));

    //float ratio = max / 256.0;
    float ratio = max / 768.0;

    //plot
    for (int x = 0; x < 256; x++) {//0~255
        for (int y = 0; y < value[x]; y++) {//number
            graph.at<uchar>(767 - floor(y / ratio), x * 3) = 255;
            graph.at<uchar>(767 - floor(y / ratio), x * 3 + 1) = 255;
            graph.at<uchar>(767 - floor(y / ratio), x * 3 + 2) = 255;
        }
    }
    return graph;
}
int main() {
    //(a) original image and its histogram
    Mat img_in;
    Mat a_hist;

    img_in = imread("lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);

    a_hist = histogram(img_in);

    imwrite("a_img.jpg", img_in);
    imwrite("a_hist.jpg", a_hist);

    //(b) image with intensity divided by 3 and its histogram
    Mat b;
    Mat b_hist;

    b = img_in.clone();
    for (int i = 0; i < b.rows; i++) {
        for (int j = 0; j < b.cols; j++) {
            b.at<uchar>(i, j) /= 3;
        }
    }
    b_hist = histogram(b);
    imwrite("b_img.jpg", b);
    imwrite("b_hist.jpg", b_hist);

    //(c)equalization

    Mat c;
    Mat c_hist;

    c = b.clone();
    int equal[256] = { 0 };
    int intensity[256] = { 0 };

    //data
    int total_pix = 0;
    for (int i = 0; i < c.rows; i++) {
        for (int j = 0; j < c.cols; j++) {
            intensity[c.at<uchar>(i, j)] ++;
            total_pix++;
        }
    }
    //equation
    for (int i = 0; i < 256; i++) {
        float s = 0;
        for (int j = 0; j <= i; j++) {
            s += intensity[j];
        }
        equal[i] = floor(255 * s / total_pix);
    }

    for (int i = 0; i < 256; i++) {
        //cout << i <<"   "<<equal[i] << endl;
    }

    //process
    for (int i = 0; i < c.rows; i++) {
        for (int j = 0; j < c.cols; j++) {
            c.at<uchar>(i, j) = equal[c.at<uchar>(i, j)];
        }
    }

    //histogram
    c_hist = histogram(c);
    imwrite("c_img.jpg", c);
    imwrite("c_hist.jpg", c_hist);

    cout << "done!!!!!!!!" << endl;
    return 0;
}
