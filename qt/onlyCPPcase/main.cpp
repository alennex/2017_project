
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include "include/alignment.h"
#include "include/feature.h"

using namespace cv;
using namespace std;

void openCSV(char*, vector< vector<string> >&);
void openImg(vector< vector<string> > ,vector<Mat>&, vector<Mat>&);
void calibration(int, vector<Mat>&, vector<Mat>&);
void featureExtraction(vector<int>, vector<Mat>, vector<Mat>, vector< vector<double> >&);
void collectAttribute(vector< vector<string> >, vector< vector<string> >&);

void overlap(vector< vector<string> >, vector< vector<string> >, vector< vector<double> >, 
	vector<int>, vector<string>&, vector<string>&, vector< vector<double> >&);
void overlap_col	(vector< vector<string> >, vector<string>&);
void overlap_row(vector<int>, vector<string>&);
string feName(int);
string int2string(int);

int main(int argc, char* argv[]){

	vector< vector<string> > inFile;
	char* fileName = argv[1];
	openCSV(fileName, inFile);

	vector<Mat> defImg, refImg;
	openImg(inFile, defImg, refImg);

//	calibration(5, defImg, refImg);
	
	vector<int> feIndex;
	feIndex.push_back(1);
	feIndex.push_back(3);
	feIndex.push_back(5);
	
	vector< vector<double> > feCal;
	featureExtraction(feIndex, defImg, refImg, feCal);

	vector< vector<string> > attTable;
	collectAttribute(inFile, attTable);
	
	vector<string> oAttCol, oAttRow;
	vector< vector<double> > oArea;

	overlap(inFile, attTable, feCal, feIndex, oAttCol, oAttRow, oArea);

	for(int i=0; i<attTable.size(); ++i){
		for(int  j=0; j<attTable[0].size(); ++j){
			cout << attTable[i][j] << " ";
		}
		cout << endl;
	}

	return 0;
}

void openCSV(char* _fileName, vector< vector<string> >& _inFile){

	string str1, str2;
	ifstream theFile(_fileName);

	 if(!theFile.is_open()){
	 	cout << "Error: csv file is not open..\nmain.out file.csv"<< endl;
	 }
	
	while (getline(theFile, str1)) {
		istringstream stream(str1);
		vector<string> RowContext;
         
         while(getline(stream, str2, ',')){
     		RowContext.push_back(str2);
     	}


        _inFile.push_back(RowContext);
    }
    theFile.close();
}

void openImg(vector< vector<string> > _inFile,vector<Mat>& _defImg, vector<Mat>& _refImg){

	int defCol, refCol;

	for(int i=0; i<_inFile[0].size(); ++i){
		if(_inFile[0][i].find("DEF_IMAGE") != std::string::npos)
			defCol = i;
		if(_inFile[0][i].find("REF_IMAGE") != std::string::npos)
			refCol = i;
	}
	string defName;
	string refName;

	for(int i=1; i<_inFile.size(); ++i){
		defName = _inFile[i][defCol];
		refName = _inFile[i][refCol];
		_defImg.push_back(imread(defName,CV_LOAD_IMAGE_GRAYSCALE));
		_refImg.push_back(imread(refName,CV_LOAD_IMAGE_GRAYSCALE));
		
		if(!_defImg[i-1].data){
			cout << "Error: DEF image " << i << " is not open."<< endl;
			cout << "defName Path:" << defName << endl;
			exit(0);
		}
		else if(!_refImg[i-1].data){
			cout << "Error: REF image " << i << " is not open."<< endl;
			cout << "refName Path:" << refName << endl;
			exit(0);
		}		
	}
}

void calibration(int _index, vector<Mat>& _defImg, vector<Mat>& _refImg){
	double thresh = 128;
	double max_value = 255;
	Alignment af;
	
	switch(_index){
          case 0:
               for(int i=0; i<_defImg.size();++i){
               	af.alignECC_Translation(_defImg[i],_refImg[i]);
               } 
               break;
          case 1:
               for(int i=0; i<_defImg.size();++i){
                    af.alignECC_Euclidean(_defImg[i],_refImg[i]);
               } 
               break;
          case 2:
               for(int i=0; i<_defImg.size();++i){
                    af.alignECC_Affine(_defImg[i],_refImg[i]);
               } 
               break;
          case 3:
               for(int i=0; i<_defImg.size();++i){
                    af.alignECC_Homography(_defImg[i],_refImg[i]);
               } 
               break;
          case 4:
               for(int i=0; i<_defImg.size();++i){
                    af.alignDCT(_defImg[i],_refImg[i]);
               } 
               break;
          case 5:
               for(int i=0; i<_defImg.size();++i){
                    af.alignDFT(_defImg[i],_refImg[i]);
               } 
               break;
          case 6:
               for(int i=0; i<_defImg.size();++i){
                    af.histogramEqualization(_defImg[i],_refImg[i]);
               } 
               break;
          case 7:
               for(int i=0; i<_defImg.size();++i){
                    af.threshOtsu(_defImg[i],_refImg[i], thresh, max_value);
               } 
               break;
          case 8:
               for(int i=0; i<_defImg.size();++i){
                    af.threshBinary(_defImg[i],_refImg[i], thresh, max_value);
               } 
               break;
          case 9:
               for(int i=0; i<_defImg.size();++i){
                    af.threshBinaryINV(_defImg[i],_refImg[i], thresh, max_value);
               } 
               break;
          case 10:
               for(int i=0; i<_defImg.size();++i){
                    af.threshTrunc(_defImg[i],_refImg[i], thresh, max_value);
               } 
               break;
          case 11:
               for(int i=0; i<_defImg.size();++i){
                    af.threshToZero(_defImg[i],_refImg[i], thresh, max_value);
               } 
               break;
          case 12:
               for(int i=0; i<_defImg.size();++i){
                    af.threshToZeroINV(_defImg[i],_refImg[i], thresh, max_value);
               } 
               break;
          default:
               break;
     }
}

void featureExtraction(vector<int> _index, vector<Mat> _defImg, vector<Mat> _refImg, vector< vector<double> >& _feCal){

	vector<double> __feCal;
	for(int i=0; i<_defImg.size(); ++i)
		__feCal.push_back(0);
	for(int i=0; i<_index.size(); ++i)
		_feCal.push_back(__feCal);

	int color_gap = 3;
     int pool_size = 4;
     int thresh = 128;
     int block_size = 16;

     Feature fe,feI(_defImg[0],_refImg[0]);

	for(int i=0; i<_index.size(); ++i){  
          switch(_index[i]){
               case 0:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.diffGlobalMean(_defImg[j], _refImg[j]);   
                    } 
                    break;
               case 1:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.diffPixelsCount(_defImg[j], _refImg[j], color_gap);
                    }
                    break;
               case 2:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = fe.getPSNR(_defImg[j], _refImg[j]);
                    }
                    break;
               case 3:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = fe.localmeanSSE(_defImg[j], _refImg[j], pool_size);
                    }
                    break;
               case 4:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = fe.localmeanSAE(_defImg[j], _refImg[j], pool_size);
                    }
                    break;
               case 5:
                    for(int j=0; j<_defImg.size(); ++j){
                         Feature feI(_defImg[j], _refImg[j]);
                         _feCal[i][j] = feI.maxNumBlackPixelsInBlock(thresh, block_size);
                    }
                    break;
               case 6:
                    for(int j=0; j<_defImg.size(); ++j){
                         Feature feI(_defImg[j], _refImg[j]);
                         _feCal[i][j] = feI.maxNumBlackPixelsInBlock(_defImg[j],thresh, block_size);
                    }
                    break;
               case 7:
                    for(int j=0; j<_defImg.size(); ++j){
                         Feature feI(_defImg[j], _refImg[j]);
                         _feCal[i][j] = feI.maxNumDiffPixelsInBlock(thresh, block_size);
                    }
                    break;
               default:
                    break;
          }
     }
}

void collectAttribute(vector< vector<string> > _inFile, vector< vector<string> >& _attTable){

	vector<string> __attTable;
	for(int i=0; i<5; ++i){
		__attTable.push_back(" ");
	}

	int typeID = -1;

	for(int i=0; i<_inFile[0].size(); ++i){
		if(_inFile[0][i].find("DEF_IMAGE") != std::string::npos || 
			_inFile[0][i].find("REF_IMAGE") != std::string::npos || 
			_inFile[0][i].find("DEFECTID") != std::string::npos)
			continue;
		else{
			++typeID;
			_attTable.push_back(__attTable);
			_attTable[typeID][0] = _inFile[0][i];
			for(int j=1; j<_inFile.size(); ++j){
				for(int k=1; k<_attTable[0].size(); ++k){
					if(_attTable[typeID][k] == _inFile[j][i]){
						break;
					}
					else if (_attTable[typeID][k] == " "){
						_attTable[typeID][k] = _inFile[j][i];
						break;
					}
					else
						continue; 
				}
			}
		}
	}
}

void overlap(vector< vector<string> > _inFile, vector< vector<string> > _attTable, 
	vector< vector<double> > _feCal, vector<int> _feIndex, 
	vector<string>& _oAttCol, vector<string>& _oAttRow, vector< vector<double> >& _oArea){

	overlap_col(_attTable, _oAttCol);
	overlap_row(_feIndex, _oAttRow);
	for(int i=0; i<_oAttRow.size(); ++i)
		cout << _oAttRow[i] << endl;
	for(int i=0; i<_oAttCol.size(); ++i)
		cout << _oAttCol[i] << endl;

	cout<< "in overlap"<< endl;


//	std::vector<std::string> x = split("one:two::three", ':');



}

void overlap_col	(vector< vector<string> > __attTable,vector<string>& __oAttCol ){

	for(int i=0; i<__attTable.size(); ++i){
		if(__attTable[i][0].find("Type") != std::string::npos){
			__oAttCol.push_back(__attTable[i][0]);
			continue;
		}
		else{
			for(int j=0; j<__attTable.size(); ++j){
				if(i == j)
					break;
				else if (__attTable[j][0].find("Type") != std::string::npos){
					string buf = __attTable[i][0]+","+__attTable[j][0];
					__oAttCol.push_back(buf);
					break;
				}
				else{
					string buf = __attTable[i][0]+","+__attTable[j][0]+","+"Type";
					__oAttCol.push_back(buf);
					break;
				}
			}
		}
	}
}

void overlap_row(vector<int> __feIndex, vector<string>& __oAttRow){
	for(int i=0; i<__feIndex.size(); ++i){
		string buf = "fe-" + int2string(i);
		__oAttRow.push_back(buf);
	}
}

string feName(int _index){
	string name;

          switch(_index){
               case 0:
                    name = ("diffGlobalMean");
                    break;
               case 1:
                    name = ("diffPixelsCount");
                    break;
               case 2:
                    name = ("getPSNR");
                    break;
               case 3:
                    name = ("localmeanSSE");
                    break;
               case 4:
                    name = ("localmeanSAE");
                    break;
               case 5:
                    name = ("maxNumBlackPixelsInBlock");
                    break;
               case 6:
                    name = ("maxNumBlackPixelsInBlock");
                    break;
               case 7:
                    name = ("maxNumDiffPixelsInBlock");
                    break;
               default:
                    break;
          }
     return name;
}

string int2string(int _in){
	stringstream ss;
	ss << _in;

	string convert_str;
	ss >>  convert_str;

	return convert_str;
}