#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char* argv[]){
	Mat img = imread(argv[1],-1);
	cout << "cols: " << img.cols << endl;
	cout << "rows: " << img.rows << endl;
	return 0;
}