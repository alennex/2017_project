#include "../include/feature.h"

Feature::Feature()
{

}

Feature::Feature(const Mat & patch, const Mat & ref)
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
	Mat better_patch = patch.clone();

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

template <class T>
Statistics calStatistics(vector<T> v)
{
	Statistics statistics;
	statistics.min = *std::min_element(v.begin(), v.end());
	statistics.max = *std::max_element(v.begin(), v.end());
	double sum = std::accumulate(v.begin(), v.end(), 0.0);
	double mean = sum / v.size();
	statistics.mean = mean;
	vector<double> diff(v.size());
	std::transform(v.begin(), v.end(), diff.begin(), [mean](double x){ return x-mean; });
	double sq_sum =  std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
	double stdev = std::sqrt(sq_sum / v.size());
	statistics.stdev = stdev;
	statistics.var = pow(stdev, 2);

	return statistics;
}

void showStatistics(Statistics s)
{
	cout << s.min << "  " << s.max << "  " << s.mean << "  " << s.stdev << "  " << s.var << endl;
}

Statistics Feature::localPSNR(const Mat & patch, const Mat & ref, const int pool_size)
{
	vector<Mat> slices_patch = sliceImg(patch, pool_size);
	vector<Mat> slices_ref = sliceImg(ref, pool_size);
	
	vector<double> local_psnrs;
	for ( int i = 0; i < slices_patch.size(); i++ ) {
		local_psnrs.push_back(getPSNR(slices_patch[i], slices_ref[i]));
	}

	return calStatistics<double>(local_psnrs);
}

void Feature::mostDifferentPatch(const Mat & patch, const int pool_size)
{
	vector<Mat> slices_patch = sliceImg(patch, pool_size);
	
	Mat_<double> psnrs(slices_patch.size(), slices_patch.size());
 
	for ( int i = 0; i < slices_patch.size(); i++ ) {
		for ( int j = i; j < slices_patch.size(); j++ ) {
			psnrs.at<double>(i, j) = getPSNR(slices_patch[i], slices_patch[j]);
		}
	}

	// avoid re-calculate but unstable
	//psnrs += psnrs.t();
	//cout << psnrs << endl;
	
	vector<Statistics> sta;
	for ( int i = 0; i < slices_patch.size(); i++ ) {
		vector<double> v;
		for ( int j = 0; j < slices_patch.size(); j++ ) {
			if (i < j) {
				v.push_back(psnrs.at<double>(i, j));
			}
			if (i > j) {
				v.push_back(psnrs.at<double>(j, i));
			}
		}
		//printVector(v);
		sta.push_back(calStatistics<double>(v));
		cout << i << "  ";
		showStatistics(sta[i]);
	}
	
				
}

double Feature::localmeanSSE(const Mat & patch, const Mat & ref, const int pool_size)
{
	vector<Mat> slices_patch = sliceImg(patch, pool_size);
	vector<Mat> slices_ref = sliceImg(ref, pool_size);
	
	int sse = 0;	
	for ( int i = 0; i < slices_patch.size(); i++ ) {
		sse += pow(mean(slices_patch[i])[0] - mean(slices_ref[i])[0], 2);
	}

	return sse;
}


double Feature::localmeanSAE(const Mat & patch, const Mat & ref, const int pool_size)
{
	/*if ( !isPowerOfTwo(pool_size) || pool_size > 32 ) {
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
		
	}*/

	vector<Mat> slices_patch = sliceImg(patch, pool_size);
	vector<Mat> slices_ref = sliceImg(ref, pool_size);
	
	int sae = 0;	
	for ( int i = 0; i < slices_patch.size(); i++ ) {
		sae += abs(mean(slices_patch[i])[0] - mean(slices_ref[i])[0]);
	}
	return sae;
}

int getIntegralval(const Mat & num_black_mat, int rowIndex, int colIndex) {
	if ( rowIndex < 0 || colIndex < 0 ) {
		return 0;
	}	
	else {
		return num_black_mat.at<int>(rowIndex, colIndex);
	}
}

int Feature::maxNumBlackPixelsInBlock(const int thresh, const int block_size)
{
	Mat he_patch;
	equalizeHist(_patch, he_patch);
	// Integral image -> speed up
	Mat num_black_mat(_patch.size(), CV_32S, Scalar(0));
	for ( int rowIndex = 0; rowIndex < _patch.rows; rowIndex++ ) {
		if ( (int)he_patch.at<uchar>(rowIndex, 0) < thresh ) {
			num_black_mat.at<int>(rowIndex, 0) = 1;	
		}
		for ( int colIndex = 1; colIndex < _patch.cols; colIndex++ ) {
			if ( (int)he_patch.at<uchar>(rowIndex, colIndex) < thresh ) {
				num_black_mat.at<int>(rowIndex, colIndex) = num_black_mat.at<int>(rowIndex, colIndex - 1) + 1;
			}
			else {
				num_black_mat.at<int>(rowIndex, colIndex) = num_black_mat.at<int>(rowIndex, colIndex - 1);
			}
		}
	}
	for ( int colIndex = 0; colIndex < _patch.cols; colIndex++ ) {
		for ( int rowIndex = 1; rowIndex < _patch.rows; rowIndex++ ) {
			num_black_mat.at<int>(rowIndex, colIndex) = num_black_mat.at<int>(rowIndex-1, colIndex) + num_black_mat.at<int>(rowIndex, colIndex);
		}
	}

	int max_num_blacks = 0;
	for ( int i = 0; i <= _patch.rows - block_size; i++ ) {
		for ( int j = 0; j <= _patch.cols - block_size; j++ ) {
			int upper_left = getIntegralval(num_black_mat, i - 1, j - 1);
			int upper_right = getIntegralval(num_black_mat, i - 1, j + block_size - 1);
			int lower_left = getIntegralval(num_black_mat, i + block_size - 1, j - 1);
			int lower_right = getIntegralval(num_black_mat, i + block_size - 1, j + block_size - 1);	
			int num_blacks = lower_right - lower_left - upper_right + upper_left;
			max_num_blacks = max( max_num_blacks, num_blacks );	
		}
	}

	return max_num_blacks;
}

int Feature::maxNumBlackPixelsInBlock(const Mat & srcImg, const int thresh, const int block_size)
{
	Mat he_src;
	equalizeHist(srcImg, he_src);
	// Integral image -> speed up
	Mat num_black_mat(srcImg.size(), CV_32S, Scalar(0));
	for ( int rowIndex = 0; rowIndex < srcImg.rows; rowIndex++ ) {
		if ( (int)he_src.at<uchar>(rowIndex, 0) < thresh ) {
			num_black_mat.at<int>(rowIndex, 0) = 1;	
		}
		for ( int colIndex = 1; colIndex < srcImg.cols; colIndex++ ) {
			if ( (int)he_src.at<uchar>(rowIndex, colIndex) < thresh ) {
				num_black_mat.at<int>(rowIndex, colIndex) = num_black_mat.at<int>(rowIndex, colIndex - 1) + 1;
			}
			else {
				num_black_mat.at<int>(rowIndex, colIndex) = num_black_mat.at<int>(rowIndex, colIndex - 1);
			}
		}
	}
	for ( int colIndex = 0; colIndex < srcImg.cols; colIndex++ ) {
		for ( int rowIndex = 1; rowIndex < srcImg.rows; rowIndex++ ) {
			num_black_mat.at<int>(rowIndex, colIndex) = num_black_mat.at<int>(rowIndex-1, colIndex) + num_black_mat.at<int>(rowIndex, colIndex);
		}
	}

	int max_num_blacks = 0;
	for ( int i = 0; i <= srcImg.rows - block_size; i++ ) {
		for ( int j = 0; j <= srcImg.cols - block_size; j++ ) {
			int upper_left = getIntegralval(num_black_mat, i - 1, j - 1);
			int upper_right = getIntegralval(num_black_mat, i - 1, j + block_size - 1);
			int lower_left = getIntegralval(num_black_mat, i + block_size - 1, j - 1);
			int lower_right = getIntegralval(num_black_mat, i + block_size - 1, j + block_size - 1);	
			int num_blacks = lower_right - lower_left - upper_right + upper_left;
			max_num_blacks = max( max_num_blacks, num_blacks );	
		}
	}

	return max_num_blacks;
}

int Feature::maxNumDiffPixelsInBlock(const int thresh, const int block_size)
{

	Mat he_patch, he_ref;
	equalizeHist(_patch, he_patch);
	equalizeHist(_ref, he_ref);
	
	imshow("he_p", he_patch);
	imshow("he_r", he_ref);
	
	// Integral image -> speed up
	Mat num_diff_mat(_patch.size(), CV_32S, Scalar(0));
	for ( int rowIndex = 0; rowIndex < _patch.rows; rowIndex++ ) {
		if ( abs((int)he_patch.at<uchar>(rowIndex, 0) - (int)he_ref.at<uchar>(rowIndex, 0)) > thresh ) {
			num_diff_mat.at<int>(rowIndex, 0) = 1;	
		}
		for ( int colIndex = 1; colIndex < _patch.cols; colIndex++ ) {
			if ( abs((int)he_patch.at<uchar>(rowIndex, 0) - (int)he_ref.at<uchar>(rowIndex, 0)) > thresh ) {
				num_diff_mat.at<int>(rowIndex, colIndex) = num_diff_mat.at<int>(rowIndex, colIndex - 1) + 1;
			}
			else {
				num_diff_mat.at<int>(rowIndex, colIndex) = num_diff_mat.at<int>(rowIndex, colIndex - 1);
			}
		}
	}
	for ( int colIndex = 0; colIndex < _patch.cols; colIndex++ ) {
		for ( int rowIndex = 1; rowIndex < _patch.rows; rowIndex++ ) {
			num_diff_mat.at<int>(rowIndex, colIndex) = num_diff_mat.at<int>(rowIndex-1, colIndex) + num_diff_mat.at<int>(rowIndex, colIndex);
		}
	}

	int max_num_diffs = 0;
	for ( int i = 0; i <= _patch.rows - block_size; i++ ) {
		for ( int j = 0; j <= _patch.cols - block_size; j++ ) {
			int upper_left = getIntegralval(num_diff_mat, i - 1, j - 1);
			int upper_right = getIntegralval(num_diff_mat, i - 1, j + block_size - 1);
			int lower_left = getIntegralval(num_diff_mat, i + block_size - 1, j - 1);
			int lower_right = getIntegralval(num_diff_mat, i + block_size - 1, j + block_size - 1);	
			int num_diffs = lower_right - lower_left - upper_right + upper_left;
			max_num_diffs = max( max_num_diffs, num_diffs );	
		}
	}

	return max_num_diffs;
}

void Feature::trySIFT(const Mat & patch, const Mat & ref)
{

	
	// you will have to use cv::Ptr all the way down:	
	cv::Ptr<Feature2D> f2d = xfeatures2d::SIFT::create();
	//cv::Ptr<Feature2D> f2d = xfeatures2d::SURF::create();
	//cv::Ptr<Feature2D> f2d = ORB::create();
	// you get the picture, i hope..
	
	//-- Step 1: Detect the keypoints:
	std::vector<KeyPoint> keypoints_1, keypoints_2;    
	f2d->detect( patch, keypoints_1 );
	f2d->detect( ref, keypoints_2 );
	
	//-- Step 2: Calculate descriptors (feature vectors)    
	Mat descriptors_1, descriptors_2;    
	f2d->compute( patch, keypoints_1, descriptors_1 );
	f2d->compute( ref, keypoints_2, descriptors_2 );
	
	Mat img_keypoints1, img_keypoints2;
	drawKeypoints(patch, keypoints_1, img_keypoints1, 0, 4);
	drawKeypoints(ref, keypoints_2, img_keypoints2);
	imshow("keypoints", img_keypoints1);
	imshow("keypoints2", img_keypoints2);

	/*//-- Step 3: Matching descriptor vectors using BFMatcher :
	BFMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match( descriptors_1, descriptors_2, matches );
	Mat img_matches;
	drawMatches(patch, keypoints_1, ref, keypoints_2, matches, img_matches);
	imshow("match", img_matches);*/
	

	
}

int Feature::houghLine(const Mat & patch, const Mat & ref){
	int different=0;	
	//HoughLine Patch	
	Mat midImage,dstImage; 
	Canny(patch, midImage, 50, 200, 3);//Do Canny 
	cvtColor(midImage,dstImage, CV_GRAY2BGR);//Convert to Gray  
	//Do Houghline
	vector<Vec2f> lines;  
	HoughLines(midImage, lines, 1, CV_PI/180, 60, 0, 0 );  
	//drawLine
	for( size_t i = 0; i < lines.size(); i++ )  
	{  
	float rho = lines[i][0], theta = lines[i][1];  
	Point pt1, pt2;  
	double a = cos(theta), b = sin(theta);  
	double x0 = a*rho, y0 = b*rho;  
	pt1.x = cvRound(x0 + 1000*(-b));  
	pt1.y = cvRound(y0 + 1000*(a));  
	pt2.x = cvRound(x0 - 1000*(-b));  
	pt2.y = cvRound(y0 - 1000*(a));  
	line( dstImage, pt1, pt2, Scalar(55,100,195), 1, CV_AA);  
	}  
	//cout<<lines.size()<<endl;
	// show origin img    
	imshow("origin patch img", patch);    
	// canny img   
	imshow("houghline img", dstImage);  
	//HoughLine Ref
	Mat RefmidImage,RefdstImage;
	Canny(ref, RefmidImage, 50, 200, 3);//Do Canny 
	cvtColor(RefmidImage,RefdstImage, CV_GRAY2BGR);//Convert to Gray  
	//Do Houghline
	vector<Vec2f> Reflines;  
	HoughLines(RefmidImage, Reflines, 1, CV_PI/180, 60, 0, 0 );  
	//drawLine
	for( size_t i = 0; i < Reflines.size(); i++ )  
	{  
	float Refrho = Reflines[i][0], theta = Reflines[i][1];  
	Point Refpt1, Refpt2;  
	double Refa = cos(theta), Refb = sin(theta);  
	double Refx0 = Refa*Refrho, Refy0 = Refb*Refrho;  
	Refpt1.x = cvRound(Refx0 + 1000*(-Refb));  
	Refpt1.y = cvRound(Refy0 + 1000*(Refa));  
	Refpt2.x = cvRound(Refx0 - 1000*(-Refb));  
	Refpt2.y = cvRound(Refy0 - 1000*(Refa));  
	line( RefdstImage,Refpt1, Refpt2, Scalar(55,100,195), 1, CV_AA);  
	}  
	//cout<<Reflines.size()<<endl;
	// show origin img    
	imshow("origin ref img", ref);    
	// canny img   
	imshow("houghline ref img", RefdstImage);  
	different=abs(Reflines.size()-lines.size());
	return different;

}
