
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

typedef struct _overArea{
	double ON = 0.0;	// over/type0
	double OD = 0.0;	// over/type1
	double OA = 0.0;	// over/all
}overArea;

void openCSV(char*, vector< vector<string> >&);
void openImg(vector< vector<string> > ,vector<Mat>&, vector<Mat>&);
void calibration(int, vector<Mat>&, vector<Mat>&);
void featureExtraction(vector<int>, vector<Mat>, vector<Mat>, vector< vector<double> >&);
void collectAttribute(vector< vector<string> >, vector< vector<string> >&);

void overlap(vector< vector<string> >, vector< vector<string> >, vector< vector<double> >, 
	vector<int>, vector<string>&, vector<string>&, vector< vector<overArea> >&);
void overlap_col	(vector< vector<string> >, vector<string>&);
void overlap_row(vector<int>, vector<string>&);
void calOverArea(vector<string>, vector<double>, overArea&);

void sortOAreaTb(vector<string>, vector<string>, vector< vector<overArea> >,
 vector< vector<string> >&, vector< vector<string> >&, vector< vector<string> >&,
  vector< vector<double> >&, vector< vector<double> >&, vector< vector<double> >&);
void sorFinit(vector<string>, vector<string>, vector< vector<string> >&);
void sorVinit(vector< vector<overArea> >, vector< vector<double> >&, vector< vector<double> >&, vector< vector<double> >&);
void sortswap(double&, double&, string&, string&);

void showRank( vector<string>, vector<string>, vector< vector<string> >, vector< vector<string> >, vector< vector<string> >,
  vector< vector<double> >, vector< vector<double> >, vector< vector<double> >);

string feName(int);
string int2string(int);
vector<string> split(const string& , const char& );


int main(int argc, char* argv[]){

	vector< vector<string> > inFile;
	char* fileName = argv[1];
	openCSV(fileName, inFile);

	vector<Mat> defImg, refImg;
	openImg(inFile, defImg, refImg);

	calibration(5, defImg, refImg);
	
	vector<int> feIndex;
	feIndex.push_back(1);
	feIndex.push_back(3);
	feIndex.push_back(5);
	
	vector< vector<double> > feCal;
	featureExtraction(feIndex, defImg, refImg, feCal);

	vector< vector<string> > attTable;
	collectAttribute(inFile, attTable);
	
	vector<string> oAttCol, oAttRow;
	vector< vector<overArea> > oArea;

	overlap(inFile, attTable, feCal, feIndex, oAttCol, oAttRow, oArea);

	vector< vector<string> > sorfON, sorfOD, sorfOA;
	vector< vector<double> > sorVON, sorVOD, sorVOA;		
	sortOAreaTb(oAttCol, oAttRow, oArea, sorfON, sorfOD, sorfOA, sorVON, sorVOD, sorVOA);

	showRank(oAttCol, oAttRow, sorfON, sorfOD, sorfOA, sorVON, sorVOD, sorVOA);
/*
	for(int i=0; i<attTable.size(); ++i){
		for(int  j=0; j<attTable[0].size(); ++j){
			cout << attTable[i][j] << " ";
		}
		cout << endl;
	}
*/
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
	vector<string>& _oAttCol, vector<string>& _oAttRow, vector< vector<overArea> >& _oArea){

	overlap_col(_attTable, _oAttCol);
	overlap_row(_feIndex, _oAttRow);

	vector<overArea> __oArea; 
	overArea tmp;
	for(int i=0; i<_oAttRow.size(); ++i)
		__oArea.push_back(tmp);
	for(int i=0; i<_oAttCol.size(); ++i)
		_oArea.push_back(__oArea);

	vector<string> TypeRow;	// collect ID = 0 or 1
	vector<double> TypeVal;	// collect TyepVal
	vector<string> typeID;
	int TypeCol0 = 0, TypeCol1 = 0, TypeCol2 = 0;

	for(int i=0; i<_oAttCol.size(); ++i){

		TypeCol0 = 0; TypeCol1 = 0; TypeCol2 = 0;
		typeID.clear();
		typeID = split(_oAttCol[i], ',');

		switch(typeID.size()){
			case 1:	// type only
			{
				for(int j=0; j<_inFile[0].size(); ++j){
					if(_inFile[0][j] == typeID[0]){
						TypeCol0 = j;
						break;
					}
				}
				for(int j=0; j<_feIndex.size(); ++j){
					TypeRow.clear();
					TypeVal.clear();
						
					for(int k=1; k<_inFile.size(); ++k){
						TypeRow.push_back(_inFile[k][TypeCol0]);
						TypeVal.push_back(_feCal[j][k-1]);	
					}
					calOverArea(TypeRow, TypeVal, _oArea[i][j]);	
				}
			}
				break;

			case 2:	// one attrib + only
			{
				vector<string> type0, type1;
				type0 = split(typeID[0], '@');
				type1 = split(typeID[1], '@');

				for(int j=0; j<_inFile[0].size(); ++j){
					if(_inFile[0][j] == type0[0]){
						TypeCol0 = j;
					}
					else if(_inFile[0][j] == type1[0]){
						TypeCol1 = j;
					}
				}

				for(int j=0; j<_feIndex.size(); ++j){
					TypeRow.clear();
					TypeVal.clear();

					for(int k=1; k<_inFile.size(); ++k){
						if(_inFile[k][TypeCol0] == type0[1]){
							TypeRow.push_back(_inFile[k][TypeCol1]);
							TypeVal.push_back(_feCal[j][k-1]);
						}
					}
					calOverArea(TypeRow, TypeVal, _oArea[i][j]);
				}
			}
				break;

			case 3:
			{
				vector<string> type0, type1, type2;
				type0 = split(typeID[0], '@');
				type1 = split(typeID[1], '@');
				type2 = split(typeID[2], '@');
			
				for(int j=0; j<_inFile[0].size(); ++j){
					if(_inFile[0][j] == type0[0]){
						TypeCol0 = j;
					}
					else if(_inFile[0][j] == type1[0]){
						TypeCol1 = j;
					}
					else if(_inFile[0][j] == type2[0]){
						TypeCol2 = j;
					}
				}

				for(int j=0; j<_feIndex.size(); ++j){
					TypeRow.clear();
					TypeVal.clear();

					for(int k=1; k<_inFile.size(); ++k){
						if(_inFile[k][TypeCol0] == type0[1] &&
							_inFile[k][TypeCol1] == type1[1]){

							TypeRow.push_back(_inFile[k][TypeCol2]);
							TypeVal.push_back(_feCal[j][k-1]);

						}
					}
					calOverArea(TypeRow, TypeVal, _oArea[i][j]);
				}
			}
				break;
			default:
				cout << "Error: no match case in overlap" << endl;
				break;
		}
	}
}

void overlap_col	(vector< vector<string> > __attTable,vector<string>& __oAttCol ){
	string buf;

	for(int i=0; i<__attTable.size(); ++i){
		if(__attTable[i][0].find("Type") != std::string::npos){
			__oAttCol.push_back(__attTable[i][0]);
		}
		else{
			for(int j=i; j<__attTable.size(); ++j){
				if(i == j)
					continue;
				else if (__attTable[j][0].find("Type") != std::string::npos){
					for(int k=1; k<__attTable[i].size(); ++k){
						if( __attTable[i][k] == " ")
							break;
						buf = __attTable[i][0]+"@"+__attTable[i][k]+","+__attTable[j][0];
						__oAttCol.push_back(buf);
					}
				}
				else{
					for(int k=1; k<__attTable[i].size(); ++k){
						for(int l=1; l<__attTable[j].size(); ++l){
							if(__attTable[i][k] == " "||__attTable[j][l] == " ")
								break;
							buf = __attTable[i][0]+"@"+__attTable[i][k]+","+
							__attTable[j][0]+"@"+__attTable[j][l]+","+"Type";
							__oAttCol.push_back(buf);
						}
					}
				}
			}
		}
	}
}

void overlap_row(vector<int> __feIndex, vector<string>& __oAttRow){
	for(int i=0; i<__feIndex.size(); ++i){
		string buf = "fe-" + int2string(__feIndex[i]);
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

vector<string> split(const string& source, const char& delim){

    stringstream ss(source);
    string token;
    vector<string> ans;
    while (getline(ss, token, delim))
        ans.push_back(token);
    return ans;
}

void calOverArea(vector<string> _typeRow, vector<double> _typeVal, overArea& rst){

	if(_typeRow.size() != _typeVal.size()){
		cout << "Error: in calOverArea, _typeRow.size() != _typeVal.size()" << endl;
		exit(0);
	}

	vector<double> type0Val, type1Val;
	for(int i=0; i<_typeRow.size(); ++i){
		if(_typeRow[i] == "0"){
			type0Val.push_back(_typeVal[i]);
		}
		else if(_typeRow[i] == "1"){
			type1Val.push_back(_typeVal[i]);
		}
		else{
			cout << "Error: the type more than 2 kind of value." << endl;
			exit(0);
		}
	}

	double maxVal = 0;
	for(int i=0; i<type0Val.size(); ++i)
		maxVal = (maxVal>type0Val[i])?maxVal:type0Val[i];
	for(int i=0; i<type1Val.size(); ++i)
		maxVal = (maxVal>type0Val[i])?maxVal:type0Val[i];
	maxVal = maxVal + 5;

	vector<int> overArea, t0Area, t1Area;
	for(int i=0; i<maxVal; ++i){
		overArea.push_back(0);
		t0Area.push_back(0);
		t1Area.push_back(0);
	}

	for(int i=0; i<type0Val.size(); ++i)
		t0Area[round(type0Val[i])]++;
	for(int i=0; i<type1Val.size(); ++i)
		t1Area[round(type1Val[i])]++;
	for(int i=0; i<maxVal; ++i)
		overArea[i] = (t0Area[i] < t1Area[i])?(t0Area[i]):(t1Area[i]);

	double sumOverArea = 0., sumT0 = 0., sumT1 = 0.;
	for(int i=0; i<maxVal; ++i){
		sumOverArea += overArea[i];
		sumT0 += t0Area[i];
		sumT1 += t1Area[i];
	}

	if(sumOverArea == 0){
		rst.ON = 0;
		rst.OD = 0;
		rst.OA = 0;
	}
	else if(sumT0 == 0){
		rst.ON = 0;
		rst.OD = 100.* sumOverArea / sumT1;
		rst.OA = 100.* sumOverArea / (sumT0 + sumT1 - sumOverArea); 
	}
	else if(sumT1 == 0){
		rst.ON = 100.* sumOverArea / sumT0;
		rst.OD = 0;
		rst.OA = 100.* sumOverArea / (sumT0 + sumT1 - sumOverArea); 
	}
	else{
		rst.ON = 100.* sumOverArea / sumT0;
		rst.OD = 100.* sumOverArea / sumT1;
		rst.OA = 100.* sumOverArea / (sumT0 + sumT1 - sumOverArea); 
	}
}

void sortOAreaTb(vector<string> _oAttCol, vector<string> _oAttRow, vector< vector<overArea> > _oArea,
 vector< vector<string> >& _sorfON, vector< vector<string> >& _sorfOD, vector< vector<string> >& _sorfOA,
  vector< vector<double> >& _sorVON, vector< vector<double> >& _sorVOD, vector< vector<double> >& _sorVOA){

	sorFinit(_oAttCol, _oAttRow, _sorfON);
	sorFinit(_oAttCol, _oAttRow, _sorfOD);
	sorFinit(_oAttCol, _oAttRow, _sorfOA);

	sorVinit(_oArea, _sorVON, _sorVOD, _sorVOA);

	for(int i=0; i<_oAttCol.size(); ++i){
		for(int j=0; j<_oAttRow.size(); ++j){
			for(int k=j; k<_oAttRow.size(); ++k){
				if(_sorVON[i][j] > _sorVON[i][k]){
					sortswap(_sorVON[i][j], _sorVON[i][k], _sorfON[i][j], _sorfON[i][k]);
				}

				if(_sorVOD[i][j] > _sorVOD[i][k]){
					sortswap(_sorVOD[i][j], _sorVOD[i][k], _sorfOD[i][j], _sorfOD[i][k]);
				}
					
				if(_sorVOA[i][j] > _sorVOA[i][k]){
					sortswap(_sorVOA[i][j], _sorVOA[i][k], _sorfOA[i][j], _sorfOA[i][k]);
				}					
			}
		}
	}
}

void sorFinit(vector<string> __oAttCol, vector<string> __oAttRow, vector< vector<string> >& __sorfTable){
	vector<string> __lsorfTable;
	for(int i=0; i<__oAttRow.size(); ++i){
		__lsorfTable.push_back(__oAttRow[i]);
	}
	for(int i=0; i<__oAttCol.size(); ++i){
		__sorfTable.push_back(__lsorfTable);
	}
}

void sorVinit(vector< vector<overArea> > __oArea, vector< vector<double> >& __sorvON, vector< vector<double> >& __sorvOD, vector< vector<double> >& __sorvOA){
	
	vector<double> __lsorv;
	for(int i=0; i<__oArea[0].size(); ++i){
		__lsorv.push_back(0);
	}

	for(int i=0; i<__oArea.size(); ++i){
		__sorvON.push_back(__lsorv);
		__sorvOD.push_back(__lsorv);
		__sorvOA.push_back(__lsorv);
	}

	for(int i=0; i<__oArea.size(); ++i){
		for(int j=0; j<__oArea[0].size(); ++j){
			__sorvON[i][j] = __oArea[i][j].ON;
			__sorvOD[i][j] = __oArea[i][j].OD;
			__sorvOA[i][j] = __oArea[i][j].OA;
		}
	}
}

void sortswap(double& val0, double& val1, string& tex0, string& tex1){
	double tmpv = 0;
	string tmpt = "";

	tmpv = val0; val0 = val1; val1 = tmpv;
	tmpt = tex0; tex0 = tex1; tex1 = tmpt;
}

void showRank( vector<string> _oAttCol, vector<string> _oAttRow, vector< vector<string> > _sorfON, vector< vector<string> > _sorfOD, vector< vector<string> > _sorfOA,
  vector< vector<double> > _sorVON, vector< vector<double> > _sorVOD, vector< vector<double> > _sorVOA){
	
	cout << "\t\t\t" << "/* --- overlap/N ---*/" << "\t\t\t" << endl;
	cout << "\tAttribute\t"<<"\t"<<"NO.1"<<"\t\t"<<"NO.2"<<"\t\t"<<"NO.3"<<endl; 
	for(int i=0; i<_oAttCol.size(); ++i){
		if(_oAttCol[i].size() < 20 &&_oAttCol[i].size() > 13)
			cout << _oAttCol[i] <<"\t\t";
		else if(_oAttCol[i].size() < 13 &&_oAttCol[i].size() > 7)
			cout << _oAttCol[i] <<"\t\t\t";
		else if(_oAttCol[i].size() < 7)
			cout << _oAttCol[i] <<"\t\t\t\t";
		else
			cout << _oAttCol[i] <<"\t";
		cout <<  _sorfON[i][0] << "("<< _sorVON[i][0]<< "%)"<<"\t";
		cout <<  _sorfON[i][1] << "("<< _sorVON[i][1]<< "%)"<<"\t";
		cout <<  _sorfON[i][2] << "("<< _sorVON[i][2]<< "%)"<<"\t";
		cout << endl;

	}

	cout << "\t\t\t" << "/* --- overlap/D ---*/" << "\t\t\t" << endl;
	cout << "\tAttribute\t"<<"\t"<<"NO.1"<<"\t\t"<<"NO.2"<<"\t\t"<<"NO.3"<<endl; 
	for(int i=0; i<_oAttCol.size(); ++i){
		if(_oAttCol[i].size() < 20 &&_oAttCol[i].size() > 13)
			cout << _oAttCol[i] <<"\t\t";
		else if(_oAttCol[i].size() < 13 &&_oAttCol[i].size() > 7)
			cout << _oAttCol[i] <<"\t\t\t";
		else if(_oAttCol[i].size() < 7)
			cout << _oAttCol[i] <<"\t\t\t\t";
		else
			cout << _oAttCol[i] <<"\t";
		cout <<  _sorfOD[i][0] << "("<< _sorVOD[i][0]<< "%)"<<"\t";
		cout <<  _sorfOD[i][1] << "("<< _sorVOD[i][1]<< "%)"<<"\t";
		cout <<  _sorfOD[i][2] << "("<< _sorVOD[i][2]<< "%)"<<"\t";
		cout << endl;

	}

	cout << "\t\t\t" << "/* --- overlap/ALL ---*/" << "\t\t\t" << endl;
	cout << "\tAttribute\t"<<"\t"<<"NO.1"<<"\t\t"<<"NO.2"<<"\t\t"<<"NO.3"<<endl; 
	for(int i=0; i<_oAttCol.size(); ++i){
		if(_oAttCol[i].size() < 20 &&_oAttCol[i].size() > 13)
			cout << _oAttCol[i] <<"\t\t";
		else if(_oAttCol[i].size() < 13 &&_oAttCol[i].size() > 7)
			cout << _oAttCol[i] <<"\t\t\t";
		else if(_oAttCol[i].size() < 7)
			cout << _oAttCol[i] <<"\t\t\t\t";
		else
			cout << _oAttCol[i] <<"\t";
		cout <<  _sorfOA[i][0] << "("<< _sorVOA[i][0]<< "%)"<<"\t";
		cout <<  _sorfOA[i][1] << "("<< _sorVOA[i][1]<< "%)"<<"\t";
		cout <<  _sorfOA[i][2] << "("<< _sorVOA[i][2]<< "%)"<<"\t";
		cout << endl;
	}

	vector<double> feCount;
	vector<string> feSort;
	for(int i=0; i<_oAttRow.size(); ++i){
		feCount.push_back(0);
		feSort.push_back(_oAttRow[i]);
	}
		
	int firstThree = 2;

	for(int i=0; i<_oAttCol.size(); ++i){
		for(int j=0; j<firstThree; ++j){
			for(int k=0; k<_oAttRow.size(); ++k){
				if(_sorfON[i][j] == _oAttRow[k])
					++feCount[k];
				if(_sorfOD[i][j] == _oAttRow[k])
					++feCount[k];
				if(_sorfOA[i][j] == _oAttRow[k])
					++feCount[k];
			}
		}
	}

	for(int i=0; i<_oAttRow.size(); ++i){
		for(int j=i; j<_oAttRow.size(); ++j){
			if(feCount[i]<feCount[j]){
				sortswap(feCount[i], feCount[j], feSort[i], feSort[j]);
			}
		}
	}

	cout << "Best Feature: ";
	for(int i=0; i<firstThree; ++i)
		cout << feSort[i] << " ";
	cout << endl;


	






}

