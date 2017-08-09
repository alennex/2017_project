#ifndef FEATURE_H
#define FEATURE_H

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "utility.h"
#include "alignment.h"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/ximgproc.hpp"
#include "opencv2/saliency.hpp"

using namespace std;
using namespace cv;
using namespace saliency;

typedef struct{
	double min;
	double max;
	double mean;
	double stdev;
	double var;
	int diffPatchCount;
} Statistics;

class Feature {
public:
	Feature();
	Feature(const Mat & patch, const Mat & ref);

	// Difference
	float diffGlobalMean(const Mat & patch, const Mat & ref);

	// Different count
	// 1. Replace the patch's pixel value by reference's pixel value if their difference is smaller than color_gap
	// 2. Count the number of changed pixels
	int diffPixelsCount(const Mat & patch, const Mat & ref, const int color_gap);
	// Return difference #hough_line count
	int diffHoughLineCount(const Mat & patch, const Mat & ref, int min_val, int max_val, int threshold);

	// Compute Peak Signal-to-Noise-Ratio between two images
	double getPSNR(const Mat & patch, const Mat & ref);
	
	// Local: divide the image into cells first
	// Get statistical PSNR data among cells compared between patch and reference
	Statistics localPSNR(const Mat & patch, const Mat & ref, const int poolsize);

	// Compare among sub-patches in patch
	double lowestMaxLocalPSNR(const Mat & patch, const int pol_size);
	int mostDifferentPatchCount(const Mat & patch, const int pool_size);
	
	// return Mat_<double>, with sub-patch psnrs ( comparison among sub-patches)
	Mat calPSNRs(const Mat & patch, const int pool_size);

	// Calculate SSE or SAE of means among every cell
	double localmeanSSE(const Mat & patch, const Mat & ref, const int pool_size); // sum of squared error	
	double localmeanSAE(const Mat & patch, const Mat & ref, const int pool_size); // sum of absolute error	
	
	// block analysis
	// Use a block to scan the patch, count the max number of black(darker than thresh) pixels among blocks
	int maxNumBlackPixelsInBlock(const int thresh, const int block_size); // patch
	// Use a block to scan the srcImg, count the number max number of black(darker than thresh  pixels among blcoks
	int maxNumBlackPixelsInBlock(const Mat & srcImg, const int thresh, const int block_size); // srcImg
	// Use a block to scan the patch and references images, coumt the max number of different bewteeen patch and reference
	int maxNumDiffPixelsInBlock(const Mat & patch, const Mat & ref, const int thresh, const int block_size); // compare patch and ref
	
	// SuperPixel
	void trySuperPixel(const Mat & patch, const Mat & ref);
	
	// Saliency map
	void trySaliencyMap(const Mat & patch, const Mat & ref);
	
	// SIFT
	void trySIFT(const Mat & patch, const Mat & ref);


	//Encoding

	//******Difference******/
	//diffGlobalMean
	float FE5000(const Mat & patch, const Mat & ref);
	//******Difference******/


	//******Different count******/
	//diffPixelsCount
	int FE5100(const Mat & patch, const Mat & ref); 
	int FE5101(const Mat & patch, const Mat & ref); 
	int FE5102(const Mat & patch, const Mat & ref); 
	int FE5103(const Mat & patch, const Mat & ref); 
	int FE5104(const Mat & patch, const Mat & ref); 
	//diffHoughLineCount	
	int FE5110(const Mat & patch, const Mat & ref); 
	//******Different count******/

	
	//******PSNR******/
	double FE5200(const Mat & patch, const Mat & ref); 
	//localPSNR.min
	double FE5210(const Mat & patch, const Mat & ref); 
	double FE5211(const Mat & patch, const Mat & ref); 
	double FE5212(const Mat & patch, const Mat & ref); 
	//localPSNR.max
	double FE5220(const Mat & patch, const Mat & ref); 	
	double FE5221(const Mat & patch, const Mat & ref);
	double FE5222(const Mat & patch, const Mat & ref);
	//localPSNR.mean
	double FE5230(const Mat & patch, const Mat & ref); 
	double FE5231(const Mat & patch, const Mat & ref); 
	double FE5232(const Mat & patch, const Mat & ref); 
	//localPSNR.stdev
	double FE5240(const Mat & patch, const Mat & ref); 
	double FE5241(const Mat & patch, const Mat & ref); 	
	double FE5242(const Mat & patch, const Mat & ref);
	//localPSNR.var
	double FE5250(const Mat & patch, const Mat & ref); 
	double FE5251(const Mat & patch, const Mat & ref); 	
	double FE5252(const Mat & patch, const Mat & ref);
	//******PSNR******/
	

	//******localmean******//
	//localmeanSSE
	double FE5300(const Mat & patch, const Mat & ref); 
	double FE5301(const Mat & patch, const Mat & ref); 	
	double FE5302(const Mat & patch, const Mat & ref);
	double FE5303(const Mat & patch, const Mat & ref);
	double FE5304(const Mat & patch, const Mat & ref);
	double FE5305(const Mat & patch, const Mat & ref);
	//localmeanSAE
	double FE5310(const Mat & patch, const Mat & ref); 
	double FE5311(const Mat & patch, const Mat & ref); 	
	double FE5312(const Mat & patch, const Mat & ref);
	double FE5313(const Mat & patch, const Mat & ref);
	double FE5314(const Mat & patch, const Mat & ref);
	double FE5315(const Mat & patch, const Mat & ref);
	//******localmean******//
	

	//******Block analysis******//
	//maxNumBlackPixelsInBlock
	int FE5400(const Mat & patch, const Mat & ref); 
	int FE5401(const Mat & patch, const Mat & ref); 	
	int FE5402(const Mat & patch, const Mat & ref);
	int FE5403(const Mat & patch, const Mat & ref);
	int FE5404(const Mat & patch, const Mat & ref);
	int FE5405(const Mat & patch, const Mat & ref);
	//maxNumDiffPixelsInBlock
	int FE5410(const Mat & patch, const Mat & ref); 
	int FE5411(const Mat & patch, const Mat & ref); 	
	int FE5412(const Mat & patch, const Mat & ref);
	int FE5413(const Mat & patch, const Mat & ref);
	int FE5414(const Mat & patch, const Mat & ref);
	int FE5415(const Mat & patch, const Mat & ref);
	int FE5416(const Mat & patch, const Mat & ref);
	int FE5417(const Mat & patch, const Mat & ref);
	int FE5418(const Mat & patch, const Mat & ref);
	int FE5419(const Mat & patch, const Mat & ref);
	//******Block analysis******//

	
	//******Superpixels analysis******//
	void FE5500(const Mat & patch, const Mat & ref);
	//******Superpixels analysis******//

private:
	Mat _patch;
	Mat _ref;
};


#endif // FEATURE_H
