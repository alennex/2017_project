#ifndef FEATURE_H
#define FEATURE_H

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "utility.h"

using namespace std;
using namespace cv;

class Feature {
public:
	Feature();
	Feature(Mat & patch, Mat & ref);

	// difference
	float diffGlobalMean(const Mat & patch, const Mat & ref);
	int diffPixelsCount(const Mat & patch, const Mat & ref, const int color_gap);

	double getPSNR(const Mat & patch, const Mat & ref);

	// local feature 
	// (mean)
	double localmeanSSE(const Mat & patch, const Mat & ref, const int pool_size); // sum of squared error	
	double localmeanSAE(const Mat & patch, const Mat & ref, const int pool_size); // sum of absolute error	

private:
	Mat _patch;
	Mat _ref;
};


#endif // FEATURE_H
