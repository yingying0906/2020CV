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

void snr_func(double vs, Mat ori, Mat next) {
    double n = (double)img_rows * (double)img_cols;
    double sum_new = 0;
    for (int i = 0; i < img_rows; i++) {
        for (int j = 0; j < img_cols; j++) {
            sum_new += (next.at<uchar>(i, j) - ori.at<uchar>(i, j));
        }
    }
    double mean_new = sum_new / n;

    double vn_sum = 0;
    for (int i = 0; i < img_rows; i++) {
        for (int j = 0; j < img_cols; j++) {
            double temp = next.at<uchar>(i, j) - ori.at<uchar>(i, j) - mean_new;
            vn_sum += temp * temp;
        }
    }
    double vn = vn_sum / n;
    double rootvs = sqrt(vs);
    double rootvn = sqrt(vn);
    double snr = 20 * log10(rootvs / rootvn);
    cout << "SNR = " << snr << endl;

}

void gaussian_noise(Mat output, int amplitude) {
    RNG rng;
    for (int i = 0; i < img_rows; i++) {
        for (int j = 0; j < img_cols; j++) {
            double rnd = rng.gaussian(1);
            output.at<uchar>(i, j) = output.at<uchar>(i, j) + amplitude * rnd;
        }
    }
}

void salt_pepper(Mat output, double threshold) {
    RNG rng;
    for (int i = 0; i < img_rows; i++) {
        for (int j = 0; j < img_cols; j++) {
            double rnd = rng.uniform((double)0,(double)1);
            if (rnd < threshold) {
                output.at<uchar>(i, j) = 0;
            }
            else if (rnd > (1 - threshold)) {
                output.at<uchar>(i, j) = 255;
            }
        }
    }
}

void box_filter_3(Mat output) {
    for (int i = 0; i < img_rows; i++) {
        for (int j = 0; j < img_cols; j++) {
            int sum = 0;
            int count = 0;
            for (int a = -1; a <= 1; a++) {
                for (int b = -1; b <= 1; b++) {
                    int x = i + a;
                    int y = j + b;
                    if (x < 0 || x >= img_rows || y < 0 || y >= img_cols) {
                        continue;
                    }
                    else {
                        sum += output.at<uchar>(i + a, j + b);
                        count++;
                    }
                    
                }
            }
            output.at<uchar>(i, j) = sum/count;
        }
    }
}

void box_filter_5(Mat output) {
    for (int i = 0; i < img_rows; i++) {
        for (int j = 0; j < img_cols; j++) {
            int sum = 0;
            int count = 0;
            for (int a = -2; a <= 2; a++) {
                for (int b = -2; b <= 2; b++) {
                    int x = i + a;
                    int y = j + b;
                    if (x < 0 || x >= img_rows || y < 0 || y >= img_cols) {
                        continue;
                    }
                    else {
                        sum += output.at<uchar>(i + a, j + b);
                        count++;
                    }
                }
            }
            output.at<uchar>(i, j) = sum / count;
        }
    }
}

void median_filter_3(Mat output) {
    for (int i = 0; i < img_rows; i++) {
        for (int j = 0; j < img_cols; j++) {
            vector <int> temp;
            int count = 0;
            for (int a = -1; a <= 1; a++) {
                for (int b = -1; b <= 1; b++) {
                    int x = i + a;
                    int y = j + b;
                    if (x < 0 || x >= img_rows || y < 0 || y >= img_cols) {
                        continue;
                    }
                    else {
                        temp.push_back(output.at<uchar>(i + a, j + b));
                        count++;
                    }
                }
            }
            int mid = count / 2;
            sort(temp.begin(), temp.end());
            output.at<uchar>(i, j) = temp.at(mid);
        }
    }
}

void median_filter_5(Mat output) {
    for (int i = 0; i < img_rows; i++) {
        for (int j = 0; j < img_cols; j++) {
            vector <int> temp;
            int count = 0;
            for (int a = -2; a <= 2; a++) {
                for (int b = -2; b <= 2; b++) {
                    int x = i + a;
                    int y = j + b;
                    if (x < 0 || x >= img_rows || y < 0 || y >= img_cols) {
                        continue;
                    }
                    else {
                        temp.push_back(output.at<uchar>(i + a, j + b));
                        count++;
                    }
                }
            }
            int mid = count / 2;
            sort(temp.begin(), temp.end());
            output.at<uchar>(i, j) = temp.at(mid);
        }
    }
}

void erosion(Mat ori, Mat ero) {
    for (int i = 0; i < img_rows; i++) {
        for (int j = 0; j < img_cols; j++) {
            int minimum = 999;
            for (int a = -2; a <= 2; a++) {
                for (int b = -2; b <= 2; b++) {
                    int x = i + a;
                    int y = j + b;
                    if (x < 0 || x >= img_rows || y < 0 || y >= img_cols) {
                        continue;
                    }
                    else {
                        if (ori.at<uchar>(x, y) <= minimum) {
                            minimum = ori.at<uchar>(x, y);
                        }
                    }
                }
            }
            ero.at<uchar>(i, j) = minimum;
        }
    }
}

void dilation(Mat ori, Mat dil) {
    for (int i = 0; i < img_rows; i++) {
        for (int j = 0; j < img_cols; j++) {
            int maximum = -10;
            for (int a = -2; a <= 2; a++) {
                for (int b = -2; b <= 2; b++) {
                    int x = i + a;
                    int y = j + b;
                    if (x < 0 || x >= img_rows || y < 0 || y >= img_cols) {
                        continue;
                    }
                    else {
                        if (ori.at<uchar>(x, y) >= maximum) {
                            maximum = ori.at<uchar>(x, y);
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

    //SNR
    //mean
    double sum = 0;
    double n = (double)img_rows * (double)img_cols;
    for (int i = 0; i < img_rows; i++) {
        for (int j = 0; j < img_cols; j++) {
            sum += original.at<uchar>(i, j);
        }
    }
    double mean = sum / n;
    //vs
    double vs_mean = 0;
    for (int i = 0; i < img_rows; i++) {
        for (int j = 0; j < img_cols; j++) {
            double squ = original.at<uchar>(i, j) - mean;
            vs_mean += squ * squ;
        }
    }
    double vs = vs_mean / n;

    //(a) Generate noisy images with gaussian noise(amplitude of 10 and 30)
    Mat gaussian_10 = original.clone();
    Mat gaussian_30 = original.clone();
    gaussian_noise(gaussian_10, 10);
    gaussian_noise(gaussian_30, 30);

    imwrite("1_gaussian_10.jpg", gaussian_10);
    imwrite("2_gaussian_30.jpg", gaussian_30);

    //(b) Generate noisy images with salt-and-pepper noise( probability 0.1 and 0.05)
    Mat salt_01 = original.clone();
    Mat salt_005 = original.clone();
    salt_pepper(salt_01, 0.1);
    salt_pepper(salt_005, 0.05);

    imwrite("3_salt_01.jpg", salt_01);
    imwrite("4_salt_005.jpg", salt_005);

    //(c) Use the 3x3, 5x5 box filter on images generated by (a)(b)
    Mat gaussian_10_box3 = gaussian_10.clone();
    Mat gaussian_10_box5 = gaussian_10.clone();
    Mat gaussian_30_box3 = gaussian_30.clone();
    Mat gaussian_30_box5 = gaussian_30.clone();
    Mat salt_01_box3 = salt_01.clone();
    Mat salt_01_box5 = salt_01.clone();
    Mat salt_005_box3 = salt_005.clone();
    Mat salt_005_box5 = salt_005.clone();

    box_filter_3(gaussian_10_box3);
    box_filter_5(gaussian_10_box5);
    box_filter_3(gaussian_30_box3);
    box_filter_5(gaussian_30_box5);
    box_filter_3(salt_01_box3);
    box_filter_5(salt_01_box5);
    box_filter_3(salt_005_box3);
    box_filter_5(salt_005_box5);

    imwrite("1_gaussian_10_box3.jpg", gaussian_10_box3);
    imwrite("1_gaussian_10_box5.jpg", gaussian_10_box5);
    imwrite("2_gaussian_30_box3.jpg", gaussian_30_box3);
    imwrite("2_gaussian_30_box5.jpg", gaussian_30_box5);
    imwrite("3_salt_01_box3.jpg", salt_01_box3);
    imwrite("3_salt_01_box5.jpg", salt_01_box5);
    imwrite("4_salt_005_box3.jpg", salt_005_box3);
    imwrite("4_salt_005_box5.jpg", salt_005_box5);

    //(d) Use 3x3, 5x5 median filter on images generated by (a)(b)
    Mat gaussian_10_median3 = gaussian_10.clone();
    Mat gaussian_10_median5 = gaussian_10.clone();
    Mat gaussian_30_median3 = gaussian_30.clone();
    Mat gaussian_30_median5 = gaussian_30.clone();
    Mat salt_01_median3 = salt_01.clone();
    Mat salt_01_median5 = salt_01.clone();
    Mat salt_005_median3 = salt_005.clone();
    Mat salt_005_median5 = salt_005.clone();

    median_filter_3(gaussian_10_median3);
    median_filter_5(gaussian_10_median5);
    median_filter_3(gaussian_30_median3);
    median_filter_5(gaussian_30_median5);
    median_filter_3(salt_01_median3);
    median_filter_5(salt_01_median5);
    median_filter_3(salt_005_median3);
    median_filter_5(salt_005_median5);

    imwrite("1_gaussian_10_median3.jpg", gaussian_10_median3);
    imwrite("1_gaussian_10_median5.jpg", gaussian_10_median5);
    imwrite("2_gaussian_30_median3.jpg", gaussian_30_median3);
    imwrite("2_gaussian_30_median5.jpg", gaussian_30_median5);
    imwrite("3_salt_01_median3.jpg", salt_01_median3);
    imwrite("3_salt_01_median5.jpg", salt_01_median5);
    imwrite("4_salt_005_median3.jpg", salt_005_median3);
    imwrite("4_salt_005_median5.jpg", salt_005_median5);

    //(e) Use both opening-then-closing and closing-then opening filter
    //opening closing
    Mat a1;
    Mat a2;

    Mat gaussian_10_o_c = gaussian_10.clone();
    a1 = gaussian_10.clone();
    a2 = gaussian_10.clone();
    erosion(a1, a2);
    dilation(a2, a1);
    dilation(a1, a2);
    erosion(a2, gaussian_10_o_c);

    Mat gaussian_30_o_c = gaussian_30.clone();
    a1 = gaussian_30.clone();
    a2 = gaussian_30.clone();
    erosion(a1, a2);
    dilation(a2, a1);
    dilation(a1, a2);
    erosion(a2, gaussian_30_o_c);

    Mat salt_01_o_c = salt_01.clone();
    a1 = salt_01.clone();
    a2 = salt_01.clone();
    erosion(a1, a2);
    dilation(a2, a1);
    dilation(a1, a2);
    erosion(a2, salt_01_o_c);

    Mat salt_005_o_c = salt_005.clone();
    a1 = salt_005.clone();
    a2 = salt_005.clone();
    erosion(a1, a2);
    dilation(a2, a1);
    dilation(a1, a2);
    erosion(a2, salt_005_o_c);

    //closing opening
    Mat gaussian_10_c_o = gaussian_10.clone();
    a1 = gaussian_10.clone();
    a2 = gaussian_10.clone();
    dilation(a1, a2);
    erosion(a2, a1);
    erosion(a1, a2);
    dilation(a2, gaussian_10_c_o);

    Mat gaussian_30_c_o = gaussian_30.clone();
    a1 = gaussian_30.clone();
    a2 = gaussian_30.clone();
    dilation(a1, a2);
    erosion(a2, a1);
    erosion(a1, a2);
    dilation(a2, gaussian_30_c_o);

    Mat salt_01_c_o = salt_01.clone();
    a1 = salt_01.clone();
    a2 = salt_01.clone();
    dilation(a1, a2);
    erosion(a2, a1);
    erosion(a1, a2);
    dilation(a2, salt_01_c_o);

    Mat salt_005_c_o = salt_005.clone();
    a1 = salt_005.clone();
    a2 = salt_005.clone();
    dilation(a1, a2);
    erosion(a2, a1);
    erosion(a1, a2);
    dilation(a2, salt_005_c_o);

    imwrite("1_gaussian_10_o_c.jpg", gaussian_10_o_c);
    imwrite("1_gaussian_10_c_o.jpg", gaussian_10_c_o);
    imwrite("2_gaussian_30_o_c.jpg", gaussian_30_o_c);
    imwrite("2_gaussian_30_c_o.jpg", gaussian_30_c_o);
    imwrite("3_salt_01_o_c.jpg", salt_01_o_c);
    imwrite("3_salt_01_c_o.jpg", salt_01_c_o);
    imwrite("4_salt_005_o_c.jpg", salt_005_o_c);
    imwrite("4_salt_005_c_o.jpg", salt_005_c_o);

    //SNR output
    cout << "\n gaussian_10" << endl;
    snr_func(vs, original, gaussian_10);
    cout << "\n gaussian_10_box3" << endl;
    snr_func(vs, original, gaussian_10_box3);
    cout << "\n gaussian_10_box5" << endl;
    snr_func(vs, original, gaussian_10_box5);
    cout << "\n gaussian_10_median3" << endl;
    snr_func(vs, original, gaussian_10_median3);
    cout << "\n gaussian_10_median5" << endl;
    snr_func(vs, original, gaussian_10_median5);
    cout << "\n gaussian_10_o_c" << endl;
    snr_func(vs, original, gaussian_10_o_c);
    cout << "\n gaussian_10_c_o" << endl;
    snr_func(vs, original, gaussian_10_c_o);

    cout << "\n gaussian_30" << endl;
    snr_func(vs, original, gaussian_30);
    cout << "\n gaussian_30_box3" << endl;
    snr_func(vs, original, gaussian_30_box3);
    cout << "\n gaussian_30_box5" << endl;
    snr_func(vs, original, gaussian_30_box5);
    cout << "\n gaussian_30_median3" << endl;
    snr_func(vs, original, gaussian_30_median3);
    cout << "\n gaussian_30_median5" << endl;
    snr_func(vs, original, gaussian_30_median5);
    cout << "\n gaussian_30_o_c" << endl;
    snr_func(vs, original, gaussian_30_o_c);
    cout << "\n gaussian_30_c_o" << endl;
    snr_func(vs, original, gaussian_30_c_o);

    cout << "\n salt_01" << endl;
    snr_func(vs, original, salt_01);
    cout << "\n salt_01_box3" << endl;
    snr_func(vs, original, salt_01_box3);
    cout << "\n salt_01_box5" << endl;
    snr_func(vs, original, salt_01_box5);
    cout << "\n salt_01_median3" << endl;
    snr_func(vs, original, salt_01_median3);
    cout << "\n salt_01_median5" << endl;
    snr_func(vs, original, salt_01_median5);
    cout << "\n salt_01_o_c" << endl;
    snr_func(vs, original, salt_01_o_c);
    cout << "\n salt_01_c_o" << endl;
    snr_func(vs, original, salt_01_c_o);

    cout << "\n salt_005" << endl;
    snr_func(vs, original, salt_005);
    cout << "\n salt_005_box3" << endl;
    snr_func(vs, original, salt_005_box3);
    cout << "\n salt_005_box5" << endl;
    snr_func(vs, original, salt_005_box5);
    cout << "\n salt_005_median3" << endl;
    snr_func(vs, original, salt_005_median3);
    cout << "\n salt_005_median5" << endl;
    snr_func(vs, original, salt_005_median5);
    cout << "\n salt_005_o_c" << endl;
    snr_func(vs, original, salt_005_o_c);
    cout << "\n salt_005_c_o" << endl;
    snr_func(vs, original, salt_005_c_o);

    return 0;
}
