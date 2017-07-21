#include "../include/feature.h"

Feature::Feature()
{

}

Feature::Feature(Mat & patch, Mat & ref)
{
	_patch = patch;
	_ref = ref;
}

// difference

float Feature::diffGlobalMean(const Mat & patch, const Mat & ref)
{
	return abs(mean(patch)[0] - mean(ref)[0]);
}

int Feature::diffPixelsCount(const Mat & patch, const Mat & ref, const int color_gap)
{
	Mat better_patch = patch;

	for ( int rowIndex = 0; rowIndex < ref.rows; rowIndex++ ) {
		for ( int colIndex = 0; colIndex < ref.cols; colIndex++) {
			if ( abs( (int)patch.at<uchar>(rowIndex, colIndex)-(int)ref.at<uchar>(rowIndex, colIndex)) < color_gap ) {
				better_patch.at<uchar>(rowIndex, colIndex) = ref.at<uchar>(rowIndex, colIndex);
			}
		}
	}

	int cnt = 0;
	for ( int rowIndex = 0; rowIndex < ref.rows; rowIndex++ ) {
		for ( int colIndex = 0; colIndex < ref.cols; colIndex++) {
			if ( better_patch.at<uchar>(rowIndex, colIndex) != ref.at<uchar>(rowIndex, colIndex) ) {
				cnt++;
			}
		}
	}
	return cnt;
}



double Feature::getPSNR(const Mat & patch, const Mat & ref)
{
	Mat s1;
    	absdiff(patch, ref, s1);       // |I1 - I2|
   	s1.convertTo(s1, CV_32F);  // cannot make a square on 8 bits
   	s1 = s1.mul(s1);           // |I1 - I2|^2

   	Scalar s = sum(s1);         // sum elements per channel

   	double sse = s.val[0] + s.val[1] + s.val[2]; // sum channels

   	if( sse <= 1e-10) // for small values return zero
   	    return 0;
   	else
   	{
   	    double  mse =sse /(double)(patch.channels() * patch.total());
   	    double psnr = 10.0*log10((255*255)/mse);
   	    return psnr;
   	}
}

double Feature::localmeanSSE(const Mat & patch, const Mat & ref, const int pool_size)
{
	if ( !isPowerOfTwo(pool_size) || pool_size > 32) {
		cout << "Error: in Feature::localmeanSSE(const Mat & patch, const Mat & ref, const int pool_size), ";
		cout << "pool_size should be 4, 8, 16, or 32" << endl;
		return 0;
	}


	//const int pool_size = 4;
	const int num_grid = 128 / pool_size;
	Mat patch_pool( 128 / pool_size, 128 / pool_size, CV_32F, Scalar(0) );
	Mat ref_pool( 128 / pool_size, 128 / pool_size, CV_32F, Scalar(0) );

	double sse = 0;

	for ( int rowIndex = 0; rowIndex < num_grid; rowIndex++ ) {
		for ( int colIndex = 0; colIndex < num_grid; colIndex++ ) {
			int x = rowIndex * 4;
			int y = colIndex * 4;
			for ( int i = 0; i < pool_size; i++ ) {
				for ( int j = 0; j < pool_size; j++ ) {
					patch_pool.at<float>(rowIndex, colIndex) += patch.at<uchar>(x + i, y + j);
					ref_pool.at<float>(rowIndex, colIndex) += ref.at<uchar>(x + i, y + j);
				}
			}
			patch_pool.at<float>(rowIndex, colIndex) /= (pool_size * pool_size);
			ref_pool.at<float>(rowIndex, colIndex) /= (pool_size * pool_size);
			
			//sse += pow ( ( patch_pool.at<float>(rowIndex, colIndex) - ref_pool.at<float>(rowIndex, colIndex) ), 2 );
			sse += pow( ( patch_pool.at<float>(rowIndex, colIndex) - ref_pool.at<float>(rowIndex, colIndex) ), 2);
		}
		
	}

	return sse;
}


double Feature::localmeanSAE(const Mat & patch, const Mat & ref, const int pool_size)
{
	if ( !isPowerOfTwo(pool_size) || pool_size > 32 ) {
		cout << "in Feature::localmeanSAE(const Mat & patch, const Mat & ref, const int pool_size)" << endl;
		cout << "pool_size should be 4, 8, 16, or 32" << endl;
		return 0;
	}

	
	//const int pool_size = 4;
	const int num_grid = 128 / pool_size;
	Mat patch_pool( 128 / pool_size, 128 / pool_size, CV_32F, Scalar(0) );
	Mat ref_pool( 128 / pool_size, 128 / pool_size, CV_32F, Scalar(0) );

	double sae = 0;

	for ( int rowIndex = 0; rowIndex < num_grid; rowIndex++ ) {
		for ( int colIndex = 0; colIndex < num_grid; colIndex++ ) {
			int x = rowIndex * 4;
			int y = colIndex * 4;
			for ( int i = 0; i < pool_size; i++ ) {
				for ( int j = 0; j < pool_size; j++ ) {
					patch_pool.at<float>(rowIndex, colIndex) += patch.at<uchar>(x + i, y + j);
					ref_pool.at<float>(rowIndex, colIndex) += ref.at<uchar>(x + i, y + j);
				}
			}
			patch_pool.at<float>(rowIndex, colIndex) /= (pool_size * pool_size);
			ref_pool.at<float>(rowIndex, colIndex) /= (pool_size * pool_size);
			
			sae += abs( patch_pool.at<float>(rowIndex, colIndex) - ref_pool.at<float>(rowIndex, colIndex) );
		}
		
	}

	return sae;
}

