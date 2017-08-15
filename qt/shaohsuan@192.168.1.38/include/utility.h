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
#include <cstdlib>
#include <map>
#include <functional>

#include "alignment.h"
#include "feature.h"

using namespace std;
using namespace cv;

string clearDoublequote(string str);
string extractFilename(string& str);
void createFolders(string name, string left_folder, string right_folder);
string createImgnamewithIC(string& ic_method, string& _folder, string &_name);

cv::Size readNumOfLines(const char filename[]);
string** readcsv(const char filename[], int row, int col);
void readcsv(const char filename[], string ** table, int row, int col);
void writecsv(const char filename[] , string** table, const int rows, const int cols, const int num_of_attributes, bool discard);

//void ICFilter(string ic_method, double keep_rate, const char csv_path[], int & num_kept_defect, int & num_kept_nuisance);

float calResult(string** result, const int num_of_lines, const int num_of_cols, const int num_of_attributes);


Mat getHistImg(const Mat &srcImg);

Mat getGradient(const Mat & srcImg);

Mat fourierTransform(const Mat & I);
Mat cosineTransform(const Mat & I);
Mat phaseOnlyFT(const Mat & I);

bool isPowerOfTwo(int x);

vector<Mat> sliceImg(const Mat &srcImg, const int pool_size);
void showGrid(const Mat srcImg, const int pool_size);

void printVector(vector<double> v);
Mat showHistograms_localPSNR(vector<double> v);

double calSSE(const Mat &img1, const Mat &img2);
double calSSE(vector<double> v1, vector<double> v2);
double calSAE(const Mat &img1, const Mat &img2);
double calSAE(vector<double> v1, vector<double> v2);


#endif // UTILITY_H
