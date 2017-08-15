#include "feature.h"

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
	
	Alignment af;
	af.alignReplaceNotSoDifferent(better_patch, ref, color_gap);

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

int Feature::diffHoughLineCount(const Mat & patch, const Mat & ref, int min_val, int max_val, int threshold){
	Mat better_patch = patch.clone();	
	Alignment af;
	af.alignReplaceNotSoDifferent(better_patch, ref, 50);

	int different=0;	
	//HoughLine Patch	
	Mat midImage,dstImage;
	// Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
	Canny(better_patch, midImage, min_val, max_val, 3);//Do Canny 
	cvtColor(midImage,dstImage, CV_GRAY2BGR);//Convert to Gray 
	//Do Houghline
	vector<Vec2f> lines;  
	HoughLines(midImage, lines, 1, CV_PI/180, threshold, 0, 0 );  
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
	//imshow("origin patch img", better_patch);    
	// canny img   
	//imshow("houghline img", dstImage);  
	//HoughLine Ref
	Mat RefmidImage,RefdstImage;
	Canny(ref, RefmidImage, min_val, max_val, 3);//Do Canny 
	cvtColor(RefmidImage,RefdstImage, CV_GRAY2BGR);//Convert to Gray  
	//Do Houghline
	vector<Vec2f> Reflines;  
	HoughLines(RefmidImage, Reflines, 1, CV_PI/180, threshold, 0, 0 );  
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
	//imshow("origin ref img", ref);    
	// canny img   
	//imshow("houghline ref img", RefdstImage);
	//cout << Reflines.size() << ' ' << lines.size() << endl;
	int d = Reflines.size()-lines.size();
	//cout << d << ' ' << abs(d) << ' ' << abs((Reflines.size()-lines.size())) <<  endl;
	different = abs( d );

	return different;
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
	
	int cnt = 0;
	for ( int i = 0; i < v.size(); i++ ) {
		if ( v[i] - statistics.min <= 2 ) {
			cnt++;
		}
	}
	statistics.diffPatchCount = cnt;

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

	//printVector(local_psnrs);
	//Mat hist = showHistograms(local_psnrs);
	//imshow("hist", hist);
	
	Statistics sta = calStatistics<double>(local_psnrs);
	//showStatistics(sta);
	return calStatistics<double>(local_psnrs);
}

Mat Feature::calPSNRs(const Mat & patch, const int pool_size)
{
	vector<Mat> slices_patch = sliceImg(patch, pool_size);
	
	Mat_<double> psnrs(slices_patch.size(), slices_patch.size());
 
	for ( int i = 0; i < slices_patch.size(); i++ ) {
		for ( int j = i; j < slices_patch.size(); j++ ) {
			psnrs.at<double>(i, j) = getPSNR(slices_patch[i], slices_patch[j]);
		}
	}
	return psnrs;
}
vector<Statistics> calStatisticsVector(Mat_<double> mat)
{
	vector<Statistics> sta;
	for ( int i = 0; i < mat.rows; i++ ) {
		vector<double> v;
		for ( int j = 0; j < mat.cols; j++ ) {
			if (i < j) {
				v.push_back(mat.at<double>(i, j));
			}
			if (i > j) {
				v.push_back(mat.at<double>(j, i));
			}
		}
		//printVector(v);
		sta.push_back(calStatistics<double>(v));
		//cout << i << "  ";
		//showStatistics(sta[i]);
	}
	return sta;
}

double Feature::lowestMaxLocalPSNR(const Mat & patch, const int pool_size)
{
	Mat psnrs = calPSNRs(patch, pool_size);
	vector<Statistics> sta = calStatisticsVector(psnrs);

	
	double min_maxpsnr = (*std::min_element(sta.begin(), sta.end(), [](Statistics a, Statistics b) { return a.max < b.max; })).max;

	return min_maxpsnr;
}

int Feature::mostDifferentPatchCount(const Mat & patch, const int pool_size)
{
	Mat psnrs = calPSNRs(patch, pool_size);
	
	vector<Statistics> sta = calStatisticsVector(psnrs);
	
	double min_maxpsnr = (*std::min_element(sta.begin(), sta.end(), [](Statistics a, Statistics b) { return a.max < b.max; })).max;
	int cnt = 0;
	for ( int i = 0; i < sta.size(); i++ ) {
		if ( sta[i].max - min_maxpsnr <= 2 ) {
			cnt++;
		}
	}	

	return cnt;
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

int Feature::maxNumDiffPixelsInBlock(const Mat & patch, const Mat & ref, const int thresh, const int block_size)
{

	Mat he_patch, he_ref;
	equalizeHist(patch, he_patch);
	equalizeHist(ref, he_ref);
	
	//imshow("he_p", he_patch);
	//imshow("he_r", he_ref);
	//waitKey();
	
	// Integral image -> speed up
	Mat num_diff_mat(patch.size(), CV_32S, Scalar(0));
	for ( int rowIndex = 0; rowIndex < patch.rows; rowIndex++ ) {
		if ( abs((int)he_patch.at<uchar>(rowIndex, 0) - (int)he_ref.at<uchar>(rowIndex, 0)) > thresh ) {
			num_diff_mat.at<int>(rowIndex, 0) = 1;	
		}
		for ( int colIndex = 1; colIndex < patch.cols; colIndex++ ) {
			if ( abs((int)he_patch.at<uchar>(rowIndex, 0) - (int)he_ref.at<uchar>(rowIndex, 0)) > thresh ) {
				num_diff_mat.at<int>(rowIndex, colIndex) = num_diff_mat.at<int>(rowIndex, colIndex - 1) + 1;
			}
			else {
				num_diff_mat.at<int>(rowIndex, colIndex) = num_diff_mat.at<int>(rowIndex, colIndex - 1);
			}
		}
	}
	for ( int colIndex = 0; colIndex < patch.cols; colIndex++ ) {
		for ( int rowIndex = 1; rowIndex < patch.rows; rowIndex++ ) {
			num_diff_mat.at<int>(rowIndex, colIndex) = num_diff_mat.at<int>(rowIndex-1, colIndex) + num_diff_mat.at<int>(rowIndex, colIndex);
		}
	}

	int max_num_diffs = 0;
	for ( int i = 0; i <= patch.rows - block_size; i++ ) {
		for ( int j = 0; j <= patch.cols - block_size; j++ ) {
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

void Feature::trySuperPixel(const Mat & patch, const Mat & ref)
{
	imshow("p", patch);
	imshow("r", ref);

	Mat frame, labels, mask;

	frame = patch.clone();

	Ptr<cv::ximgproc::SuperpixelSEEDS> seeds = cv::ximgproc::createSuperpixelSEEDS(frame.cols, frame.rows, frame.channels(), 70, 5, 5, 3, true);

        seeds->iterate(frame);//迭代次数，默认为4
        seeds->getLabels(labels);//获取labels
        seeds->getLabelContourMask(mask);;//获取超像素的边界
        int number_seeds = seeds->getNumberOfSuperpixels();//获取超像素的数量

	cout << "SuperPixel -- number_seeds_patch: " << number_seeds << endl;

	//labels.convertTo(labels, CV_32F);
	//normalize(labels, labels, 1, 0, NORM_MINMAX);	
	//imshow("labes", labels);
	imshow("mask_p", mask);
	frame.setTo(Scalar(255, 255, 255), mask);
	imshow("sp_p", frame);
	imwrite("sp_p.png", frame);

	frame = ref.clone();

	seeds = cv::ximgproc::createSuperpixelSEEDS(frame.cols, frame.rows, frame.channels(), 70, 5, 5, 3, true);

        seeds->iterate(frame);//迭代次数，默认为4
        seeds->getLabels(labels);//获取labels
        seeds->getLabelContourMask(mask);;//获取超像素的边界
        number_seeds = seeds->getNumberOfSuperpixels();//获取超像素的数量

	cout << "SuperPixel -- number_seeds_ref: " << number_seeds << endl;

	//labels.convertTo(labels, CV_32F);
	//normalize(labels, labels, 1, 0, NORM_MINMAX);	
	//imshow("labes", labels);
	imshow("mask_r", mask);
	frame.setTo(Scalar(255, 255, 255), mask);
	imshow("sp_r", frame);
	imwrite("sp_r.png", frame);


/*
	vector<int> nums(number_seeds, 0);
	Mat_<double> means(1, number_seeds);
	for ( int rowIndex = 0; rowIndex < labels.rows; rowIndex++ ) {
		for ( int colIndex = 0; colIndex < labels.cols; colIndex++ ) {
			nums[ labels.at<int>(rowIndex, colIndex) ]++;
			means.at<double>(0, labels.at<int>(rowIndex, colIndex) ) += (double)frame.at<uchar>(rowIndex, colIndex);
		}
	}
	for ( int i = 0; i < number_seeds; i++ ) {
		if ( nums[i] == 0 ) {
			means.at<double>(0, i) = 0;
		}
		else {
			means.at<double>(0, i) /= nums[i];
		}
		//cout << nums[i] << ' ';
		//cout << means[i] << ' ';
		//cout << means[i] << ' ';
	}
	Mat means_img;
	means.convertTo(means_img, CV_8U);
	Mat hist = getHistImg(means_img);
	imshow("hist", hist);
	imwrite("hist_sp_r.png", hist);
*/
}

void Feature::trySaliencyMap(const Mat & patch, const Mat & ref)
{
	string saliency_algorithm = "SPECTRAL_RESIDUAL";
	Ptr<Saliency> saliencyAlgorithm = Saliency::create( saliency_algorithm );
	if( saliencyAlgorithm == NULL ) {
		cout << "***Error in the instantiation of the saliency algorithm...***\n";
		return;
	}
	Mat binaryMap;
	Mat img = patch.clone();
	Mat img2 = ref.clone();

	if( saliency_algorithm.find("SPECTRAL_RESIDUAL") == 0 ) {
		Mat saliencyMap;
		if( saliencyAlgorithm->computeSaliency( img, saliencyMap ) ) {
			StaticSaliencySpectralResidual spec;
      			spec.computeBinaryMap( saliencyMap, binaryMap );

      			imshow( "Saliency Map", saliencyMap );
			imshow( "Original Image", img );
      			imshow( "Binary Map", binaryMap );
			imwrite( "sm_p.png", binaryMap);
      			//waitKey( 0 );
		}
		
		if( saliencyAlgorithm->computeSaliency( img2, saliencyMap ) ) {
			StaticSaliencySpectralResidual spec;
      			spec.computeBinaryMap( saliencyMap, binaryMap );

      			imshow( "Saliency Map_r", saliencyMap );
			imshow( "Original Image_r", img2 );
      			imshow( "Binary Map_r", binaryMap );
			imwrite( "sm_r.png", binaryMap);
      			//waitKey( 0 );
		}
	}
	
}

void Feature::trySIFT(const Mat & patch, const Mat & ref)
{
	vector<Mat> slices_patch = sliceImg(patch, 64);

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
	drawKeypoints(patch, keypoints_1, img_keypoints1);
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
	
	for ( int i = 0; i < slices_patch.size(); i++ ) {
		std::vector<KeyPoint> k;
		Mat d;
		f2d->detect( slices_patch[i], k, d );
		Mat img_k;
		drawKeypoints(slices_patch[i], k, img_k);
		string s = std::to_string(i);
		imshow(s, img_k);
	}
	
}
//diffGlobalMean
float Feature::FE5000(const Mat & patch, const Mat & ref){
	return diffGlobalMean(patch,ref);
}
 
int Feature::FE5100(const Mat & patch, const Mat & ref){
	return diffPixelsCount(patch,ref,70);
}
int Feature::FE5101(const Mat & patch, const Mat & ref){
	return diffPixelsCount(patch,ref,80);
}
int Feature::FE5102(const Mat & patch, const Mat & ref){
	return diffPixelsCount(patch,ref,90);
}
int Feature::FE5103(const Mat & patch, const Mat & ref){
	return diffPixelsCount(patch,ref,100);
}
int Feature::FE5104(const Mat & patch, const Mat & ref){
	return diffPixelsCount(patch,ref,120);
}

//diffHoughLineCount
int Feature::FE5110(const Mat & patch, const Mat & ref){
	return diffHoughLineCount(patch, ref, 50, 150, 62);
}

//getPSNR
double Feature::FE5200(const Mat & patch, const Mat & ref){
	return getPSNR(patch,ref);
}

double Feature::FE5210(const Mat & patch, const Mat & ref){
	return localPSNR(patch, ref, 20).min;
}
double Feature::FE5211(const Mat & patch, const Mat & ref){
	return localPSNR(patch, ref, 24).min;
}
double Feature::FE5212(const Mat & patch, const Mat & ref){
	return localPSNR(patch, ref, 32).min;
}
//localPSNR.max
double Feature::FE5220(const Mat & patch, const Mat & ref){
	return localPSNR(patch, ref, 20).max;
}
double Feature::FE5221(const Mat & patch, const Mat & ref){
	return localPSNR(patch, ref, 24).max;
}
double Feature::FE5222(const Mat & patch, const Mat & ref){
	return localPSNR(patch, ref, 32).max;
}

//localPSNR.mean
double Feature::FE5230(const Mat & patch, const Mat & ref){
	return localPSNR(patch, ref, 20).mean;
}
double Feature::FE5231(const Mat & patch, const Mat & ref){
	return localPSNR(patch, ref, 24).mean;
}
double Feature::FE5232(const Mat & patch, const Mat & ref){
	return localPSNR(patch, ref, 32).mean;
}

//localPSNR.stdev
double Feature::FE5240(const Mat & patch, const Mat & ref){
	return localPSNR(patch, ref, 20).stdev;
}
double Feature::FE5241(const Mat & patch, const Mat & ref){
	return localPSNR(patch, ref, 24).stdev;
}
double Feature::FE5242(const Mat & patch, const Mat & ref){
	return localPSNR(patch, ref, 32).stdev;
}

//localPSNR.var
double Feature::FE5250(const Mat & patch, const Mat & ref){
	return localPSNR(patch, ref, 20).var;
}
double Feature::FE5251(const Mat & patch, const Mat & ref){
	return localPSNR(patch, ref, 24).var;
}
double Feature::FE5252(const Mat & patch, const Mat & ref){
	return localPSNR(patch, ref, 32).var;
}

//localmeanSSE
double Feature::FE5300(const Mat & patch, const Mat & ref){
	return localmeanSSE(patch,ref,16);
}

double Feature::FE5301(const Mat & patch, const Mat & ref){
	return localmeanSSE(patch,ref,20);
}

double Feature::FE5302(const Mat & patch, const Mat & ref){
	return localmeanSSE(patch,ref,24);
}

double Feature::FE5303(const Mat & patch, const Mat & ref){
	return localmeanSSE(patch,ref,25);
}

double Feature::FE5304(const Mat & patch, const Mat & ref){
	return localmeanSSE(patch,ref,28);
}

double Feature::FE5305(const Mat & patch, const Mat & ref){
	return localmeanSSE(patch,ref,32);
}

//localmeanSAE
double Feature::FE5310(const Mat & patch, const Mat & ref){
	return localmeanSAE(patch,ref,16);
}

double Feature::FE5311(const Mat & patch, const Mat & ref){
	return localmeanSAE(patch,ref,20);
}

double Feature::FE5312(const Mat & patch, const Mat & ref){
	return localmeanSAE(patch,ref,24);
}

double Feature::FE5313(const Mat & patch, const Mat & ref){
	return localmeanSAE(patch,ref,25);
}

double Feature::FE5314(const Mat & patch, const Mat & ref){
	return localmeanSAE(patch,ref,28);
}

double Feature::FE5315(const Mat & patch, const Mat & ref){
	return localmeanSAE(patch,ref,32);
}

//maxNumBlackPixelsInBlock
int Feature::FE5400(const Mat & patch, const Mat & ref){
	return maxNumBlackPixelsInBlock(80, 16);
}
int Feature::FE5401(const Mat & patch, const Mat & ref){
	return maxNumBlackPixelsInBlock(80, 20);
}
int Feature::FE5402(const Mat & patch, const Mat & ref){
	return maxNumBlackPixelsInBlock(80, 24);
}
int Feature::FE5403(const Mat & patch, const Mat & ref){
	return maxNumBlackPixelsInBlock(80, 25);
}
int Feature::FE5404(const Mat & patch, const Mat & ref){
	return maxNumBlackPixelsInBlock(80, 28);
}
int Feature::FE5405(const Mat & patch, const Mat & ref){
	return maxNumBlackPixelsInBlock(80, 32);
}
//maxNumDiffPixelsInBlock
int Feature::FE5410(const Mat & patch, const Mat & ref){
	return maxNumDiffPixelsInBlock(patch, ref, 30, 16);
}
int Feature::FE5411(const Mat & patch, const Mat & ref){
	return maxNumDiffPixelsInBlock(patch, ref, 30, 20);
}
int Feature::FE5412(const Mat & patch, const Mat & ref){
	return maxNumDiffPixelsInBlock(patch, ref, 30, 24);
}
int Feature::FE5413(const Mat & patch, const Mat & ref){
	return maxNumDiffPixelsInBlock(patch, ref, 30, 25);
}
int Feature::FE5414(const Mat & patch, const Mat & ref){
	return maxNumDiffPixelsInBlock(patch, ref, 30, 28);
}
int Feature::FE5415(const Mat & patch, const Mat & ref){
	return maxNumDiffPixelsInBlock(patch, ref, 30, 32);
}
int Feature::FE5416(const Mat & patch, const Mat & ref){
	return maxNumDiffPixelsInBlock(patch, ref, 40, 16);
}
int Feature::FE5417(const Mat & patch, const Mat & ref){
	return maxNumDiffPixelsInBlock(patch, ref, 40, 20);
}
int Feature::FE5418(const Mat & patch, const Mat & ref){
	return maxNumDiffPixelsInBlock(patch, ref, 40, 24);
}
int Feature::FE5419(const Mat & patch, const Mat & ref){
	return maxNumDiffPixelsInBlock(patch, ref, 40, 25);
}
//Superpixels
/*void Feature::FE5500(Mat & patch, Mat & ref){
	trySuperPixel(patch,ref);
}*/


