
/*
version v1.0
date: 2017/08/11
purpose: to verify feature effect work on defect image.
usage: ./out {csv path} {calibration Name}

to do thing: 1. openImg check, 2. caloverArea, Normalize. 3. kernal density estimation(to code project)

doing thing: 1.sort Attrible( sortAttribute Func). 2.openImg func(removeFileRow) 3. caloverArea Normalize
*/

#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include "include/alignment.h"
#include "include/feature.h"
#define FeatureN 51

using namespace cv;
using namespace std;

typedef struct _overArea{
	double ON = 0.0;	// over/type0
	double OD = 0.0;	// over/type1
	double OA = 0.0;	// over/all
}overArea;

typedef struct _ndnum{
    string attName;
    int nNum = 0;
	int dNum = 0;
	int nd = nNum + dNum;
}ndNum;

void openCSV(char*, vector< vector<string> >&);
void openImg(vector< vector<string> > ,vector<Mat>&, vector<Mat>&);
void eraseSpace(string&);
void removeFileRow(vector< vector<string> >&, vector<int>);
void calibration(char*, char*, string&);
void featureExtraction(vector<int>, vector<Mat>, vector<Mat>, vector< vector<double> >&);
void collectAttribute(vector< vector<string> >, vector< vector<string> >&);
void sortAttribute(vector< vector<string> >& );

void overlap(vector< vector<string> >, vector< vector<string> >, vector< vector<double> >, 
	vector<int>, vector<string>&, vector<string>&, vector< vector<overArea> >&, vector<ndNum>&);
void overlap_col	(vector< vector<string> >, vector<string>&);
void overlap_row(vector<int>, vector<string>&);
void calOverArea(vector<string>, vector<double>, overArea&);
void countND(string, vector<string>, ndNum&);

void sortOAreaTb(vector<string>, vector<string>, vector< vector<overArea> >,
 vector< vector<string> >&, vector< vector<string> >&, vector< vector<string> >&,
  vector< vector<double> >&, vector< vector<double> >&, vector< vector<double> >&);
void sorFinit(vector<string>, vector<string>, vector< vector<string> >&);
void sorVinit(vector< vector<overArea> >, vector< vector<double> >&, vector< vector<double> >&, vector< vector<double> >&);
void sortswap(double&, double&, string&, string&);

void showRank( vector<string>, vector<string>, vector< vector<string> >, vector< vector<string> >, vector< vector<string> >,
  vector< vector<double> >, vector< vector<double> >, vector< vector<double> >, vector<ndNum>);
void changeAttName(vector<string>&);

void help();
void openAllFe(int, vector<int>&);
string feName(int);
string int2string(int);
vector<string> split(const string& , const char& );
string currentDateTime();
void progressBar(string, int, int, bool);
void writeCSV(vector< vector<double> >, vector<string>, vector< vector<string> >);

int main(int argc, char* argv[]){
	

	if(argc!=3){ help(); exit(0);}

	char* fileName = argv[1];
	char* calN = argv[2];

	string outfile;
	calibration(calN, fileName, outfile);

	char* _fileName = &outfile[0u];


	vector< vector<string> > inFile;
	openCSV(_fileName, inFile);

	vector<Mat> defImg, refImg;
	openImg(inFile, defImg, refImg);
	
	vector<int> feIndex;
	openAllFe(FeatureN, feIndex);
	
	vector< vector<double> > feCal;
	featureExtraction(feIndex, defImg, refImg, feCal);

	vector< vector<string> > attTable;
	collectAttribute(inFile, attTable);

	vector<string> oAttCol, oAttRow;
	vector< vector<overArea> > oArea;
	vector<ndNum> ndCount;
	overlap(inFile, attTable, feCal, feIndex, oAttCol, oAttRow, oArea, ndCount);
	
	vector< vector<string> > sorfON, sorfOD, sorfOA;
	vector< vector<double> > sorVON, sorVOD, sorVOA;		
	sortOAreaTb(oAttCol, oAttRow, oArea, sorfON, sorfOD, sorfOA, sorVON, sorVOD, sorVOA);

	showRank(oAttCol, oAttRow, sorfON, sorfOD, sorfOA, sorVON, sorVOD, sorVOA, ndCount);

	writeCSV(feCal, oAttRow, inFile);
	
	return 0;
}


void calibration(char* _calName, char* _fileName, string& _outfile){
	
	Alignment a;
	int num_kept_defect, num_kept_nuisance;
	// ICFilter(calibration_Name, csv_in_path, csv_out_path, num_kept_defect, num_kept_nuisance, true:not show fail data, false: show in csv(ECC_label=-1))
	a.ICFilter(_calName, 0.9, _fileName, _outfile, num_kept_defect, num_kept_nuisance, false);
}

void openCSV(char* _fileName, vector< vector<string> >& _inFile){

	string str1, str2;
	ifstream theFile(_fileName);

	 if(!theFile.is_open()){
	 	cout << "Error in opencsv: csv file is not open..\nmain.out file.csv"<< endl;
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

void eraseSpace(string& str){
	string buf = str;
	str = "";
	for(int i=0; i<buf.size(); ++i){
		if(buf[i] == ' ')
			continue;
		else{
			str += buf[i];
		}
	}
}

void openImg(vector< vector<string> > _inFile,vector<Mat>& _defImg, vector<Mat>& _refImg){

	int defCol = 0, refCol = 0, keptImg = 0;

	for(int i=0; i<_inFile[0].size(); ++i){
		if(_inFile[0][i].find("DEF_IMAGE") != std::string::npos)
			defCol = i;
		if(_inFile[0][i].find("REF_IMAGE") != std::string::npos)
			refCol = i;
		if(_inFile[0][i].find("ECC_result") != std::string::npos)
			keptImg = i;
	}

	if(defCol == 0|| refCol == 0|| keptImg == 0){
		cout << "Error in openImg: defCol == 0|| refCol == 0|| keptImg == 0" << endl;
		exit(0);
	}
	string defName;
	string refName;
	Mat defbuf;
	Mat refbuf;
	vector< int > rowIndex;

	for(int i=1; i<_inFile.size(); ++i){
		progressBar("open Image", i, _inFile.size(),false);
		if(_inFile[i][keptImg] != "0"){
			cout << "kept" << i << endl;
			continue;
		}

		defName = _inFile[i][defCol];
		refName = _inFile[i][refCol];
		eraseSpace(defName);
		eraseSpace(refName);
		
		defbuf = imread(defName,CV_LOAD_IMAGE_GRAYSCALE);
		refbuf = imread(refName,CV_LOAD_IMAGE_GRAYSCALE);

		if(!defbuf.data){
			cout << "Error in openImg: DEF image " << i << " is not open."<< endl;
			cout << "defName Path:" << defName << endl;
			rowIndex.push_back(i);
			continue;
		}
		else if(!refbuf.data){
			cout << "Error in openImg: REF image " << i << " is not open."<< endl;
			cout << "refName Path:" << refName << endl;
			rowIndex.push_back(i);
			continue;
		}
		_defImg.push_back(defbuf.clone());
		_refImg.push_back(refbuf.clone());
		
		defbuf = Mat::zeros(defbuf.size(), defbuf.type());
		refbuf = Mat::zeros(refbuf.size(), refbuf.type());
	}
	removeFileRow(_inFile,rowIndex);
	progressBar("open Image", _inFile.size(), _inFile.size(),true);
}

void removeFileRow(vector< vector<string> >& __inFile, vector<int> __index){
	
	if(__index.size() > 0){
		
		vector< vector<string> > bufFile;
		vector<string> _bufFile;

		for(int i=0; i<__inFile[0].size(); ++i)
			_bufFile.push_back(" ");
		for(int i=0; i<__inFile.size(); ++i)
			bufFile.push_back(_bufFile);

		for(int i=0; i<__inFile.size(); ++i){
			for(int j=0; j<__inFile[0].size(); ++j){
				bufFile[i][j] = __inFile[i][j];
			}
			__inFile[i].clear();
		}
		__inFile.clear();
	
		int k=0;
		for(int i=0; i<bufFile.size(); ++i){
			if(i == __index[k]){
				++k;
				continue;
			}
			else{
				__inFile.push_back(bufFile[i]);
			}
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
			_inFile[0][i].find("DEFECTID") != std::string::npos||
			_inFile[0][i].find("ECC_result") != std::string::npos||
			_inFile[0][i].find("LOTID") != std::string::npos)
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

	sortAttribute(_attTable);
}

void sortAttribute(vector< vector<string> >& __attTable){
	
	int val1 = 0, val2 = 0;
	string bufString;
	for(int i=0; i<__attTable.size(); ++i){
		for(int j=1; j<__attTable[i].size(); ++j){	
			for(int k=j+1; k<__attTable[i].size(); ++k){
				if(__attTable[i][j] == " "||__attTable[i][k] == " "){
					break;
				}
				else{
					val1 = 0;
					val2 = 0;
					val1 = atoi(__attTable[i][j].c_str());
					val2 = atoi(__attTable[i][k].c_str());
					if(val1 > val2){
						bufString = __attTable[i][k]; 
						__attTable[i][k] = __attTable[i][j];
						__attTable[i][j] = bufString;
					}
				}
			}
		}
	}

	for(int i=0; i<__attTable.size(); ++i){
		for(int j=i+1; j<__attTable.size(); ++j){
			if(__attTable[i][0].size() > __attTable[j][0].size() || 
				__attTable[j][0] == "Type"||
				__attTable[j][0] == "TYPE"){
				for(int k=0; k<__attTable[0].size(); ++k){
					bufString = __attTable[i][k];
					__attTable[i][k] = __attTable[j][k];
					__attTable[j][k] = bufString;
				}
			}
		}
	}
}

void overlap(vector< vector<string> > _inFile, vector< vector<string> > _attTable, 
	vector< vector<double> > _feCal, vector<int> _feIndex, 
	vector<string>& _oAttCol, vector<string>& _oAttRow, vector< vector<overArea> >& _oArea, vector<ndNum>& _ndCount ){

	overlap_col(_attTable, _oAttCol);
	overlap_row(_feIndex, _oAttRow);

	vector<overArea> __oArea; 
	overArea tmp;
	for(int i=0; i<_oAttRow.size(); ++i)
		__oArea.push_back(tmp);
	for(int i=0; i<_oAttCol.size(); ++i)
		_oArea.push_back(__oArea);

	ndNum tmp2;
	for(int i=0; i<_oAttCol.size(); ++i)
		_ndCount.push_back(tmp2);

	vector<string> TypeRow;	// collect ID = 0 or 1
	vector<double> TypeVal;	// collect TyepVal
	vector<string> typeID;
	int TypeCol0 = 0, TypeCol1 = 0, TypeCol2 = 0;

	for(int i=0; i<_oAttCol.size(); ++i){
		progressBar("overlap area", i, _oAttCol.size(),false);

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
				cout << "Error in overlap: no match case in overlap" << endl;
				break;
		}
		countND(_oAttCol[i], TypeRow, _ndCount[i]);
	}
	progressBar("overlap area", _oAttCol.size(), _oAttCol.size(),true);
	
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
		string buf = feName(i);
		__oAttRow.push_back(buf);
	}
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

void countND(string label, vector<string> _typeRow, ndNum& rst){

	int nSum = 0, dSum = 0;
	for(int i=0; i<_typeRow.size(); i++){
		if(_typeRow[i] == "0")
			++nSum;
		else if (_typeRow[i] == "1")
			++dSum;
		else{
			cout << "Error in countND: the type more than 2 kind of value." << endl;
			exit(0);
		}
	}

	rst.attName = label;
	rst.nNum = nSum;
	rst.dNum = dSum;
	rst.nd = nSum + dSum;
}

void calOverArea(vector<string> _typeRow, vector<double> _typeVal, overArea& rst){
	if(_typeRow.size() != _typeVal.size()){
		cout << "Error in calOverArea: in calOverArea, _typeRow.size() != _typeVal.size()" << endl;
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
			cout << "Error in calOverArea: the type more than 2 kind of value." << endl;
			exit(0);
		}
	}

	int bin = 20;
	double interval = 0;
	double minVal = 100000.0,maxVal = 0.0;
	for(int i=0; i<type0Val.size(); ++i){
		maxVal = (maxVal>type0Val[i])?maxVal:type0Val[i];
		minVal = (minVal<type0Val[i])?minVal:type0Val[i];
	}

	for(int i=0; i<type1Val.size(); ++i){
		maxVal = (maxVal>type1Val[i])?maxVal:type1Val[i];
		minVal = (minVal<type1Val[i])?minVal:type1Val[i];
	}
	if(maxVal == 0) maxVal++;
	interval = fabs(maxVal - minVal)/(double)bin + 1;

	vector<double> overArea, t0Area, t1Area;
	for(int i=0; i<abs(maxVal/interval); ++i){
		overArea.push_back(0.0);
		t0Area.push_back(0.0);
		t1Area.push_back(0.0);
	}

	for(int i=0; i<type0Val.size(); ++i)
		t0Area[round(type0Val[i]/interval)]++;
	for(int i=0; i<type1Val.size(); ++i)
		t1Area[round(type1Val[i]/interval)]++;
	
	double sumOverArea = 0., sumT0 = 0., sumT1 = 0.;
	for(int i=0; i<overArea.size(); ++i){
		sumT0 += t0Area[i];
		sumT1 += t1Area[i];
	}

	for(int i=0; i<overArea.size(); ++i){
		t0Area[i] = t0Area[i]/sumT0;
		t1Area[i] = t1Area[i]/sumT1;
		overArea[i] = (t0Area[i] < t1Area[i])?(t0Area[i]):(t1Area[i]);
	}

	sumT0 = 0.0; sumT1 = 0.0;	
	for(int i=0; i<overArea.size(); ++i){
		sumT0 += t0Area[i];
		sumT1 += t1Area[i];
		sumOverArea += overArea[i];
	}
	
	type0Val.clear();
	type1Val.clear();
	overArea.clear();
	t0Area.clear();
	t1Area.clear();

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
  vector< vector<double> > _sorVON, vector< vector<double> > _sorVOD, vector< vector<double> > _sorVOA, vector<ndNum> _ndCount){
	
	fstream file;
	string name = currentDateTime() + ".txt";
	file.open(name, ios::out);

	changeAttName(_oAttCol);
	cout << fixed  <<  setprecision(2);
	file << fixed  <<  setprecision(2);

	cout << "\n\t\t\t" << "/* --- overlap/N ---*/" << "\t\t\t" << endl;
	cout << "\tAttribute\t"<<"\t"<<"NO.1"<<"\t\t"<<"NO.2"<<"\t\t"<<"NO.3"<<endl; 
	file << "\n\t\t\t" << "/* --- overlap/N ---*/" << "\t\t\t" << endl;
	file << "\tAttribute\t"<<"\t"<<"NO.1"<<"\t\t"<<"NO.2"<<"\t\t"<<"NO.3"<<endl;
	for(int i=0; i<_oAttCol.size(); ++i){
		if(_oAttCol[i].size() < 20 &&_oAttCol[i].size() > 13){
			cout << _oAttCol[i] <<"\t\t";
			file << _oAttCol[i] <<"\t\t";
		}	
		else if(_oAttCol[i].size() < 13 &&_oAttCol[i].size() > 7){
			cout << _oAttCol[i] <<"\t\t\t";
			file << _oAttCol[i] <<"\t\t\t";
		}
		else if(_oAttCol[i].size() < 7){
			cout << _oAttCol[i] <<"\t\t\t\t";
			file << _oAttCol[i] <<"\t\t\t\t";
		}
		else{
			cout << _oAttCol[i] <<"\t";
			file << _oAttCol[i] <<"\t";
		}	
		cout <<  _sorfON[i][0] << "("<< _sorVON[i][0]<< "%)"<<"\t";
		cout <<  _sorfON[i][1] << "("<< _sorVON[i][1]<< "%)"<<"\t";
		cout <<  _sorfON[i][2] << "("<< _sorVON[i][2]<< "%)"<<"\t";
		cout << endl;

		file <<  _sorfON[i][0] << "("<< _sorVON[i][0]<< "%)"<<"\t";
		file <<  _sorfON[i][1] << "("<< _sorVON[i][1]<< "%)"<<"\t";
		file <<  _sorfON[i][2] << "("<< _sorVON[i][2]<< "%)"<<"\t";
		file << endl;
	}

	cout << "\n\t\t\t" << "/* --- overlap/D ---*/" << "\t\t\t" << endl;
	cout << "\tAttribute\t"<<"\t"<<"NO.1"<<"\t\t"<<"NO.2"<<"\t\t"<<"NO.3"<<endl; 
	file << "\n\t\t\t" << "/* --- overlap/D ---*/" << "\t\t\t" << endl;
	file << "\tAttribute\t"<<"\t"<<"NO.1"<<"\t\t"<<"NO.2"<<"\t\t"<<"NO.3"<<endl; 

	for(int i=0; i<_oAttCol.size(); ++i){
		if(_oAttCol[i].size() < 20 &&_oAttCol[i].size() > 13){
			cout << _oAttCol[i] <<"\t\t";
			file << _oAttCol[i] <<"\t\t";
		}
		else if(_oAttCol[i].size() < 13 &&_oAttCol[i].size() > 7){
			cout << _oAttCol[i] <<"\t\t\t";
			file << _oAttCol[i] <<"\t\t\t";
		}	
		else if(_oAttCol[i].size() < 7){
			cout << _oAttCol[i] <<"\t\t\t\t";
			file << _oAttCol[i] <<"\t\t\t\t";
		}	
		else{
			cout << _oAttCol[i] <<"\t";
			file << _oAttCol[i] <<"\t";
		}
			
		cout <<  _sorfOD[i][0] << "("<< _sorVOD[i][0]<< "%)"<<"\t";
		cout <<  _sorfOD[i][1] << "("<< _sorVOD[i][1]<< "%)"<<"\t";
		cout <<  _sorfOD[i][2] << "("<< _sorVOD[i][2]<< "%)"<<"\t";
		cout << endl;

		file <<  _sorfOD[i][0] << "("<< _sorVOD[i][0]<< "%)"<<"\t";
		file <<  _sorfOD[i][1] << "("<< _sorVOD[i][1]<< "%)"<<"\t";
		file <<  _sorfOD[i][2] << "("<< _sorVOD[i][2]<< "%)"<<"\t";
		file << endl;
	}

	cout << "\n\t\t\t" << "/* --- overlap/ALL ---*/" << "\t\t\t" << endl;
	cout << "\tAttribute\t"<<"\t"<<"NO.1"<<"\t\t"<<"NO.2"<<"\t\t"<<"NO.3"<<endl; 
	file << "\n\t\t\t" << "/* --- overlap/ALL ---*/" << "\t\t\t" << endl;
	file << "\tAttribute\t"<<"\t"<<"NO.1"<<"\t\t"<<"NO.2"<<"\t\t"<<"NO.3"<<endl; 
	for(int i=0; i<_oAttCol.size(); ++i){
		if(_oAttCol[i].size() < 20 &&_oAttCol[i].size() > 13){
			cout << _oAttCol[i] <<"\t\t";
			file << _oAttCol[i] <<"\t\t";
		}
		else if(_oAttCol[i].size() < 13 &&_oAttCol[i].size() > 7){
			cout << _oAttCol[i] <<"\t\t\t";
			file << _oAttCol[i] <<"\t\t\t";
		}
		else if(_oAttCol[i].size() < 7){
			cout << _oAttCol[i] <<"\t\t\t\t";
			file << _oAttCol[i] <<"\t\t\t\t";
		}
		else{
			cout << _oAttCol[i] <<"\t";
			file << _oAttCol[i] <<"\t";
		}
			
		cout <<  _sorfOA[i][0] << "("<< _sorVOA[i][0]<< "%)"<<"\t";
		cout <<  _sorfOA[i][1] << "("<< _sorVOA[i][1]<< "%)"<<"\t";
		cout <<  _sorfOA[i][2] << "("<< _sorVOA[i][2]<< "%)"<<"\t";
		cout << endl;

		file <<  _sorfOA[i][0] << "("<< _sorVOA[i][0]<< "%)"<<"\t";
		file <<  _sorfOA[i][1] << "("<< _sorVOA[i][1]<< "%)"<<"\t";
		file <<  _sorfOA[i][2] << "("<< _sorVOA[i][2]<< "%)"<<"\t";
		file << endl;
	}

	/* --- ND count show --- */
	cout << "\n\t\t\t" << "/* --- N/D Number ---*/" << "\t\t\t" << endl;
	cout << "\tAttribute\t"<<"\t"<<"N num"<<"\t"<<"D num"<<"\t"<<"total"<<endl; 
	file << "\n\t\t\t" << "/* --- N/D Number ---*/" << "\t\t\t" << endl;
	file << "\tAttribute\t"<<"\t"<<"N num"<<"\t"<<"D num"<<"\t"<<"total"<<endl; 
	for(int i=0; i<_oAttCol.size(); ++i){
		if(_oAttCol[i].size() < 20 &&_oAttCol[i].size() > 13){
			cout << _oAttCol[i] <<"\t\t";
			file << _oAttCol[i] <<"\t\t";
		}
		else if(_oAttCol[i].size() < 13 &&_oAttCol[i].size() > 7){
			cout << _oAttCol[i] <<"\t\t\t";
			file << _oAttCol[i] <<"\t\t\t";
		}
		else if(_oAttCol[i].size() < 7){
			cout << _oAttCol[i] <<"\t\t\t\t";
			file << _oAttCol[i] <<"\t\t\t\t";
		}
		else{
			cout << _oAttCol[i] <<"\t";
			file << _oAttCol[i] <<"\t";
		}

		cout <<  _ndCount[i].nNum << "\t";
		cout <<  _ndCount[i].dNum << "\t";
		cout <<  _ndCount[i].nd << "\t";
		//cout <<  _ndCount[i].attName;
		cout << endl;

		file <<  _ndCount[i].nNum << "\t";
		file <<  _ndCount[i].dNum << "\t";
		file <<  _ndCount[i].nd << "\t";
		//file <<  _ndCount[i].attName;
		file << endl;
	}

	/* --- rank fe --- */
	vector<double> feCount;
	vector<string> feSort;
	for(int i=0; i<_oAttRow.size(); ++i){
		feCount.push_back(0);
		feSort.push_back(_oAttRow[i]);
	}
		
	int feFirstThree = 3;

	for(int i=0; i<_oAttCol.size(); ++i){
		for(int j=0; j<feFirstThree; ++j){
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

	cout << "\nBest Feature: ";
	file << "\nBest Feature: ";
	for(int i=0; i<feFirstThree; ++i){
		cout << feSort[i] << " ";
		file << feSort[i] << " ";
	}

	cout << endl;
	file << endl;

	/* --- rank type --- */
	vector<double> tyCount;
	vector<string> tySort;

	for(int i=0; i<_oAttCol.size(); ++i){
		tyCount.push_back(0);
		tySort.push_back(_oAttCol[i]);
	}

	int tyFirstThree = 1;

	for(int i=0; i<_oAttCol.size(); ++i){
		for(int j=0; j<tyFirstThree; ++j){
			tyCount[i] += _sorVON[i][j];
			tyCount[i] += _sorVOD[i][j];
			tyCount[i] += _sorVOA[i][j];
		}
	}
	
	for(int i=0; i<_oAttCol.size(); ++i){
		for(int j=i; j<_oAttCol.size(); ++j){
			if(tyCount[i]>tyCount[j]){
				sortswap(tyCount[i], tyCount[j], tySort[i], tySort[j]);
			}
		}
	}

	cout << "Best Group: ";
	file << "Best Group: ";
	for(int i=0; i<tyFirstThree; ++i){
		cout << tySort[i] << " ";
		file << tySort[i] << " ";
	}
	cout << endl;
	file << endl;


	cout.unsetf( ios::fixed );
	file.unsetf( ios::fixed );
	file.close();
}

string currentDateTime(){
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%Y%m%d_%I%M%S", &tstruct);
    return buf;
}

void help(){
	cout << "Error: main.out  [csv path] [calibration func(IC0400 or IC0061)]" << endl;
}

void openAllFe(int feN, vector<int>& _feIndex){
	for(int i=0; i<feN; ++i)
		_feIndex.push_back(i);
}

void progressBar(string name, int index, int total, bool fin){
	float progress = ((float)index/(float)total);
	
	int barWidth = 70;

	cout << name << "\t"<<"[";
	int pos = barWidth * progress;
	
	for (int i = 0; i < barWidth; ++i) {
		if (i < pos) std::cout << "#";
		else if (i == pos) std::cout << ">";
		else std::cout << " ";
	}

	cout << "] " << int(progress * 100.0) << "%" << "\r";
	if(fin)
		cout << endl;
}

void changeAttName(vector<string>& __oAttCol){
	vector<string> att;

	for(int i=0; i<__oAttCol.size(); ++i){
		att = split(__oAttCol[i], ',');
		
		__oAttCol[i] = "";
		
		for(int j=0; j<att.size(); ++j){
			if(att.size() < 2){
				__oAttCol[i] += "NG";
			}
			else{
				if(att[j] == "Type")
					continue;
				if(j!=0)
					__oAttCol[i] += ",";
				__oAttCol[i] += att[j];			
			}
		}
		att.clear();
	}
}

void featureExtraction(vector<int> _index, vector<Mat> _defImg, vector<Mat> _refImg, vector< vector<double> >& _feCal){

	vector<double> __feCal;
	for(int i=0; i<_defImg.size(); ++i)
		__feCal.push_back(0);
	for(int i=0; i<_index.size(); ++i)
		_feCal.push_back(__feCal);

     Feature fe;

	for(int i=0; i<_index.size(); ++i){  
		progressBar("feExtraction",i,_index.size(),false);
          switch(_index[i]){
               case 0:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5000(_defImg[j], _refImg[j]);   
                    } 
                    break;
               case 1:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5100(_defImg[j], _refImg[j]);
                    }
                    break;
               case 2:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5101(_defImg[j], _refImg[j]);
                    }
                    break;
               case 3:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5102(_defImg[j], _refImg[j]);
                    }
                    break;
               case 4:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5103(_defImg[j], _refImg[j]);
                    }
                    break;
               case 5:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5104(_defImg[j], _refImg[j]);
                    }
                    break;
               case 6:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5110(_defImg[j], _refImg[j]);
                    }
                    break;
               case 7:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5200(_defImg[j], _refImg[j]);
                    }
                    break;
               case 8:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5210(_defImg[j], _refImg[j]);
                    }
                    break;
               case 9:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5211(_defImg[j], _refImg[j]);
                    }
                    break;
               case 10:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5212(_defImg[j], _refImg[j]);
                    }
                    break;
               case 11:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5220(_defImg[j], _refImg[j]);
                    }
                    break;
               case 12:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5221(_defImg[j], _refImg[j]);
                    }
                    break;
               case 13:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5222(_defImg[j], _refImg[j]);
                    }
                    break;
               case 14:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5230(_defImg[j], _refImg[j]);
                    }
                    break;
               case 15:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5231(_defImg[j], _refImg[j]);
                    }
                    break;
               case 16:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5232(_defImg[j], _refImg[j]);
                    }
                    break;
               case 17:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5240(_defImg[j], _refImg[j]);
                    }
                    break;
               case 18:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5241(_defImg[j], _refImg[j]);
                    }
                    break;
               case 19:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5242(_defImg[j], _refImg[j]);
                    }
                    break;
               case 20:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5250(_defImg[j], _refImg[j]);
                    }
                    break;
               case 21:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5251(_defImg[j], _refImg[j]);
                    }
                    break;                    
               case 22:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5252(_defImg[j], _refImg[j]);
                    }
                    break;
               case 23:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5300(_defImg[j], _refImg[j]);
                    }
                    break;                    
               case 24:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5301(_defImg[j], _refImg[j]);
                    }
                    break;                    
               case 25:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5302(_defImg[j], _refImg[j]);
                    }
                    break;                    
               case 26:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5303(_defImg[j], _refImg[j]);
                    }
                    break;                    
               case 27:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5304(_defImg[j], _refImg[j]);
                    }
                    break;                    
               case 28:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5305(_defImg[j], _refImg[j]);
                    }
                    break;                    
               case 29:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5310(_defImg[j], _refImg[j]);
                    }
                    break;                    
               case 30:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5311(_defImg[j], _refImg[j]);
                    }
                    break;                    
               case 31:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5312(_defImg[j], _refImg[j]);
                    }
                    break;                    
               case 32:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5313(_defImg[j], _refImg[j]);
                    }
                    break;                    
               case 33:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5314(_defImg[j], _refImg[j]);
                    }
                    break;                    
               case 34:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5315(_defImg[j], _refImg[j]);
                    }
                    break;                    
               case 35:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5400(_defImg[j], _refImg[j]);
                    }
                    break;                    
               case 36:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5401(_defImg[j], _refImg[j]);
                    }
                    break;                    
               case 37:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5402(_defImg[j], _refImg[j]);
                    }
                    break;                    
               case 38:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5403(_defImg[j], _refImg[j]);
                    }
                    break;                    
               case 39:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5404(_defImg[j], _refImg[j]);
                    }
                    break;                    
               case 40:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5405(_defImg[j], _refImg[j]);
                    }
                    break;                    
               case 41:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5410(_defImg[j], _refImg[j]);
                    }
                    break;                    
               case 42:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5411(_defImg[j], _refImg[j]);
                    }
                    break;                    
               case 43:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5412(_defImg[j], _refImg[j]);
                    }
                    break;                    
               case 44:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5413(_defImg[j], _refImg[j]);
                    }
                    break;
               case 45:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5414(_defImg[j], _refImg[j]);
                    }
                    break;                    
               case 46:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5415(_defImg[j], _refImg[j]);
                    }
                    break;
               case 47:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5416(_defImg[j], _refImg[j]);
                    }
                    break;
               case 48:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5417(_defImg[j], _refImg[j]);
                    }
                    break;
               case 49:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5418(_defImg[j], _refImg[j]);
                    }
                    break;
               case 50:
                    for(int j=0; j<_defImg.size(); ++j){
                         _feCal[i][j] = (double)fe.FE5419(_defImg[j], _refImg[j]);
                    }
                    break;
               default:
                    break;
          }
     }
	progressBar("feExtraction",_index.size(),_index.size(),true);
}

string feName(int _index){
	string name;

          switch(_index){
               case 0:
                    name = ("FE5000");
                    break;
               case 1:
                    name = ("FE5100");
                    break;
               case 2:
                    name = ("FE5101");
                    break;
               case 3:
                    name = ("FE5102");
                    break;
               case 4:
                    name = ("FE5103");
                    break;
               case 5:
                    name = ("FE5104");
                    break;
               case 6:
                    name = ("FE5110");
                    break;
               case 7:
                    name = ("FE5200");
                    break;
               case 8:
                    name = ("FE5210");
                    break;                    
               case 9:
                    name = ("FE5211");
                    break;                    
               case 10:
                    name = ("FE5212");
                    break;                    
               case 11:
                    name = ("FE5220");
                    break;                    
               case 12:
                    name = ("FE5221");
                    break;                    
               case 13:
                    name = ("FE5222");
                    break;                    
               case 14:
                    name = ("FE5230");
                    break;                    
               case 15:
                    name = ("FE5231");
                    break;                    
               case 16:
                    name = ("FE5232");
                    break;                    
               case 17:
                    name = ("FE5240");
                    break;                    
               case 18:
                    name = ("FE5241");
                    break;                    
               case 19:
                    name = ("FE5242");
                    break;
               case 20:
                    name = ("FE5250");
                    break;
               case 21:
                    name = ("FE5251");
                    break;
               case 22:
                    name = ("FE5252");
                    break;
               case 23:
                    name = ("FE5300");
                    break;
               case 24:
                    name = ("FE5301");
                    break;
               case 25:
                    name = ("FE5302");
                    break;
               case 26:
                    name = ("FE5303");
                    break;
               case 27:
                    name = ("FE5304");
                    break;
               case 28:
                    name = ("FE5305");
                    break;
               case 29:
                    name = ("FE5310");
                    break;
               case 30:
                    name = ("FE5311");
                    break;
               case 31:
                    name = ("FE5312");
                    break; 
               case 32:
                    name = ("FE5313");
                    break;
               case 33:
                    name = ("FE5314");
                    break;
               case 34:
                    name = ("FE5315");
                    break;
               case 35:
                    name = ("FE5400");
                    break;
               case 36:
                    name = ("FE5401");
                    break;
               case 37:
                    name = ("FE5402");
                    break;
               case 38:
                    name = ("FE5403");
                    break;
               case 39:
                    name = ("FE5404");
                    break;
               case 40:
                    name = ("FE5405");
                    break;
               case 41:
                    name = ("FE5410");
                    break;
               case 42:
                    name = ("FE5411");
                    break;
               case 43:
                    name = ("FE5412");
                    break;
               case 44:
                    name = ("FE5413");
                    break;
               case 45:
                    name = ("FE5414");
                    break;
               case 46:
                    name = ("FE5415");
                    break;
               case 47:
                    name = ("FE5416");
                    break;
               case 48:
                    name = ("FE5417");
                    break;
               case 49:
                    name = ("FE5418");
                    break;
               case 50:
                    name = ("FE5419");
                    break;
               default:
                    break;
          }
     return name;
}

void writeCSV(vector< vector<double> > _feCal, vector<string> fename, vector< vector<string> > _inFile){

	ofstream file;
	string name = currentDateTime() + ".csv";
	file.open(name);
	//file << fixed  <<  setprecision(2);

	for(int i=0; i<_inFile[0].size(); ++i)
		file << _inFile[0][i] << ",";
	for(int i=0; i<fename.size(); ++i){
		file << fename[i];
		if(i != (fename.size()-1)) file << ",";
	}
	file << endl;
	
	for(int i=0; i<_feCal.size(); ++i){
		for(int j=0; j<_inFile[0].size(); ++j){
			file << _inFile[i+1][j] << ",";
		}
		for(int j=0; j<_feCal[0].size(); ++j){
			file << _feCal[i][j];
			if(j != (_feCal[0].size()-1)) file<< ",";
		}
		file << endl;
	}

	//file.unsetf( ios::fixed );
	file.close();
}
