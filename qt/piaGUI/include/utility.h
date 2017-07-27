#ifndef UTILITY_H
#define UTILITY_H

#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include <numeric>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include "alignment.h"
#include "feature.h"

using namespace std;
using namespace cv;

int readNumOfLines(const char filename[]);
string** readcsv(const char filename[], int row, int col);
void readcsv(const char filename[], string ** table, int row, int col);
void writecsv(const char filename[] , string** table, int rows, int cols);
void calResult(string** result, int num_of_lines, int num_of_cols);

Mat getHistImg(const Mat &srcImg);
Mat getGradient(const Mat & srcImg);
Mat fourierTransform(const Mat & I);
Mat cosineTransform(const Mat & I);

bool isPowerOfTwo(int x);

vector<Mat> sliceImg(const Mat &srcImg, const int pool_size);
void showGrid(const Mat srcImg, const int pool_size);

void printVector(vector<double> v);

double calSSE(const Mat &img1, const Mat &img2);
double calSSE(vector<double> v1, vector<double> v2);
double calSAE(const Mat &img1, const Mat &img2);
double calSAE(vector<double> v1, vector<double> v2);

#endif // UTILITY_H
