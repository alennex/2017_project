#ifndef ALIGNMENT_H
#define ALIGNMENT_H
#include<fstream>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "utility.h"

using namespace std;
using namespace cv;

class Alignment {
public:
	Alignment();
	Alignment(Mat & patch, Mat & ref);
	
	// ECC Image Alignment
	void alignECC_Translation(Mat & patch, Mat & ref);
	void alignECC_Euclidean(Mat & patch, Mat & ref);
	void alignECC_Affine(Mat & patch, Mat & ref);
	void alignECC_Homography(Mat & patch, Mat & ref);

	//DCT Tranform	
	void alignDCT(Mat &patch, Mat &ref);
	void alignDFT(Mat &patch, Mat &ref);
	
	// histogram	
	void histogramEqualization(Mat & patch, Mat & ref);	
	
	// thresholding
	void threshOtsu(Mat &patch,Mat &ref);
	void threshBinary(Mat &patch,Mat &ref);
	void threshBinaryINV(Mat &patch,Mat &ref);
	void threshTrunc(Mat &patch,Mat &ref);
	void threshToZero(Mat &patch,Mat &ref);
	void threshToZeroINV(Mat &patch,Mat &ref);

	
	void align();

private:
	Mat _patch;
	Mat _ref;
};

#endif // ALIGNMENT_H
