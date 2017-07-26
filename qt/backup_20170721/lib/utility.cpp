#include "../include/utility.h"

int readNumOfLines(const char filename[])
{
    	fstream fp;
    	fp.open(filename, ios::in);
	if(!fp){
     	   	cout << "Fail to open file: " << filename << endl;
		return 0;
        }

	int num_lines = 0;
	string tmp;
    	while(getline(fp, tmp)){
		++num_lines;
    	}
    	fp.close();

    	return num_lines;
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

void writecsv(const char filename[] , string** table, int rows, int cols){
	fstream fp;
	fp.open(filename, ios::out);
	if (!fp) {
		cout << "Fail to open file: " << filename << endl;
		return;
	}

 	for (int i = 0; i < rows; i++)
 	{
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

inline string clearDoublequote(string str)
{
	str.erase(str.begin());
	str.erase(str.end()-1);

	return str;
}

// 5th col: defect image
// 6th col: reference image
void calResult(string** result, int num_of_lines, int num_of_cols)
{
	result[0][7] = "PSNR"; 
	result[0][8] = "diffPixelsCount_145";
	result[0][9] = "diffPixelsCount_150";
	result[0][10] = "diffPixelsCount_160";
	result[0][11] = "diffPixelsCount_170";
	result[0][12] = "diffPixelsCount_180";
	result[0][13] = "diffPixelsCount_190";
	result[0][14] = "diffPixelsCount_200";
	result[0][15] = "diffPixelsCount_210";
	result[0][16] = "localmeanSSE_4";
	result[0][17] = "localmeanSSE_8";
	result[0][18] = "localmeanSSE_16";
	result[0][19] = "localmeanSSE_32";
	result[0][20] = "localmeanSAE_4";
	result[0][21] = "localmeanSAE_8";
	result[0][22] = "localmeanSAE_16";
	result[0][23] = "localmeanSAE_32";
	
	for (int i = 1; i < num_of_lines; i++) {
		string patch_file = clearDoublequote(result[i][5]);
		string ref_file = clearDoublequote(result[i][6]);

		Mat patch = imread(patch_file, CV_LOAD_IMAGE_GRAYSCALE);
		Mat ref = imread(ref_file, CV_LOAD_IMAGE_GRAYSCALE);

		Alignment af(patch, ref);
		Feature fe(patch, ref);

		af.align();
		
		// std::to_string supported in C++11
		result[i][7] = std::to_string(fe.getPSNR(patch, ref));
		result[i][8] = std::to_string(fe.diffPixelsCount(patch, ref, 145));
		result[i][9] = std::to_string(fe.diffPixelsCount(patch, ref, 150));
		result[i][10] = std::to_string(fe.diffPixelsCount(patch, ref, 160));
		result[i][11] = std::to_string(fe.diffPixelsCount(patch, ref, 170));
		result[i][12] = std::to_string(fe.diffPixelsCount(patch, ref, 180));
		result[i][13] = std::to_string(fe.diffPixelsCount(patch, ref, 190));
		result[i][14] = std::to_string(fe.diffPixelsCount(patch, ref, 200));
		result[i][15] = std::to_string(fe.diffPixelsCount(patch, ref, 210));
		result[i][16] = std::to_string(fe.localmeanSSE(patch, ref, 4));
		result[i][17] = std::to_string(fe.localmeanSSE(patch, ref, 8));
		result[i][18] = std::to_string(fe.localmeanSSE(patch, ref, 16));	
                result[i][19] = std::to_string(fe.localmeanSSE(patch, ref, 32));
                result[i][20] = std::to_string(fe.localmeanSAE(patch, ref, 4));
                result[i][21] = std::to_string(fe.localmeanSAE(patch, ref, 8));
                result[i][22] = std::to_string(fe.localmeanSAE(patch, ref, 16));
                result[i][23] = std::to_string(fe.localmeanSAE(patch, ref, 32));
	}
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
	magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
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

bool isPowerOfTwo(int x)
{

	return ( (x != 0) && ( (x & (~x + 1)) == x) );
}

