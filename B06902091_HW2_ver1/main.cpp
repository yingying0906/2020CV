#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int label[1000][1000];//label table

int loop(int label[][1000], int rows, int cols) {
    //top down
    int changes = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (label[i][j] == 0) {//black
                continue;
            }
            if (i == 0 && j == 0) {//left up
                continue;
            }
            else if (i == 0 && j > 0) {//1st row
                if (label[i][j - 1] > 0) {
                    if (label[i][j - 1] < label[i][j]) {
                        label[i][j] = label[i][j - 1];
                        changes ++;
                        
                    }
                }
            }
            else if (j == 0 && i > 0) {//1st col
                if (label[i - 1][j] > 0) {
                    if (label[i - 1][j] < label[i][j]) {
                        label[i][j] = label[i - 1][j];
                        changes++;
                    }
                }
            }
            else {//other
                int min = 10000;
                if (label[i][j - 1] > 0) {//left
                    if (label[i][j - 1] <= min) {
                        min = label[i][j - 1];
                    }
                }
                if (label[i - 1][j] > 0) {//up
                    if (label[i - 1][j] <= min) {
                        min = label[i - 1][j];
                    }
                }
                if (min != 10000) {
                    if (label[i][j] != min) {
                        label[i][j] = min;
                        changes++;
                    }
                }
            }
        }
    }

    //bottom up
    for (int i = 511; i >= 0; i--) {
        for (int j = 511; j >= 0; j--) {
            if (label[i][j] == 0) {//black
                continue;
            }
            if (i == 511 && j == 511) {//right down
                continue;
            }
            else if (i == 511 && j < 511) {//last row
                if (label[i][j + 1] > 0) {
                    if (label[i][j + 1] < label[i][j]) {
                        label[i][j] = label[i][j + 1];
                        changes++;
                    }
                }
            }
            else if (j == 511 && i < 511) {//last col
                if (label[i + 1][j] > 0) {
                    if (label[i + 1][j] < label[i][j]) {
                        label[i][j] = label[i + 1][j];
                        changes++;
                    }
                }
            }
            else {//other
                int min = 10000;
                if (label[i][j + 1] > 0) {//right
                    if (label[i][j + 1] <= min) {
                        min = label[i][j + 1];
                    }
                }
                if (label[i + 1][j] > 0) {//down
                    if (label[i + 1][j] <= min) {
                        min = label[i + 1][j];
                    }
                }
                if (min != 10000) {
                    if (label[i][j] != min) {
                        label[i][j] = min;
                        changes++;
                    }
                }
            }
        }
    }
    return changes;
}
int main() {
//1a binarize//
    Mat img;
    Mat A;

    img = imread("lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    A = img.clone();

    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            if (A.at<uchar>(i, j) < 128) {//0~127
                A.at<uchar>(i, j) = 0;
            }
            else {
                A.at<uchar>(i, j) = 255;
            }
        }
    }

//1b histogram//
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

    //graph resize
    //float ratio = max / 256.0;
    float ratio = max / 768.0;
    
    //background
    //Mat graph(256, 256, CV_8UC1, Scalar(0)); //y, x
    Mat graph(768, 768, CV_8UC1, Scalar(0)); //size*3

    //plot
    for (int x = 0; x < 256; x++) {//0~255
        for (int y = 0; y < value[x]; y++) {//number
            graph.at<uchar>(767 - floor(y / ratio), x * 3) = 255;
            graph.at<uchar>(767 - floor(y / ratio), x * 3 + 1) = 255;
            graph.at<uchar>(767 - floor(y / ratio), x * 3 + 2) = 255;
        }
    }
    //resize(graph, graph, Size(img.rows * 2, img.cols * 2), 0, 0, CV_INTER_LINEAR);

//1c connected  (4-con)
    //label initial
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            if (A.at<uchar>(i, j) == 255) {
                label[i][j] = -1;//white
            }
            else {
                label[i][j] = 0;//black
            }
        }
    }

    //1st top down + label
    int tag = 1;
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            int need = 0;//label?
            if (label[i][j] == 0) {//black
                continue;
            }
            if (i == 0 && j == 0) {//left up
                if (label[i][j] == -1) {
                    need = 1;
                }
            }
            else if (i == 0 && j > 0) {//1st row
                if (label[i][j - 1] > 0) {
                    label[i][j] = label[i][j - 1];
                }
                else {
                    need = 1;
                }
            }
            else if (j == 0 && i > 0) {//1st col
                if (label[i - 1][j] > 0) {
                    label[i][j] = label[i - 1][j];
                }
                else {
                    need = 1;
                }
            }
            else {
                int min = 10000;
                if (label[i][j - 1] > 0) {//left
                    if (label[i][j - 1] <= min) {
                        min = label[i][j - 1];
                    }
                }
                if (label[i - 1][j] > 0) {//up
                    if (label[i - 1][j] <= min) {
                        min = label[i - 1][j];
                    }
                }
                if (min == 10000) {//need tag
                    need = 1;
                }
                else {
                    need = 0;
                    label[i][j] = min;//tag
                }
            }
            //label
            if (need == 1) {
                label[i][j] = tag;
                tag++;
            }
        }
    }

    //loop topdown
    int z = 0;
    while (z <100) {
        int change = 0;
        change = loop(label, img.rows, img.cols);
        //cout <<"change_   "<< change<<endl;

        //no change
        if (change == 0) {
            //cout << "break at z = "<< z << endl << endl;
            break;
        }
        z++;
    }
    /////////////////////////////////////////////////
    //outputtext
    //ofstream file;
    //file.open("end.txt");
    //for (int i = 0; i < 512; i++) {
    //    for (int j = 0; j < 200; j++) {
    //        file << setw(4) << label[i][j] << " ";
    //    }
    //    file << endl;
    //}
    //file.close();
    /////////////////////////////////////////////////

    //count tag
    int count[3000] = { 0 };
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            if (label[i][j] > 0) {//black
                count[label[i][j]] ++;
            }
        }
    }

    //clear <500
    int box_num = 0;
    for (int i = 1; i < 3000; i++) {
        if (count[i] >= 500) {
            //cout << "tag: " << i <<" = " << count[i] << endl << endl;
            box_num++;
        }
        else {
            count[i] = 0;
        }
    }
    cout <<"total number of bounding box = "<< box_num << endl << endl;

    //Find bounding box coordinate
    Mat C;
    C = A.clone();
    cvtColor(C, C, COLOR_GRAY2RGB);

    for (int z = 1; z < 3000; z++) {
        //centroid
        int total_X = 0;
        int total_Y = 0;
        int area = 0;
        if (count[z] >= 500) {
            //find 4 vert
            int min1_x = 10000;//0
            int min1_y = 10000;
            int min2_x = 10000;
            int min2_y = 10000;

            int max1_x = 0;//mov
            int max1_y = 0;
            int max2_x = 0;
            int max2_y = 0;

            for (int i = 0; i < img.rows; i++) {
                for (int j = 0; j < img.cols; j++) {
                    if (label[i][j] == z) {
                        //cout << min1_x << " " << min1_y << " // " << b_x << " " << min2_y << "  //  " << c_x << " " << max1_y << "  //   " << max2_x << " " << max2_y << " " << endl;
                        if (i < min1_x){
                            min1_x = i; 
                            min1_y = j;
                        }
                        if (i > max1_x) {
                            max1_x = i; 
                            max1_y = j;
                        }
                        if (j < min2_y) {
                            min2_x = i;
                            min2_y = j; 
                        }
                        if (j > max2_y) {
                            max2_x = i;
                            max2_y = j;
                        }
                        total_X += i;
                        total_Y += j;
                        area++;
                    }
                }
            }
            //find min max
            int row[4] = { min1_x,min2_x,max1_x,max2_x };
            int col[4] = { min1_y,min2_y,max1_y,max2_y };

            int xmin = 9999; int ymin = 9999;
            int xmax = -1; int ymax = -1;

            for (int k = 0; k < 4; k++) {
                if (row[k] < xmin) {
                    xmin = row[k];
                }
                if (row[k] > xmax) {
                    xmax = row[k];
                }
                if (col[k] < ymin) {
                    ymin = col[k];
                }
                if (col[k] > ymax) {
                    ymax = col[k];
                }
            }

            //cout << "tag = " << z << endl;
            //cout << "(" <<xmin <<" "<< ymin << ")" << "(" <<xmax << " " << ymax << ")" << endl << endl;

            //draw box
            rectangle(C, Point(ymin, xmin), cvPoint(ymax, xmax), Scalar(50, 50, 255), 1, 1, 0);
            //draw center
            Point center = Point(total_Y / area, total_X / area);
            circle(C, center, 2, Scalar(255, 130, 0), -1);
        }
    }

    //output file
    imwrite("1a.jpg", A);
    imwrite("1b.jpg", graph);
    imwrite("1c.jpg", C);

    cout << "done!!!!!!!!" << endl;
    return 0;
}
