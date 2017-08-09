#include "utility.h"



cv::Size readNumOfLines(const char filename[])
{
	cv::Size size;

    	fstream fp;
    	fp.open(filename, ios::in);
	if(!fp){
     	   	cout << "Fail to open file: " << filename << endl;
		return Size(0, 0);
        }

	int num_lines = 0;
	int num_cols = 0;
	string tmp;
	getline(fp, tmp);
	++num_lines;

	char * pch;
	pch = strtok (strdup(tmp.c_str()), ",");
	while(pch != NULL) {
		++num_cols;
		pch = strtok(NULL, ",");
	}

    	while(getline(fp, tmp)){
		++num_lines;
    	}
    	fp.close();

	size.width = num_cols;
	size.height = num_lines;

    	return size;
}

string** readcsv(const char filename[], int row, int col){
	
    	fstream fp;
      	fp.open(filename, ios::in);//開啟檔案
   	if(!fp){//如果開啟檔案失敗，fp為0；成功，fp為非0
     	   	cout<<"Fail to open file: "<<filename<<endl;
		return NULL;
        }
	//cout<<"File Descriptor: " << fp << endl;

	string ** table = new string*[row];
	for ( int i = 0; i < row; i++ ) {
		table[i] = new string[col];
		for ( int j = 0; j < col; j++ ) {
			table[i][j] = "";
		}

	}

    	char line[256];
	for(int i = 0; i < row; i++){
	   	for(int j = 0; j < col - 1; j++){
  			fp.getline(line, 256, ',');
			table[i][j] = line;
	   	}
		fp.getline(line, 256);
	        table[i][col-1] = line;
        }
	
	return table;
}

void readcsv(const char filename[], string ** table, int row, int col) {
	
    	fstream fp;
      	fp.open(filename, ios::in);//開啟檔案
   	if(!fp){//如果開啟檔案失敗，fp為0；成功，fp為非0
     	   	cout<<"Fail to open file: "<<filename<<endl;
		return;
        }
	//cout<<"File Descriptor: " << fp << endl;

    	char line[256];
	for(int i = 0; i < row; i++){
	   	for(int j = 0; j < col - 1; j++){
  			fp.getline(line, 256, ',');
			table[i][j] = line;
	   	}
		fp.getline(line, 256);
	        table[i][col-1] = line;
        }
}

void writecsv(const char filename[] , string** table, const int rows, const int cols, const int num_of_attributes, bool discard){
	fstream fp;
	fp.open(filename, ios::out);
	if (!fp) {
		cout << "Fail to open file: " << filename << endl;
		return;
	}

 	for (int i = 0; i < rows; i++)
 	{
		if ( table[i][num_of_attributes] == "-1" && discard ) continue;
		for (int j = 0; j < cols; j++)
 	 	{
 	  		fp << table[i][j];
			if ( j < cols - 1 ) {
				fp << ",";
			}
		}
		if ( i < rows - 1) 	 
			fp << "\n";
	}
 	fp.close();
}

void createFolders(string name, string left_folder, string right_folder)
{
	// create a folder named ic_method to store aligned images	
	string mkdir_command = "mkdir -p " + name;
	int dir_err = system(mkdir_command.c_str());
	if (dir_err == -1)
	{
    		printf("Error creating directory!n");
	    	exit(1);
	}
	mkdir_command = "mkdir -p " + name + "/" + left_folder;
	dir_err = system(mkdir_command.c_str());
	if (dir_err == -1)
	{
    		printf("Error creating directory!n");
	    	exit(1);
	}
	mkdir_command = "mkdir -p " + name + "/" + right_folder;
	dir_err = system(mkdir_command.c_str());
	if (dir_err == -1)
	{
    		printf("Error creating directory!n");
	    	exit(1);
	}
}

inline string clearDoublequote(string str)
{
	if ( *str.begin() == '\"' && *(str.end()-1) == '\"' ) {
		str.erase(str.begin());
		str.erase(str.end()-1);
	}

	return str;
}

string extractFilename (std::string& str)
{
	//std::cout << "Splitting: " << str << '\n';
	std::size_t found = str.find_last_of("/\\");
	//std::cout << " path: " << str.substr(0,found) << '\n';
	//std::cout << " file: " << str.substr(found+1) << '\n';
	return str.substr(found+1);
}

string createImgnamewithIC(string &ic_method, string &_folder, string &_name)
{
	std::size_t found = _name.find(".");
	//cout << _name.substr(0, found) << endl;
	
	return ic_method + "/" + _folder + "/" + _name.substr(0, found) + "_" + ic_method.substr(2) + _name.substr(found);
}
/*
void test(Mat m, Mat mm, int x, double) {

}
*//*
void ICFilter(string ic_method, double keep_rate, const char csv_path[], int & num_kept_defect, int & num_kept_nuisance)
{
	//std::map<std::string, std::map > icmap = createICMap();
	std::map<std::string, std::function<void(Mat&, Mat&, int, double)>> icmap00 ={{"IC0000", af.IC0000}};
	//std::map<std::string, std::function<bool(int)>> testmap = {{"isPowerOfTwo", isPowerOfTwo}};
	//std::map<std::string, std::function<void(Mat, Mat, int, double)>> qq = {{"test", test}};

	cv::Size size = readNumOfLines(csv_path);
	int num_of_lines = size.height;
	int num_of_attributes = size.width;
	string ** input_csv_table = readcsv(csv_path, num_of_lines, num_of_attributes);

	string left_folder = "left128";
	string right_folder = "right128";
	createFolders(ic_method, left_folder, right_folder);

	// Specify the number of iterations.
	int number_of_iterations = 5000; 
	// Specify the threshold of the increment
	// in the correlation coefficient between two iterations
	double termination_eps = 1e-10;

	int def_image = num_of_attributes - 2;
	int ref_image = num_of_attributes - 1;
	for ( int i = 0; i < num_of_attributes; i++ ) {
		if (input_csv_table[0][i] == "DEF_IMAGE")	def_image = i;
		if (input_csv_table[0][i] == "REF_IMAGE")	ref_image = i;
	}
	
	string** result = new string*[num_of_lines];
	for (int i = 0; i < num_of_lines; i++) {
		result[i] = new string[num_of_attributes + 1];
		for (int j = 0; j < num_of_attributes; j++) {
			result[i][j] = input_csv_table[i][j];
		}
		result[i][num_of_attributes] = (i==0)? "ECC_result": "-1";
	}
	
	int num_success = 0;
	double success_rate = 0;
	while ( success_rate < keep_rate ) {
		for (int i = 1; i < num_of_lines; i++) {
			cout << "Processing image number: " << i <<  endl;
	
			string patch_file = clearDoublequote(input_csv_table[i][def_image]);
			string ref_file = clearDoublequote(input_csv_table[i][ref_image]);
	
			string patch_name = extractFilename(patch_file);
			string ref_name = extractFilename(ref_file);
			
			Mat patch = imread(patch_file, CV_LOAD_IMAGE_GRAYSCALE);
			Mat ref = imread(ref_file, CV_LOAD_IMAGE_GRAYSCALE);
			
	
			++num_success;

			string out_patch = createImgnamewithIC(ic_method, left_folder, patch_name);
			string out_ref = createImgnamewithIC(ic_method, right_folder, ref_name);
			imwrite(out_patch.c_str(), patch);
			imwrite(out_ref.c_str(), ref);
		}

		success_rate = (double) num_success / (num_of_lines - 1);
	}


	string output_csv_name = ic_method + "/" + ic_method + ".csv";
	writecsv(output_csv_name.c_str(), result, num_of_lines, num_of_attributes + 1 , 0, false);
}
*/





// 5th col: defect image
// 6th col: reference image
float calResult(string** result, const int num_of_lines, const int num_of_cols, const int num_of_attributes)
{
	int k = num_of_attributes;
	result[0][k++] = "PSNR"; 
	result[0][k++] = "diffPixelsCount_50";
	result[0][k++] = "diffPixelsCount_70";
	result[0][k++] = "diffPixelsCount_80";
	result[0][k++] = "diffPixelsCount_90";
	result[0][k++] = "diffPixelsCount_100";
	result[0][k++] = "diffPixelsCount_120";
	result[0][k++] = "diffHoughLineCount";
	result[0][k++] = "localmeanSSE_16";
	result[0][k++] = "localmeanSSE_20";
	result[0][k++] = "localmeanSSE_24";
	result[0][k++] = "localmeanSSE_25";
	result[0][k++] = "localmeanSSE_28";
	result[0][k++] = "localmeanSSE_32";
	result[0][k++] = "localmeanSAE_16";
	result[0][k++] = "localmeanSAE_20";
	result[0][k++] = "localmeanSAE_24";
	result[0][k++] = "localmeanSAE_25";
	result[0][k++] = "localmeanSAE_28";
	result[0][k++] = "localmeanSAE_32";
	result[0][k++] = "maxNumBlackInBlock_80_16";
	result[0][k++] = "maxNumBlackInBlock_80_20";
	result[0][k++] = "maxNumBlackInBlock_80_24";
	result[0][k++] = "maxNumBlackInBlock_80_25";
	result[0][k++] = "maxNumBlackInBlock_80_28";
	result[0][k++] = "maxNumBlackInBlock_80_32";
	result[0][k++] = "maxNumDiffInBlock_90_16";
	result[0][k++] = "maxNumDiffInBlock_90_20";
	result[0][k++] = "maxNumDiffInBlock_90_24";
	result[0][k++] = "maxNumDiffInBlock_90_25";
	result[0][k++] = "maxNumDiffInBlock_90_28";
	result[0][k++] = "maxNumDiffInBlock_90_32";
	result[0][k++] = "min_localPSNR_20";
	result[0][k++] = "max_localPSNR_20";
	result[0][k++] = "mean_localPSNR_20";
	result[0][k++] = "stdev_localPSNR_20";
	result[0][k++] = "var_localPSNR_20";
	result[0][k++] = "min_localPSNR_24";
        result[0][k++] = "max_localPSNR_24";
        result[0][k++] = "mean_localPSNR_24";
        result[0][k++] = "stdev_localPSNR_24";
        result[0][k++] = "var_localPSNR_24";
	result[0][k++] = "min_localPSNR_32";
        result[0][k++] = "max_localPSNR_32";
        result[0][k++] = "mean_localPSNR_32";
        result[0][k++] = "stdev_localPSNR_32";
        result[0][k++] = "var_localPSNR_32";


	int num_discard = 0;

	int def_image = num_of_attributes - 2;
	int ref_image = num_of_attributes - 1;
	for ( int i = 0; i < num_of_attributes; i++ ) {
		if ( clearDoublequote(result[0][i]) == "DEF_IMAGE") 	def_image = i;
		if ( clearDoublequote(result[0][i]) == "REF_IMAGE")	ref_image = i;
	}
	
	
	for (int i = 1; i < num_of_lines; i++) {

		string patch_file = clearDoublequote(result[i][def_image]);
		string ref_file = clearDoublequote(result[i][ref_image]);

		cout << "Processing image number: " << extractFilename(patch_file) <<  endl;
		
		Mat patch = imread(patch_file, CV_LOAD_IMAGE_GRAYSCALE);
		Mat ref = imread(ref_file, CV_LOAD_IMAGE_GRAYSCALE);

		/*medianBlur(patch, patch, 3);
		medianBlur(ref, ref, 3);
	
		Alignment af(patch, ref);

		if ( af.align() < 0) {
			for ( int j = 7; j < num_of_cols; j++ ) {
				result[i][j] = "-1";
			}
			++num_discard;
			continue;
		}
		af.alignDCTZigZag10(patch, ref);*/
		
		Feature fe(patch, ref);

		k = num_of_attributes;	
		// std::to_string supported in C++11
		result[i][k++] = std::to_string(fe.getPSNR(patch, ref));
		result[i][k++] = std::to_string(fe.diffPixelsCount(patch, ref, 50));
		result[i][k++] = std::to_string(fe.diffPixelsCount(patch, ref, 70));
		result[i][k++] = std::to_string(fe.diffPixelsCount(patch, ref, 80));
		result[i][k++] = std::to_string(fe.diffPixelsCount(patch, ref, 90));
		result[i][k++] = std::to_string(fe.diffPixelsCount(patch, ref, 100));
		result[i][k++] = std::to_string(fe.diffPixelsCount(patch, ref, 120));
		result[i][k++] = std::to_string(fe.diffHoughLineCount(patch, ref, 50, 150, 62));
		result[i][k++] = std::to_string(fe.localmeanSSE(patch, ref, 16));
		result[i][k++] = std::to_string(fe.localmeanSSE(patch, ref, 20));
		result[i][k++] = std::to_string(fe.localmeanSSE(patch, ref, 24));
		result[i][k++] = std::to_string(fe.localmeanSSE(patch, ref, 25)); 	
                result[i][k++] = std::to_string(fe.localmeanSSE(patch, ref, 28));
                result[i][k++] = std::to_string(fe.localmeanSSE(patch, ref, 32));
                result[i][k++] = std::to_string(fe.localmeanSAE(patch, ref, 16)); 
                result[i][k++] = std::to_string(fe.localmeanSAE(patch, ref, 20));
                result[i][k++] = std::to_string(fe.localmeanSAE(patch, ref, 24));
		result[i][k++] = std::to_string(fe.localmeanSAE(patch, ref, 25));
		result[i][k++] = std::to_string(fe.localmeanSAE(patch, ref, 28));
		result[i][k++] = std::to_string(fe.localmeanSAE(patch, ref, 32));
		result[i][k++] = std::to_string(fe.maxNumBlackPixelsInBlock(patch, 80, 16));
		result[i][k++] = std::to_string(fe.maxNumBlackPixelsInBlock(patch, 80, 20));
		result[i][k++] = std::to_string(fe.maxNumBlackPixelsInBlock(patch, 80, 24));
                result[i][k++] = std::to_string(fe.maxNumBlackPixelsInBlock(patch, 80, 25));
                result[i][k++] = std::to_string(fe.maxNumBlackPixelsInBlock(patch, 80, 28));
                result[i][k++] = std::to_string(fe.maxNumBlackPixelsInBlock(patch, 80, 32));
                result[i][k++] = std::to_string(fe.maxNumDiffPixelsInBlock(patch, ref, 30, 16));
                result[i][k++] = std::to_string(fe.maxNumDiffPixelsInBlock(patch, ref, 30, 20));
                result[i][k++] = std::to_string(fe.maxNumDiffPixelsInBlock(patch, ref, 30, 24));
                result[i][k++] = std::to_string(fe.maxNumDiffPixelsInBlock(patch, ref, 30, 25));
                result[i][k++] = std::to_string(fe.maxNumDiffPixelsInBlock(patch, ref, 30, 28));
                result[i][k++] = std::to_string(fe.maxNumDiffPixelsInBlock(patch, ref, 30, 32));
                result[i][k++] = std::to_string(fe.localPSNR(patch, ref, 20).min);
                result[i][k++] = std::to_string(fe.localPSNR(patch, ref, 20).max);
                result[i][k++] = std::to_string(fe.localPSNR(patch, ref, 20).mean);
                result[i][k++] = std::to_string(fe.localPSNR(patch, ref, 20).stdev);
                result[i][k++] = std::to_string(fe.localPSNR(patch, ref, 20).var);
		result[i][k++] = std::to_string(fe.localPSNR(patch, ref, 24).min); 	
                result[i][k++] = std::to_string(fe.localPSNR(patch, ref, 24).max);
                result[i][k++] = std::to_string(fe.localPSNR(patch, ref, 24).mean);
	        result[i][k++] = std::to_string(fe.localPSNR(patch, ref, 24).stdev);
                result[i][k++] = std::to_string(fe.localPSNR(patch, ref, 24).var);
                result[i][k++] = std::to_string(fe.localPSNR(patch, ref, 32).min);
                result[i][k++] = std::to_string(fe.localPSNR(patch, ref, 32).max);
                result[i][k++] = std::to_string(fe.localPSNR(patch, ref, 32).mean);
                result[i][k++] = std::to_string(fe.localPSNR(patch, ref, 32).stdev);
                result[i][k++] = std::to_string(fe.localPSNR(patch, ref, 32).var);
		
	}

	float ok_data_rate = 0.0;
	ok_data_rate = (float)((num_of_lines-1) - num_discard) / (num_of_lines-1) * 100;
	return ok_data_rate;
}

/*************Get Histgram Image*******************
	        getHistImg(Mat)
**************Get Histgram Image*******************/
Mat getHistImg(const Mat &srcImg)
{   
	Mat calculedhist;
	float range[] = { 0, 256 };
	int histSize = 256;
	const float* histRange = { range };
	bool uniform = true; 
	bool accumulate = false;
	
	//Mat gray_src = srcImg.clone();
	//cvtColor(gray_src, gray_src, CV_BGR2GRAY);
	//calcHist(&gray_src, 1, 0, Mat(), calculedhist, 1, &histSize, &histRange, uniform, accumulate);	
	calcHist(&srcImg, 1, 0, Mat(), calculedhist, 1, &histSize, &histRange, uniform, accumulate);	
	double maxVal = 0;
	double minVal = 0;

	//Find Max and min
	minMaxLoc(calculedhist, &minVal, &maxVal);
	Mat histImg(histSize, histSize, CV_8U, Scalar(255));
	//Set Maximum as 90% length line in histgram windows
	int hpt = static_cast<int>(0.9 * histSize);
	
	for(int h = 0; h < histSize; h++)
	{
		float binVal = calculedhist.at<float>(h);
		int intensity = static_cast<int>(binVal * hpt / maxVal);
		line(histImg,Point(h, 255),Point(h, histSize-intensity), Scalar::all(0));
	}
	return histImg;
}

Mat getGradient(const Mat & srcImg)
{
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	int scale = 1;
	int delta = 0;
	int ddepth = CV_32FC1;
	Sobel( srcImg, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
	convertScaleAbs( grad_x, abs_grad_x);

	Sobel( srcImg, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
	convertScaleAbs( grad_y, abs_grad_y);

	Mat grad;
	addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
	
	return grad;
}

inline void shiftToMiddle(Mat & magI)
{
	
	// rearrange the quadrants of Fourier image  so that the origin is at the image center
	int cx = magI.cols/2;
	int cy = magI.rows/2;
	
	Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
	Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
	Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
	Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right
	
	Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	
	q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
	q2.copyTo(q1);
	tmp.copyTo(q2);
	
}

Mat fourierTransform(const Mat & I)
{
	Mat padded;                            //expand input image to optimal size
	int m = getOptimalDFTSize( I.rows );
	int n = getOptimalDFTSize( I.cols ); // on the border add zero values
	copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));
	
	Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
	Mat complexI;
	merge(planes, 2, complexI);         // Add to the expanded another plane with zeros
	
	dft(complexI, complexI);            // this way the result may fit in the source matrix
	
	// compute the magnitude and switch to logarithmic scale
	// => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
	split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
	magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitudei

	Mat magI = planes[0];
	
	magI += Scalar::all(1);                    // switch to logarithmic scale
	log(magI, magI);
	
	// crop the spectrum, if it has an odd number of rows or columns
	magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));
	
	shiftToMiddle(magI);
	
	/*
	normalize(magI, magI, 0, 1, CV_MINMAX); // Transform the matrix with float values into a
	                                        // viewable image form (float between values 0 and 1).
	*/

	normalize(magI, magI, 0, 255, CV_MINMAX);
	Mat showdft;
	magI.convertTo(showdft, CV_8U);
	
	
	//imshow("Input Image"       , I   );    // Show the result
	//imshow("spectrum magnitude", magI);
	//waitKey();

	return showdft;
}

Mat cosineTransform(const Mat & I)
{

	Mat padded;                            //expand input image to optimal size
	int m = getOptimalDFTSize( I.rows );
	int n = getOptimalDFTSize( I.cols ); // on the border add zero values
	copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));
	padded.convertTo(padded, CV_32F);
	
	
	Mat magI;
	dct(padded, magI);            // this way the result may fit in the source matrix
	
	magI += Scalar::all(1);                    // switch to logarithmic scale
	log(magI, magI);
	
	// crop the spectrum, if it has an odd number of rows or columns
	magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

	//shiftToMiddle(magI);	
	
	normalize(magI, magI, 0, 255, CV_MINMAX);
	Mat showdft;
	magI.convertTo(showdft, CV_8U);
	
	return showdft;
}

Mat phaseOnlyFT(const Mat & I)
{
	Mat padded;                            //expand input image to optimal size
	int m = getOptimalDFTSize( I.rows );
	int n = getOptimalDFTSize( I.cols ); // on the border add zero values
	copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));
	
	Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
	Mat complexI;
	merge(planes, 2, complexI);         // Add to the expanded another plane with zeros
	
	dft(complexI, complexI);            // this way the result may fit in the source matrix
	split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))

	Mat magI;
	magnitude(planes[0], planes[1], magI);
	
	planes[0] /= magI;
	planes[1] /= magI;

	Mat phase;
	merge(planes, 2, phase);

	Mat idft;
	dft(phase, idft, DFT_INVERSE|DFT_REAL_OUTPUT);

	normalize(idft, idft, 0, 1, CV_MINMAX);

	return idft;
}

bool isPowerOfTwo(int x)
{

	return ( (x != 0) && ( (x & (~x + 1)) == x) );
}

vector<Mat> sliceImg(const Mat &srcImg, const int pool_size)
{
	vector<Mat> smallImages;
	
	/*if ( srcImg.rows % pool_size != 0 ) {
		cout << "Error: in sliceImg(const Mat &srcImg, const int pool_size), ";
		cout << "pool_size should be divided by " << srcImg.rows <<  endl;
		return smallImages;
	}*/
	
	int optimal_size = srcImg.rows;
	while ( optimal_size % pool_size != 0 ) {
		optimal_size--;
	}
	
	int s = (srcImg.rows - optimal_size) / 2;
	cv::Size smallSize(pool_size, pool_size);

	for ( int rowIndex = s; rowIndex < srcImg.rows - s - 1; rowIndex += smallSize.height ) {
		for ( int colIndex = s; colIndex < srcImg.cols-s - 1; colIndex += smallSize.width ) {
			cv::Rect rect = cv::Rect(colIndex, rowIndex, smallSize.width, smallSize.height);
			smallImages.push_back(Mat(srcImg, rect).clone());
		}
	}
	
	return smallImages;
}

void showGrid(const Mat srcImg, const int pool_size)
{
	int optimal_size = srcImg.rows;
	while ( optimal_size % pool_size != 0 ) {
		optimal_size--;
	}
	int s = (srcImg.rows - optimal_size) / 2;
	//Mat smaller_patch = Mat(srcImg, cv::Rect(start, start, optmal_size, optmal_size));
	Mat grid = srcImg.clone();
	cvtColor(grid, grid, COLOR_GRAY2BGR);
	for ( int rowIndex = s; rowIndex < grid.rows - s + 1; rowIndex++ ) {
		for ( int colIndex = s; colIndex < grid.cols - s + 1; colIndex++ ) {
			if (rowIndex >= grid.rows || colIndex >= grid.cols) break;
			if ( (rowIndex-s) % pool_size == 0 || (colIndex-s) % pool_size == 0 ) {
				grid.at<Vec3b>(rowIndex, colIndex) = Vec3b(0, 255, 0);
			}
		}
	}
	imshow("grid", grid);
	imwrite("grid1.png", grid);
	//imshow("qq", srcImg);
}

void printVector(vector<double> v)
{
	for (int i = 0; i < v.size(); i++ ) {
		cout << v[i] << "  ";
	}
	cout << endl;
}

Mat showHistograms_localPSNR(vector<double> v)
{
	vector<double> calculated_hist(30);
	for ( int i = 0; i < v.size(); i++ ) {
		int h = (int)v[i];
		calculated_hist[h]++;
	}
	
	int histsize = 100;
	vector<double> tmp = calculated_hist;
	double max = *std::max_element(calculated_hist.begin(), calculated_hist.end());
	// 0~100
	for ( int i = 0; i < calculated_hist.size(); i++ ) tmp[i] = calculated_hist[i] * histsize  / max;
	
	Mat histImg(histsize, histsize, CV_8U, Scalar(255));

	for(int h = 0; h < tmp.size(); h++)
	{
		int x = static_cast<int> ((double)h / tmp.size() * 100);
		int intensity = static_cast<int>(tmp[h] * 0.9);
		line(histImg, Point(x, 99), Point(x, histsize-intensity), Scalar::all(0));
	}
	return histImg;
}

double calSSE(const Mat &img1, const Mat &img2)
{
	double sse = 0;
	int n = img1.rows;

	for ( int i = 0; i < n; i++ ) {
		for ( int j = 0; j < n; j++ ) {
			sse += pow( (img1.at<float>(i, j) - img2.at<float>(i, j)), 2);
		}
	}
	return sse;
}

double calSSE(vector<double> v1, vector<double>v2)
{
	double sse = 0;
	for ( int i = 0; i < v1.size(); i++ ) {
		sse += pow( (v1[i] - v2[i]), 2);
	}
	return sse;
}

double calSAE(const Mat &img1, const Mat &img2)
{
	double sae = 0;
	int n = img1.rows;

	for ( int i = 0; i < n; i++ ) {
		for ( int j = 0; j < n; j++ ) {
			sae += abs(img1.at<float>(i, j) - img2.at<float>(i, j));
		}
	}
	return sae;
}

double calSAE(vector<double> v1, vector<double>v2)
{
	double sae = 0;
	for ( int i = 0; i < v1.size(); i++ ) {
		sae += abs(v1[i] - v2[i]);
	}
	return sae;
}

