#ifndef FEATURE_H
#define FEATURE_H

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "utility.h"
#include "opencv2/xfeatures2d.hpp"

using namespace std;
using namespace cv;


typedef struct{
	double min;
	double max;
	double mean;
	double stdev;
	double var;
} Statistics;

class Feature {
public:
	Feature();
	Feature(const Mat & patch, const Mat & ref);

	// difference
	float diffGlobalMean(const Mat & patch, const Mat & ref);
	int diffPixelsCount(const Mat & patch, const Mat & ref, const int color_gap);

	double getPSNR(const Mat & patch, const Mat & ref);
	
	
	// local PSNR
	// compare with reference
	Statistics localPSNR(const Mat & patch, const Mat & ref, const int pool_size);
	void mostDifferentPatch(const Mat & patch, const int pool_size);

	// local feature 
	// (mean)
	double localmeanSSE(const Mat & patch, const Mat & ref, const int pool_size); // sum of squared error	
	double localmeanSAE(const Mat & patch, const Mat & ref, const int pool_size); // sum of absolute error	
	
	// block analysis
	int maxNumBlackPixelsInBlock(const int thresh, const int block_size); // patch
	int maxNumBlackPixelsInBlock(const Mat & srcImg, const int thresh, const int block_size); // srcImg
	int maxNumDiffPixelsInBlock(const int thresh, const int block_size); // compare patch and ref

	// SIFT
	void trySIFT(const Mat & patch, const Mat & ref);
	//HoughLine
	int houghLine(const Mat & patch, const Mat & ref);

private:
	Mat _patch;
	Mat _ref;
};


#endif // FEATURE_H
