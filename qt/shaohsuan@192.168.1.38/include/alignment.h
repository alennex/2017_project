#ifndef ALIGNMENT_H
#define ALIGNMENT_H
#include<fstream>
#include <iostream>
#include<math.h>
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
	int alignECC_Translation(Mat & patch, Mat & ref, int num_of_iteraions, double termination_eps);
	int alignECC_Euclidean(Mat & patch, Mat & ref, int num_of_iterations, double termination_eps);
	int alignECC_Affine(Mat & patch, Mat & ref, int num_of_iterations, double termination_eps);
	int alignECC_Homography(Mat & patch, Mat & ref, int num_of_iterations, double termination_eps);

	// Frequency Tranform
	void alignDCTTopRLeftCAvg(Mat &patch, Mat &ref);
	void alignDFT(Mat &patch, Mat &ref);
	void alignDCTKeepTenPixels(Mat &patch, Mat &ref);
	void alignDFTKeepTenPixels(Mat &patch, Mat &ref);
	void alignDCTZigZag10(Mat &patch, Mat &ref);

	void alignReplaceNotSoDifferent(Mat &patch, const Mat &ref, const int color_gap);
	
	// histogram	
	void histogramEqualization(Mat & patch, Mat & ref);	
	
	// thresholding
	void threshOtsu(Mat &patch,Mat &ref, double thresh, double max_value);
	void threshBinary(Mat &patch,Mat &ref, double thresh, double max_value);
	void threshBinaryINV(Mat &patch,Mat &ref, double thresh, double max_value);
	void threshTrunc(Mat &patch,Mat &ref, double thresh, double max_value);
	void threshToZero(Mat &patch,Mat &ref, double thresh, double max_value);
	void threshToZeroINV(Mat &patch,Mat &ref, double thresh, double max_value);

	int align();

	void ICFilter(string ic_method, double keep_rate, const char csv_path[], string &out_csv_path, int & num_kept_defect, int & num_kept_nuisance, bool discard);

	
	int IC0000(Mat & patch, Mat & ref, int num_its, double term_eps);
	int IC0001(Mat & patch, Mat & ref, int num_its, double term_eps);
	int IC0002(Mat & patch, Mat & ref, int num_its, double term_eps);
	int IC0003(Mat & patch, Mat & ref, int num_its, double term_eps);
	
	int IC0010(Mat & patch, Mat & ref, int num_its, double term_eps);
	int IC0011(Mat & patch, Mat & ref, int num_its, double term_eps);
	int IC0012(Mat & patch, Mat & ref, int num_its, double term_eps);
	int IC0013(Mat & patch, Mat & ref, int num_its, double term_eps);
	
	int IC0020(Mat & patch, Mat & ref, int num_its, double term_eps);
	int IC0021(Mat & patch, Mat & ref, int num_its, double term_eps);
	int IC0022(Mat & patch, Mat & ref, int num_its, double term_eps);
	int IC0023(Mat & patch, Mat & ref, int num_its, double term_eps);
	
	int IC0030(Mat & patch, Mat & ref, int num_its, double term_eps);
	int IC0031(Mat & patch, Mat & ref, int num_its, double term_eps);
	int IC0032(Mat & patch, Mat & ref, int num_its, double term_eps);
	int IC0033(Mat & patch, Mat & ref, int num_its, double term_eps);
	
	int IC0040(Mat & patch, Mat & ref, int num_its, double term_eps);
	int IC0041(Mat & patch, Mat & ref, int num_its, double term_eps);
	int IC0042(Mat & patch, Mat & ref, int num_its, double term_eps);
	int IC0043(Mat & patch, Mat & ref, int num_its, double term_eps);
	
	int IC0050(Mat & patch, Mat & ref, int num_its, double term_eps);
	int IC0051(Mat & patch, Mat & ref, int num_its, double term_eps);
	int IC0052(Mat & patch, Mat & ref, int num_its, double term_eps);
	int IC0053(Mat & patch, Mat & ref, int num_its, double term_eps);
	
	int IC0060(Mat & patch, Mat & ref, int num_its, double term_eps);
	int IC0061(Mat & patch, Mat & ref, int num_its, double term_eps);
	int IC0062(Mat & patch, Mat & ref, int num_its, double term_eps);
	int IC0063(Mat & patch, Mat & ref, int num_its, double term_eps);
	
	void IC0100(Mat & patch, Mat & ref);
	void IC0110(Mat & patch, Mat & ref);
	void IC0111(Mat & patch, Mat & ref);
	void IC0112(Mat & patch, Mat & ref);
	void IC0200(Mat & patch, Mat & ref);
	void IC0210(Mat & patch, Mat & ref);
	void IC0300(Mat & patch, Mat & ref);
	void IC0310(Mat & patch, Mat & ref);
	void IC0400(Mat & patch, Mat & ref);
	void IC0410(Mat & patch, Mat & ref);

	
private:
	Mat _patch;
	Mat _ref;

	// start with ECC transform	
	std::map<std::string, std::function<int(Mat&, Mat&, int, double)>> icmap00;
	std::map<std::string, std::function<void(Mat&, Mat&)>> icmap;
};

#endif // ALIGNMENT_H
