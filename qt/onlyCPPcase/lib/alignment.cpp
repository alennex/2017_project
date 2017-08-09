#include "alignment.h"

Alignment::Alignment() {
	
	using namespace std::placeholders;
	icmap00 = {
		{ "IC0000", std::bind( &Alignment::IC0000, this, _1, _2, _3, _4) },
		{ "IC0001", std::bind( &Alignment::IC0001, this, _1, _2, _3, _4) },
		{ "IC0002", std::bind( &Alignment::IC0002, this, _1, _2, _3, _4) },
		{ "IC0003", std::bind( &Alignment::IC0003, this, _1, _2, _3, _4) },
		{ "IC0010", std::bind( &Alignment::IC0010, this, _1, _2, _3, _4) },
		{ "IC0011", std::bind( &Alignment::IC0011, this, _1, _2, _3, _4) },
		{ "IC0012", std::bind( &Alignment::IC0012, this, _1, _2, _3, _4) },
		{ "IC0013", std::bind( &Alignment::IC0013, this, _1, _2, _3, _4) },
		{ "IC0020", std::bind( &Alignment::IC0020, this, _1, _2, _3, _4) },
		{ "IC0021", std::bind( &Alignment::IC0021, this, _1, _2, _3, _4) },
		{ "IC0022", std::bind( &Alignment::IC0022, this, _1, _2, _3, _4) },
		{ "IC0023", std::bind( &Alignment::IC0023, this, _1, _2, _3, _4) },
		{ "IC0030", std::bind( &Alignment::IC0030, this, _1, _2, _3, _4) },
		{ "IC0031", std::bind( &Alignment::IC0031, this, _1, _2, _3, _4) },
		{ "IC0032", std::bind( &Alignment::IC0032, this, _1, _2, _3, _4) },
		{ "IC0033", std::bind( &Alignment::IC0033, this, _1, _2, _3, _4) },
		{ "IC0040", std::bind( &Alignment::IC0040, this, _1, _2, _3, _4) },
		{ "IC0041", std::bind( &Alignment::IC0041, this, _1, _2, _3, _4) },
		{ "IC0042", std::bind( &Alignment::IC0042, this, _1, _2, _3, _4) },
		{ "IC0043", std::bind( &Alignment::IC0043, this, _1, _2, _3, _4) },
		{ "IC0050", std::bind( &Alignment::IC0050, this, _1, _2, _3, _4) },
		{ "IC0051", std::bind( &Alignment::IC0051, this, _1, _2, _3, _4) },
		{ "IC0052", std::bind( &Alignment::IC0052, this, _1, _2, _3, _4) },
		{ "IC0053", std::bind( &Alignment::IC0053, this, _1, _2, _3, _4) },
		{ "IC0060", std::bind( &Alignment::IC0060, this, _1, _2, _3, _4) },
		{ "IC0061", std::bind( &Alignment::IC0061, this, _1, _2, _3, _4) },
		{ "IC0062", std::bind( &Alignment::IC0062, this, _1, _2, _3, _4) },
		{ "IC0053", std::bind( &Alignment::IC0053, this, _1, _2, _3, _4) }
	};
	//icmap00.insert(std::make_pair("IC0000", std::bind( &Alignment::IC0000, this, _1, _2, _3, _4)));
	icmap = {
		{ "IC0100", std::bind( &Alignment::IC0100, this, _1, _2) },
		{ "IC0110", std::bind( &Alignment::IC0110, this, _1, _2) },
		{ "IC0111", std::bind( &Alignment::IC0111, this, _1, _2) },
		{ "IC0112", std::bind( &Alignment::IC0112, this, _1, _2) },
		{ "IC0200", std::bind( &Alignment::IC0200, this, _1, _2) },
		{ "IC0210", std::bind( &Alignment::IC0210, this, _1, _2) },
		{ "IC0300", std::bind( &Alignment::IC0300, this, _1, _2) },
		{ "IC0310", std::bind( &Alignment::IC0310, this, _1, _2) },
		{ "IC0400", std::bind( &Alignment::IC0400, this, _1, _2) },
		{ "IC0410", std::bind( &Alignment::IC0410, this, _1, _2) },
	};
}

Alignment::Alignment(Mat & target, Mat & ref) {
	_patch = target;
	_ref = ref;

	Alignment();
}

// DCT
void Alignment::alignDCTTopRLeftCAvg(Mat &patch, Mat &ref){
	
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

// DCT
void Alignment::alignDCTZigZag10(Mat &patch, Mat &ref){
	
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
	
	for (int i = 0; i < dct_patch.rows; i++)    
	{    
		for (int j = 10; j < dct_patch.cols; j++)    
		{    
			dct_patch.at<float>(i, j) = dct_ref.at<float>(i, j); 
		}    
	}  
	for (int i = 10; i < dct_patch.rows; i++)    
	{    
		for (int j = 0; j < dct_patch.cols; j++)    
		{    
			dct_patch.at<float>(i, j) = dct_ref.at<float>(i, j); 
		}    
	} 

	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++){
			if((i+j)==10){	
				for(int k=j;k<=10;k++){
					dct_patch.at<float>(i, k) = dct_ref.at<float>(i, k); 
				}			
			}
		}	
	}

	
	dct_patch.at<float>(0, 0) = dct_ref.at<float>(0, 0);
	
	idct(dct_patch, dct_patch, DCT_INVERSE);
	idct(dct_ref, dct_ref, DCT_INVERSE);
	dct_patch.convertTo(patch, CV_8U);
	dct_ref.convertTo(ref, CV_8U);
	//imwrite("zigzag10.png", patch);
	//imshow("idctp", patch);
	//imshow("idctr", ref);
}

// DCT
void Alignment::alignDCTKeepTenPixels(Mat &patch, Mat &ref){
	
	Mat fpatch, fref;
	patch.convertTo(fpatch, CV_32F);
	ref.convertTo(fref, CV_32F);
	Mat dct_patch, dct_ref;
	dct(fpatch, dct_patch);
	dct(fref, dct_ref);

	//Mat show_dct_patch, show_dct_ref;
	//dct_patch.convertTo(show_dct_patch, CV_8U
	//imshow("dct_p", show_dct_patch);
	//imshow("dct_r", show_dct_ref);
	
	for (int i = 0; i < dct_patch.rows; i++)    
	{    
		for (int j = 10; j < dct_patch.cols; j++)    
		{    
			dct_patch.at<float>(i, j) = dct_ref.at<float>(i, j); 
		}    
	}  
	for (int i = 10; i < dct_patch.rows; i++)    
	{    
		for (int j = 0; j < dct_patch.cols; j++)    
		{    
			dct_patch.at<float>(i, j) = dct_ref.at<float>(i, j); 
		}    
	} 
	dct_patch.at<float>(0, 0) = dct_ref.at<float>(0, 0);
	
	idct(dct_patch, dct_patch, DCT_INVERSE);
	idct(dct_ref, dct_ref, DCT_INVERSE);
	dct_patch.convertTo(patch, CV_8U);
	dct_ref.convertTo(ref, CV_8U);
	//imshow("idctp", patch);
	//imshow("idctr", ref);
}

// DFT
void Alignment::alignDFTKeepTenPixels(Mat &patch, Mat &ref) {
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
	
	
	for (int i = 0; i < complexP.rows; i++)    
	{    
		for (int j = 10; j < complexP.cols; j++)    
		{    
			complexP.at<float>(i, j) = complexR.at<float>(i, j); 
		}    
	}  
	for (int i = 10; i < complexP.rows; i++)    
	{    
		for (int j = 0; j < complexP.cols; j++)    
		{    
			complexP.at<float>(i, j) = complexR.at<float>(i, j); 
		}    
	} 
	complexP.at<float>(0, 0) = complexR.at<float>(0, 0);

	Mat idftp, idftr;
	dft(complexP, idftp, DFT_INVERSE|DFT_REAL_OUTPUT);
	dft(complexR, idftr, DFT_INVERSE|DFT_REAL_OUTPUT);
	normalize(idftp, idftp, 0, 1, CV_MINMAX);
	normalize(idftr, idftr, 0, 1, CV_MINMAX);
	//imshow("idftp", idftp);
	//imshow("idftr", idftr);
}

// patch and ref should be the same size
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

	imshow("idftp", idftp);
	imshow("idftr", idftr);
}

void Alignment::alignReplaceNotSoDifferent(Mat &patch, const Mat & ref, const int color_gap)
{
	for ( int rowIndex = 0; rowIndex < ref.rows; rowIndex++ ) {
		for ( int colIndex = 0; colIndex < ref.cols; colIndex++) {
			if ( abs( (int)patch.at<uchar>(rowIndex, colIndex)-(int)ref.at<uchar>(rowIndex, colIndex)) < color_gap ) {
				patch.at<uchar>(rowIndex, colIndex) = ref.at<uchar>(rowIndex, colIndex);
			}
		}
	}
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



int alignECC(Mat & patch, Mat & ref, const int warp_mode, int number_of_iterations, double termination_eps)
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
	//int number_of_iterations = 5000;
	 
	// Specify the threshold of the increment
	// in the correlation coefficient between two iterations
	//double termination_eps = 1e-10;
	 
	// Define termination criteria
	TermCriteria criteria (TermCriteria::COUNT+TermCriteria::EPS, number_of_iterations, termination_eps);
	
	try
	{
		findTransformECC(
			         ref,
		        	 patch,
			         warp_matrix,
			         warp_mode,
			         criteria
			     );

	}
	catch ( cv::Exception& e )
	{
		//const char* err_msg = e.what();
		//std:cout << "exception caught: " << err_msg << std::endl;

		cout << "Discard." << endl;
		return -1;
	}

	//cout << "#it: " << number_of_iterations << endl;

	// Run the ECC algorithm. The results are stored in warp_matrix.
	/*double tmp = findTransformECC(
		         ref,
		         patch,
		         warp_matrix,
		         warp_mode,
		         criteria
		     );

	cout << tmp << ' ';*/

	Mat patch_aligned;

	if (warp_mode != MOTION_HOMOGRAPHY)
		warpAffine (patch, patch_aligned, warp_matrix, ref.size(), INTER_LINEAR + WARP_INVERSE_MAP);
	else 
		warpPerspective (patch, patch_aligned, warp_matrix, ref.size(), INTER_LINEAR + WARP_INVERSE_MAP);

	//imshow("Patch", patch);
	//imshow("Ref", ref);
	//imshow("aligned ECC", patch_aligned);
	//waitKey(0);
	//imwrite("aligned_ecc.png", patch_aligned);
	
	return 0;
}

int Alignment::alignECC_Translation(Mat & patch, Mat & ref, int num_of_iterations, double termination_eps)
{	
	return alignECC(patch, ref, MOTION_TRANSLATION, num_of_iterations, termination_eps);
}

int Alignment::alignECC_Euclidean(Mat & patch, Mat & ref, int num_of_iterations, double termination_eps)
{
	return alignECC(patch, ref, MOTION_EUCLIDEAN, num_of_iterations, termination_eps);
}

int Alignment::alignECC_Affine(Mat & patch, Mat & ref, int num_of_iterations, double termination_eps)
{
	return alignECC(patch, ref, MOTION_AFFINE, num_of_iterations, termination_eps);
}

int Alignment::alignECC_Homography(Mat & patch, Mat & ref, int num_of_iterations, double termination_eps)
{
	return alignECC(patch, ref, MOTION_HOMOGRAPHY, num_of_iterations, termination_eps);
}

int Alignment::align()
{
	//histogramEqualization(_patch, _ref);

	//Mat grad_patch = getGradient(_patch);
	//Mat grad_ref = getGradient(_ref);

	//imshow("gp", grad_patch);
	//imshow("gr", grad_ref);

	//alignECC_Translation(_patch, _ref);
	return alignECC_Translation(_patch, _ref, 5000, 1e-10);
	//alignECC_Homography(_patch, _ref);
}


void Alignment::ICFilter(string ic_method, double keep_rate, const char csv_path[], string &out_csv_path, int & num_kept_defect, int & num_kept_nuisance, bool discard)
{
	cv::Size size = readNumOfLines(csv_path);
	int num_of_lines = size.height;
	int num_of_attributes = size.width;
	string ** input_csv_table = readcsv(csv_path, num_of_lines, num_of_attributes);

	string left_folder = "left128";
	string right_folder = "right128";
	createFolders(ic_method, left_folder, right_folder);

	int def_image = num_of_attributes - 2;
	int ref_image = num_of_attributes - 1;
	int type = num_of_attributes - 3;
	for ( int i = 0; i < num_of_attributes; i++ ) {
		if (input_csv_table[0][i] == "\"DEF_IMAGE\"")	def_image = i;
		if (input_csv_table[0][i] == "\"REF_IMAGE\"")	ref_image = i;
		if (input_csv_table[0][i] == "\"Type\"") 	type = i;
	}
	
	string** result = new string*[num_of_lines];
	for (int i = 0; i < num_of_lines; i++) {
		result[i] = new string[num_of_attributes + 1];
		for (int j = 0; j < num_of_attributes; j++) {
			result[i][j] = input_csv_table[i][j];
		}
		result[i][num_of_attributes] = (i==0)? "ECC_result": "-1";
	}
	
	// Specify the number of iterations fo ECC transform.
	int number_of_iterations = 5000; 
	// Specify the threshold of the increment
	// in the correlation coefficient between two iterations
	double termination_eps = 1e-10;

	double success_rate = 0;
	while ( success_rate < keep_rate ) {
		int num_success = 0;
		num_kept_defect = 0;
		num_kept_nuisance = 0;
		for (int i = 1; i < num_of_lines; i++) {
			if ( result[i][num_of_attributes] == "-1" ) {
				string patch_file = clearDoublequote(result[i][def_image]);
				string ref_file = clearDoublequote(result[i][ref_image]);
	
				string patch_name = extractFilename(patch_file);
				string ref_name = extractFilename(ref_file);
				
				cout << "Calibrating image: " << patch_name <<  endl;
				
				Mat patch = imread(patch_file, CV_LOAD_IMAGE_GRAYSCALE);
				Mat ref = imread(ref_file, CV_LOAD_IMAGE_GRAYSCALE);
			
				if ( ic_method.substr(2, 2) == "00" ) {
					if ( icmap00[ic_method](patch, ref, number_of_iterations, termination_eps) < 0 ) continue;
				}
				else {
					icmap[ic_method](patch, ref);	
				}
				
				result[i][num_of_attributes] = "0";
				++num_success;

				string out_patch = createImgnamewithIC(ic_method, left_folder, patch_name);
				string out_ref = createImgnamewithIC(ic_method, right_folder, ref_name);
				result[i][def_image] = out_patch;
				result[i][ref_image] = out_ref;
				imwrite(out_patch.c_str(), patch);
				imwrite(out_ref.c_str(), ref);
			}
			else {
				++num_success;
			}

			if ( result[i][num_of_attributes] == "0" && result[i][type] == "1" ) {
				++num_kept_defect;
			}
			if ( result[i][num_of_attributes] == "0" && result[i][type] == "0" ) {
				++num_kept_nuisance;
			}
		}

		success_rate = (double) num_success / (num_of_lines - 1);
		cout << "Termination eps = " << termination_eps << endl;
		cout << "Success rate: " << success_rate * 100 << "%" << endl;
		cout << "# Kept defect: " << num_kept_defect << endl;
		cout << "# Kept nuisance: " << num_kept_nuisance << endl;
		termination_eps *= 2;	
	}


	string output_csv_name = ic_method + "/" + ic_method + ".csv";
	writecsv(output_csv_name.c_str(), result, num_of_lines, num_of_attributes + 1 , 0, discard);
	out_csv_path = output_csv_name;
}








int Alignment::IC0000(Mat & patch, Mat & ref, int num_its, double term_eps)
{
	return alignECC_Translation(patch, ref, num_its, term_eps);
}
int Alignment::IC0001(Mat & patch, Mat & ref, int num_its, double term_eps)
{
	return alignECC_Euclidean(patch, ref, num_its, term_eps);
}
int Alignment::IC0002(Mat & patch, Mat & ref, int num_its, double term_eps)
{
	return alignECC_Affine(patch, ref, num_its, term_eps);
}
int Alignment::IC0003(Mat & patch, Mat & ref, int num_its, double term_eps)
{
	return alignECC_Homography(patch, ref, num_its, term_eps);
}

int Alignment::IC0010(Mat & patch, Mat & ref, int num_its, double term_eps)
{
	int r = alignECC_Translation(patch, ref, num_its, term_eps);
	if ( r < 0 ){}
	else {
		histogramEqualization(patch, ref);
		alignDCTKeepTenPixels(patch, ref);
	}
	return r;
}
int Alignment::IC0011(Mat & patch, Mat & ref, int num_its, double term_eps)
{
	int r = alignECC_Euclidean(patch, ref, num_its, term_eps);
	if ( r < 0 ){}
	else {
		histogramEqualization(patch, ref);
		alignDCTKeepTenPixels(patch, ref);
	}
	return r;
}
int Alignment::IC0012(Mat & patch, Mat & ref, int num_its, double term_eps)
{
	int r = alignECC_Affine(patch, ref, num_its, term_eps);
	if ( r < 0 ){}
	else {
		histogramEqualization(patch, ref);
		alignDCTKeepTenPixels(patch, ref);
	}
	return r;
}
int Alignment::IC0013(Mat & patch, Mat & ref, int num_its, double term_eps)
{
	int r = alignECC_Homography(patch, ref, num_its, term_eps);
	if ( r < 0 ){}
	else {
		histogramEqualization(patch, ref);
		alignDCTKeepTenPixels(patch, ref);
	}
	return r;
}

int Alignment::IC0020(Mat & patch, Mat & ref, int num_its, double term_eps)
{
	int r = alignECC_Translation(patch, ref, num_its, term_eps);
	if ( r < 0 ){}
	else {
		alignDCTTopRLeftCAvg(patch, ref);
	}
	return r;
}
int Alignment::IC0021(Mat & patch, Mat & ref, int num_its, double term_eps)
{
	int r = alignECC_Euclidean(patch, ref, num_its, term_eps);
	if ( r < 0 ){}
	else {
		alignDCTTopRLeftCAvg(patch, ref);
	}
	return r;
}
int Alignment::IC0022(Mat & patch, Mat & ref, int num_its, double term_eps)
{
	int r = alignECC_Affine(patch, ref, num_its, term_eps);
	if ( r < 0 ){}
	else {
		alignDCTTopRLeftCAvg(patch, ref);
	}
	return r;
}
int Alignment::IC0023(Mat & patch, Mat & ref, int num_its, double term_eps)
{
	int r = alignECC_Homography(patch, ref, num_its, term_eps);
	if ( r < 0 ){}
	else {
		alignDCTTopRLeftCAvg(patch, ref);
	}
	return r;
}

int Alignment::IC0030(Mat & patch, Mat & ref, int num_its, double term_eps)
{
	int r = alignECC_Translation(patch, ref, num_its, term_eps);
	if ( r < 0 ){}
	else {
		histogramEqualization(patch, ref);
		alignDCTTopRLeftCAvg(patch, ref);
	}
	return r;
}
int Alignment::IC0031(Mat & patch, Mat & ref, int num_its, double term_eps)
{
	int r = alignECC_Euclidean(patch, ref, num_its, term_eps);
	if ( r < 0 ){}
	else {
		histogramEqualization(patch, ref);
		alignDCTTopRLeftCAvg(patch, ref);
	}
	return r;
}
int Alignment::IC0032(Mat & patch, Mat & ref, int num_its, double term_eps)
{
	int r = alignECC_Affine(patch, ref, num_its, term_eps);
	if ( r < 0 ){}
	else {
		histogramEqualization(patch, ref);
		alignDCTTopRLeftCAvg(patch, ref);
	}
	return r;
}
int Alignment::IC0033(Mat & patch, Mat & ref, int num_its, double term_eps)
{
	int r = alignECC_Homography(patch, ref, num_its, term_eps);
	if ( r < 0 ){}
	else {
		histogramEqualization(patch, ref);
		alignDCTTopRLeftCAvg(patch, ref);
	}
	return r;
}

int Alignment::IC0040(Mat & patch, Mat & ref, int num_its, double term_eps)
{
	int r = alignECC_Translation(patch, ref, num_its, term_eps);
	if ( r < 0 ){}
	else {
		histogramEqualization(patch, ref);
	}
	return r;
}
int Alignment::IC0041(Mat & patch, Mat & ref, int num_its, double term_eps)
{
	int r = alignECC_Euclidean(patch, ref, num_its, term_eps);
	if ( r < 0 ){}
	else {
		histogramEqualization(patch, ref);
	}
	return r;
}
int Alignment::IC0042(Mat & patch, Mat & ref, int num_its, double term_eps)
{
	int r = alignECC_Affine(patch, ref, num_its, term_eps);
	if ( r < 0 ){}
	else {
		histogramEqualization(patch, ref);
	}
	return r;
}
int Alignment::IC0043(Mat & patch, Mat & ref, int num_its, double term_eps)
{
	int r = alignECC_Homography(patch, ref, num_its, term_eps);
	if ( r < 0 ){}
	else {
		histogramEqualization(patch, ref);
	}
	return r;
}

int Alignment::IC0050(Mat & patch, Mat & ref, int num_its, double term_eps)
{
	int r = alignECC_Translation(patch, ref, num_its, term_eps);
	if ( r < 0 ){}
	else {
		alignDCTKeepTenPixels(patch, ref);
	}
	return r;
}
int Alignment::IC0051(Mat & patch, Mat & ref, int num_its, double term_eps)
{
	int r = alignECC_Euclidean(patch, ref, num_its, term_eps);
	if ( r < 0 ){}
	else {
		alignDCTKeepTenPixels(patch, ref);
	}
	return r;
}
int Alignment::IC0052(Mat & patch, Mat & ref, int num_its, double term_eps)
{
	int r = alignECC_Affine(patch, ref, num_its, term_eps);
	if ( r < 0 ){}
	else {
		alignDCTKeepTenPixels(patch, ref);
	}
	return r;
}
int Alignment::IC0053(Mat & patch, Mat & ref, int num_its, double term_eps)
{
	int r = alignECC_Homography(patch, ref, num_its, term_eps);
	if ( r < 0 ){}
	else {
		alignDCTKeepTenPixels(patch, ref);
	}
	return r;
}

int Alignment::IC0060(Mat & patch, Mat & ref, int num_its, double term_eps)
{
	int r = alignECC_Translation(patch, ref, num_its, term_eps);
	if ( r < 0 ){}
	else {
		alignDCTZigZag10(patch, ref);
	}
	return r;
}
int Alignment::IC0061(Mat & patch, Mat & ref, int num_its, double term_eps)
{
	int r = alignECC_Euclidean(patch, ref, num_its, term_eps);
	if ( r < 0 ){}
	else {
		alignDCTZigZag10(patch, ref);
	}
	return r;
}
int Alignment::IC0062(Mat & patch, Mat & ref, int num_its, double term_eps)
{
	int r = alignECC_Affine(patch, ref, num_its, term_eps);
	if ( r < 0 ){}
	else {
		alignDCTZigZag10(patch, ref);
	}
	return r;
}
int Alignment::IC0063(Mat & patch, Mat & ref, int num_its, double term_eps)
{
	int r = alignECC_Homography(patch, ref, num_its, term_eps);
	if ( r < 0 ){}
	else {
		alignDCTZigZag10(patch, ref);
	}
	return r;
}

void Alignment::IC0100(Mat & patch, Mat & ref)
{
	histogramEqualization(patch, ref);
}
void Alignment::IC0110(Mat & patch, Mat & ref)
{
	histogramEqualization(patch, ref);
	alignDCTTopRLeftCAvg(patch, ref);
	
}
void Alignment::IC0111(Mat & patch, Mat & ref)
{
	histogramEqualization(patch, ref);
	alignDCTKeepTenPixels(patch, ref);
}
void Alignment::IC0112(Mat & patch, Mat & ref)
{
	histogramEqualization(patch, ref);
	alignDCTZigZag10(patch, ref);
}
void Alignment::IC0200(Mat & patch, Mat & ref)
{
	alignDCTTopRLeftCAvg(patch, ref);
}
void Alignment::IC0210(Mat & patch, Mat & ref)
{
	alignDCTTopRLeftCAvg(patch, ref);
	histogramEqualization(patch, ref);
}
void Alignment::IC0300(Mat & patch, Mat & ref)
{
	alignDCTKeepTenPixels(patch, ref);
}
void Alignment::IC0310(Mat & patch, Mat & ref)
{
	alignDCTKeepTenPixels(patch, ref);
	histogramEqualization(patch, ref);
}
void Alignment::IC0400(Mat & patch, Mat & ref)
{
	alignDCTZigZag10(patch, ref);
}
void Alignment::IC0410(Mat & patch, Mat & ref)
{
	alignDCTZigZag10(patch, ref);
	histogramEqualization(patch, ref);
}
