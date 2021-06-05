#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int img_rows;
int img_cols;
int yokoi_m[64][64];

//binarize and downsample
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

void downsample(Mat ori, Mat img) {
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            img.at<uchar>(i, j) = ori.at<uchar>(i * 8, j * 8);
        }
    }
}

//Yokoi
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
                yokoi_m[i][j] = 0;
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
            yokoi_m[i][j] = f(a[0], a[1], a[2], a[3]);
        }
    }
}

//shrink
int h_shrink(int b, int c, int d, int e) {
    if (b == c) {
        if (d != b || e != b) {
            return 1;
        }
        else {
            return 0;
        }
    }
    return 0;
}
    
char f_shrink(int a1, int a2, int a3, int a4, int x0) {
    if (a1 == 1 || a2 == 1 || a3 == 1 || a4 == 1) {
        int count = 0;
        if (a1 == 1) {
            count++;
        }
        if (a2 == 1) {
            count++;
        }
        if (a3 == 1) {
            count++;
        }
        if (a4 == 1) {
            count++;
        }

        if (count == 1) {
            return 'g';
        }
        else {
            return x0;
        }
    }
    else {
        return x0;
    }
}

char shrink(int i, int j , Mat down) {
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
    a[0] = h_shrink(x[0], x[1], x[6], x[2]);
    a[1] = h_shrink(x[0], x[2], x[7], x[3]);
    a[2] = h_shrink(x[0], x[3], x[8], x[4]);
    a[3] = h_shrink(x[0], x[4], x[5], x[1]);

    return f_shrink(a[0], a[1], a[2], a[3], x[0]);
}

char pair_op(int i, int j) {
    int u = 0, d = 0, r = 0, l = 0;

    if (i != 0) {
        u = yokoi_m[i - 1][j];
    }
    if (i != 63) {
        d = yokoi_m[i + 1][j];
    }
    if (j != 0) {
        l = yokoi_m[i][j - 1];
    }
    if (j != 63) {
        r = yokoi_m[i][j + 1];
    }
    if (u == 1 || d == 1 || l == 1 || r == 1) {
        return 'p';
    }
    else {
        return 'q';
    }

}
int main() {
    Mat original;
    original = imread("lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    img_rows = original.rows;
    img_cols = original.cols;

    //downsample
    Mat down(64, 64, CV_8UC1);
    binarize(original);
    downsample(original, down);

    //output original
    imwrite("0.jpg", down);

    //thinning
    int time = 1;
    while (time <= 1000) {
        int change = 0;
        string name = to_string(time) + ".jpg";

        //yokoi
        yokoi(down);

        //process
        for (int i = 0; i < 64; i++) {
            for (int j = 0; j < 64; j++) {
                if (yokoi_m[i][j] == 1 && pair_op(i,j) == 'p' &&  shrink(i, j, down) == 'g') {
                    down.at<uchar>(i, j) = 0;
                    change++;
                }
                else if (down.at<uchar>(i, j) == 0) {
                    down.at<uchar>(i, j) = 0;
                }
                else{
                    down.at<uchar>(i, j) = 255;
                }

            }
        }
        imwrite(name, down);
        if (change == 0) {
            cout << "Iterations: " << time << endl;
            break;
        }
        time++;
    }
    return 0;
}
