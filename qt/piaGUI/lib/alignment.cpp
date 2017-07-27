#include "../include/alignment.h"

Alignment::Alignment() {

}

Alignment::Alignment(Mat & target, Mat & ref) {
	_patch = target;
	_ref = ref;
}

// DCT
void Alignment::alignDCT(Mat &patch, Mat &ref){
	
	Mat fpatch, fref;
	patch.convertTo(fpatch, CV_32F);
	ref.convertTo(fref, CV_32F);
	Mat dct_patch, dct_ref;
	dct(fpatch, dct_patch);
	dct(fref, dct_ref);

	//Mat show_dct_patch, show_dct_ref;
	//dct_patch.convertTo(show_dct_patch, CV_8U);
	//dct_ref.convertTo(show_dct_ref, CV_8U);
	//imshow("dct_p", show_dct_patch);
	//imshow("dct_r", show_dct_ref);
	
	for ( int i = 0; i < dct_patch.rows; i++ ) {
		for ( int j = 0; j < dct_patch.cols; j++ ) {
			if ( ! (i > 0 &&  j > 0) ) {
				if ( i == 0 && j == 0 )
					dct_patch.at<float>(i, j) = dct_ref.at<float>(i, j);
				else 
					dct_patch.at<float>(i, j) = ( dct_ref.at<float>(i, j) + dct_patch.at<float>(i, j) ) / 2;
			}
		}
	}
	//dct_patch.at<float>(0, 0) = dct_ref.at<float>(0, 0);
	
	idct(dct_patch, dct_patch, DCT_INVERSE);
	idct(dct_ref, dct_ref, DCT_INVERSE);
	dct_patch.convertTo(patch, CV_8U);
	dct_ref.convertTo(ref, CV_8U);
	//imshow("idctp", patch);
	//imshow("idctr", ref);
}

// DFT
void Alignment::alignDFT(Mat &patch, Mat &ref) {
	Mat padded_patch, padded_ref;
	int m = getOptimalDFTSize(ref.rows);
	int n = getOptimalDFTSize(ref.cols);
	copyMakeBorder( patch, padded_patch, 0, m - patch.rows, 0, n - patch.cols, BORDER_CONSTANT, Scalar::all(0) );
	copyMakeBorder( ref, padded_ref, 0, m - ref.rows, 0,  n - ref.cols, BORDER_CONSTANT, Scalar::all(0) );

	Mat planesp[] = { Mat_<float>(padded_patch), Mat::zeros(padded_patch.size(), CV_32F) };
	Mat planesr[] = { Mat_<float>(padded_ref), Mat::zeros(padded_ref.size(), CV_32F) };
	Mat complexP, complexR;
	merge(planesp, 2, complexP);
	merge(planesr, 2, complexR);

	dft(complexP, complexP);
	dft(complexR, complexR);
	
	for ( int i = 0; i < complexP.rows; i++ ) {
		for ( int j = 0; j < complexP.cols; j++ ) {
			if ( ! (i > 0 &&  j > 0) ) {
				complexP.at<float>(i, j) = ( complexR.at<float>(i, j) + complexP.at<float>(i, j) / 2);
			}
		}
	}

	Mat idftp, idftr;
	dft(complexP, idftp, DFT_INVERSE|DFT_REAL_OUTPUT);
	dft(complexR, idftr, DFT_INVERSE|DFT_REAL_OUTPUT);
	normalize(idftp, idftp, 0, 1, CV_MINMAX);
	normalize(idftr, idftr, 0, 1, CV_MINMAX);
	//imshow("idftp", idftp);
	//imshow("idftr", idftr);
}

// Histogram

void Alignment::histogramEqualization(Mat & patch, Mat & ref)
{
	equalizeHist(patch, patch);
	equalizeHist(ref, ref);
}

/// Thresholding

void Alignment::threshOtsu(Mat &patch, Mat &ref, double thresh, double max_value)
{	
	threshold(patch, patch, thresh, max_value, THRESH_OTSU);
	threshold(ref, ref, thresh, max_value, THRESH_OTSU);
}

void Alignment::threshBinary(Mat &patch,Mat &ref, double thresh, double max_value)
{	
	threshold(patch, patch, thresh, max_value, THRESH_BINARY);
	threshold(ref, ref, thresh, max_value, THRESH_BINARY);
}

void Alignment::threshBinaryINV(Mat &patch,Mat &ref, double thresh, double max_value)
{	
	threshold(patch, patch, thresh, max_value, THRESH_BINARY_INV);
	threshold(ref, ref, thresh, max_value, THRESH_BINARY_INV);
}

void Alignment::threshTrunc(Mat &patch,Mat &ref, double thresh, double max_value)
{	
	threshold(patch, patch, thresh, max_value, THRESH_TRUNC);
	threshold(ref, ref, thresh, max_value, THRESH_TRUNC);
}

void Alignment::threshToZero(Mat &patch,Mat &ref, double thresh, double max_value)
{	
	threshold(patch, patch, thresh, max_value, THRESH_TOZERO);
	threshold(ref, ref, thresh, max_value, THRESH_TOZERO);
}

void Alignment::threshToZeroINV(Mat &patch,Mat &ref, double thresh, double max_value)
{	
	threshold(patch, patch, thresh, max_value, THRESH_TOZERO_INV);
	threshold(ref, ref, thresh, max_value, THRESH_TOZERO_INV);
}



void alignECC(Mat & patch, Mat & ref, const int warp_mode)
{
	// Define the motion model
	//const int warp_mode = MOTION_EUCLIDEAN;
 
	// Set a 2x3 or 3x3 warp matrix depending on the motion model.
	Mat warp_matrix;
 
	// Initialize the matrix to identity
	if ( warp_mode == MOTION_HOMOGRAPHY )
	    warp_matrix = Mat::eye(3, 3, CV_32F);
	else
	    warp_matrix = Mat::eye(2, 3, CV_32F);
	 
	// Specify the number of iterations.
	int number_of_iterations = 5000;
	 
	// Specify the threshold of the increment
	// in the correlation coefficient between two iterations
	double termination_eps = 1e-10;
	 
	// Define termination criteria
	TermCriteria criteria (TermCriteria::COUNT+TermCriteria::EPS, number_of_iterations, termination_eps);
	 
	// Run the ECC algorithm. The results are stored in warp_matrix.
	findTransformECC(
		         ref,
		         patch,
		         warp_matrix,
		         warp_mode,
		         criteria
		     );

	Mat patch_aligned;

	if (warp_mode != MOTION_HOMOGRAPHY)
		warpAffine (patch, patch_aligned, warp_matrix, ref.size(), INTER_LINEAR + WARP_INVERSE_MAP);
	else 
		warpPerspective (patch, patch_aligned, warp_matrix, ref.size(), INTER_LINEAR + WARP_INVERSE_MAP);

	//imshow("Ref", ref);
	//imshow("aligned ECC", patch_aligned);
	imwrite("aligned_ecc.png", patch_aligned);
}

void Alignment::alignECC_Translation(Mat & patch, Mat & ref)
{
	alignECC(patch, ref, MOTION_TRANSLATION);
}

void Alignment::alignECC_Euclidean(Mat & patch, Mat & ref)
{
	alignECC(patch, ref, MOTION_EUCLIDEAN);
}

void Alignment::alignECC_Affine(Mat & patch, Mat & ref)
{
	alignECC(patch, ref, MOTION_AFFINE);
}

void Alignment::alignECC_Homography(Mat & patch, Mat & ref)
{
	alignECC(patch, ref, MOTION_HOMOGRAPHY);
}

void Alignment::align()
{
	histogramEqualization(_patch, _ref);

	//Mat grad_patch = getGradient(_patch);
	//Mat grad_ref = getGradient(_ref);

	//imshow("gp", grad_patch);
	//imshow("gr", grad_ref);

	alignECC_Translation(_patch, _ref);
}
