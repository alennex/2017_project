#include "mainwindow.h"

MyWidget::MyWidget(QWidget *parent):
QWidget(parent){

     FO_check_switch = false;
     IA_check_switch = false;
     ST_check_switch = true;

     QString name;
     SpName.push_back("Type");
     SpName.push_back("TEST");
     SpName.push_back("SCANTESTID");

     QGridLayout *mainLayout = new QGridLayout(this);


     // ========= File Open
     QPushButton* FO_bu = new QPushButton("Open CSV",this);     
     connect(FO_bu, SIGNAL(clicked()), this,SLOT(openfile()));

     // ========= Image Calibration
     IA_combo = new QComboBox(this);
     IA_combo->setEditable(false);
     for(int i=0; i<AlignmentN; ++i){
          QString Aname = alignmentName(i);
          name.sprintf("Cal-%03d(%s)",i,qPrintable(Aname));
          IA_combo->insertItem(i,name);
     }     
     connect(IA_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(IA_check(int)));

     // ========== start button
     QPushButton* SB_bu = new QPushButton("Start",this);
     connect(SB_bu, SIGNAL(clicked()), this,SLOT(progress()));

     // ========== State win
     ST_state = new QLineEdit(this);
     ST_state->setReadOnly(true);
     ST_state->setAlignment(Qt::AlignCenter);

     // ========== progress bar
     SB_pbar = new QProgressBar(this);     
     SB_pbar->setRange(0,100);
     SB_pbar->setValue(0);

     // ========== scrollBar Area
/*

    QScrollArea *scrollArea= new QScrollArea(this);
     QWidget *client= new QWidget(scrollArea);;      
     QGridLayout* infoLayoutall= new QGridLayout(client);;

     for(int i=0; i<4*SpName.size(); ++i){
          
          if(i%4 == 0){
               name.sprintf("Attribute-%s",qPrintable(SpName[i/4]));
          
               infoGroup.push_back(new QGroupBox(name,this));
               infoLayout.push_back(new QGridLayout(infoGroup[i]));
               infoLabel.push_back(new QLabel(this));
               infoBar.push_back(new QProgressBar);

               infoLabel[10*(i/4)]->setText("No info");
               infoLayout[i]->addWidget(infoLabel[10*(i/4)],0,0);
               infoGroup[i]->setLayout(infoLayout[i]);
          
          }
          else{
               if(i%4 == 1)
                    name.sprintf("%s-overlap/type0",qPrintable(SpName[i/4]));
               if(i%4 == 2)
                    name.sprintf("%s-overlap/type1",qPrintable(SpName[i/4]));
               if(i%4 == 3)
                    name.sprintf("%s-overlap/(type0+type1)",qPrintable(SpName[i/4]));
               
               infoGroup.push_back(new QGroupBox(name,this));
               infoLayout.push_back(new QGridLayout(infoGroup[i]));

               for(int j=0; j<3; ++j){
                    infoLabel.push_back(new QLabel(this));
                    infoBar.push_back(new QProgressBar(this));
               }

               infoLabel[10*(i/4)+3*(i%4)-2]->setText("No info");
               infoLabel[10*(i/4)+3*(i%4)-1]->setText("No info");
               infoLabel[10*(i/4)+3*(i%4)-0]->setText("No info");
               infoBar[10*(i/4)+3*(i%4)-2]->setRange(0,100);
               infoBar[10*(i/4)+3*(i%4)-1]->setRange(0,100);
               infoBar[10*(i/4)+3*(i%4)-0]->setRange(0,100);
               infoBar[10*(i/4)+3*(i%4)-2]->setValue(0);
               infoBar[10*(i/4)+3*(i%4)-1]->setValue(0);
               infoBar[10*(i/4)+3*(i%4)-0]->setValue(0);
               
               infoLayout[i]->addWidget(infoLabel[10*(i/4)+3*(i%4)-2],0,0);
               infoLayout[i]->addWidget(infoLabel[10*(i/4)+3*(i%4)-1],1,0);
               infoLayout[i]->addWidget(infoLabel[10*(i/4)+3*(i%4)-0],2,0);
               infoLayout[i]->addWidget(infoBar[10*(i/4)+3*(i%4)-2],0,1);
               infoLayout[i]->addWidget(infoBar[10*(i/4)+3*(i%4)-1],1,1);
               infoLayout[i]->addWidget(infoBar[10*(i/4)+3*(i%4)-0],2,1);
               infoGroup[i]->setLayout(infoLayout[i]);
          }
     }
     
     for(int i=0; i<infoGroup.size(); ++i){
          infoLayoutall->addWidget(infoGroup[i],(i/4),(i%4));
     }
          
     client->setLayout(infoLayoutall);
     scrollArea->setWidget(client);


*/
 
     // ========= Mainmenu Layout
     mainLayout->addWidget(FO_bu,0,0,Qt::AlignTop);
     mainLayout->addWidget(IA_combo,0,1,Qt::AlignTop);
     mainLayout->addWidget(SB_bu,0,2,Qt::AlignTop);
     mainLayout->addWidget(ST_state,0,3,Qt::AlignTop);
     mainLayout->addWidget(SB_pbar,0,4,Qt::AlignTop);
//     mainLayout->addWidget(scrollArea,1,0,1,5);
     setLayout(mainLayout);

}

void MyWidget::collectAttribute(){
     // QVector<QStringList> inFile;

     QVector<QVector<QString>> attribute;
     QVector<QString> _attribute;
     
     for(int i=0; i<5; ++i)
          _attribute.push_back("");

     for(int i=0; i<inFile[0].size(); ++i){
          if(inFile[0].at(i).contains("DEFECTID",Qt::CaseSensitive) ||
           inFile[0].at(i).contains("DEF_IMAGE",Qt::CaseSensitive)||
           inFile[0].at(i).contains("REF_IMAGE",Qt::CaseSensitive))
               continue;
          
          attribute.push_back(_attribute);
          attribute[i][0].sprintf("%s",qPrintable(inFile[0].at(i)));
          
          for(int j=1; j<inFile.size(); ++j){
               for(int k=1; k<attribute[i].size(); ++k){
                    if(inFile[j].at(i)==attribute[i][k])
                         break;
                    else if(attribute[i][k] == "")
                         attribute[i][k].sprintf("%s",qPrintable(inFile[j].at(i)));
                    else
                         continue;
               }
          }
     }
}

void MyWidget::selectMin(){

     QVector<QString> _feOverAreaSortFname;
     QString name;

     for(int k=0; k<feOverArea.size(); ++k){
          //name.sprintf("%03d(%s)",k,qPrintable(featureName(k)));
          name.sprintf("F%03d",k);
          
          _feOverAreaSortFname.clear();
          for(int i=0; i<feOverArea[0].size(); ++i)
               _feOverAreaSortFname.push_back(name);
          feOverAreaSortFname.push_back(_feOverAreaSortFname);
     }

     //qDebug() << feOverArea;
     //qDebug() << feOverAreaSortFname;

     for(int k=0; k<feOverArea[0].size(); ++k){
          for(int i=0; i<feOverArea.size(); ++i){
               for(int j=i+1; j<feOverArea.size(); ++j){
                    if(feOverArea[i][k]>feOverArea[j][k]){
                         swap(feOverArea[i][k], feOverArea[j][k], feOverAreaSortFname[i][k], feOverAreaSortFname[j][k]);
                    }
               }
          }
     }
     
     //qDebug() << feOverArea;
     //qDebug() << feOverAreaSortFname;
     QVector<QString> feNameTop(feOverArea.size());
     QVector<QString> feNameTopOut(feOverArea[0].size()/3);
     QVector<double> feValTop(feOverArea.size(),0);
     QVector<bool> feCheck(feOverArea.size(),false);
     double min_buf = 0;

     for(int i=0; i<feOverArea.size(); ++i){
          name.sprintf("F%03d",i);
          feNameTop[i].sprintf("%s",qPrintable(name));
     }

     qDebug() << "feNameTop" << feNameTop;
     qDebug() << "feNameTopOut" << feNameTopOut;
     qDebug() << "feValTop" << feValTop;
 

     for(int i=0; i<feOverArea[0].size()/3; ++i){
          for(int j=0; j<3; ++j){
               for(int k=0; k<feOverArea.size(); ++k){
                    if(feOverAreaSortFname[j][i*3+0] == feNameTop[k]){
                         feValTop[k] += feOverArea[j][i*3+0];
                         feCheck[k] = true;
                    }
                    if(feOverAreaSortFname[j][i*3+1] == feNameTop[k]){
                         feValTop[k] += feOverArea[j][i*3+1];
                         feCheck[k] = true;
                    }
                    if(feOverAreaSortFname[j][i*3+1] == feNameTop[k]){
                         feValTop[k] += feOverArea[j][i*3+1];
                         feCheck[k] = true;
                    }
               }
          }

          for(int j=0; j<feOverArea.size(); ++j){
               if(feCheck[j]==true){
                    min_buf = feValTop[j];
                    feNameTopOut[i].sprintf("%s(%s)",qPrintable(feNameTop[j]),qPrintable(featureName(j))); 
                    break;
               }
          }

          for(int j=0; j<feOverArea.size(); ++j){
               if((feCheck[j]==true)&&(feValTop[j] < min_buf)){
                    min_buf = feValTop[j];
                    feNameTopOut[i].sprintf("%s(%s)",qPrintable(feNameTop[j]),qPrintable(featureName(j)));
               }
          }

          for(int j=0; j<feOverArea.size(); ++j){
               feCheck[j] = false;
               feValTop[j] = 0;
          }
     }

     qDebug() << "feNameTop" << feNameTop;
     qDebug() << "feNameTopOut" << feNameTopOut;
     qDebug() << "feValTop" << feValTop;

     for(int i=0; i<4*SpName.size(); ++i){
          if(i%4 != 0){
               infoLabel[10*(i/4)+3*(i%4)-2]->setText(feOverAreaSortFname[0][i-1-(i/4)]);
               infoLabel[10*(i/4)+3*(i%4)-1]->setText(feOverAreaSortFname[1][i-1-(i/4)]);
               infoLabel[10*(i/4)+3*(i%4)-0]->setText(feOverAreaSortFname[2][i-1-(i/4)]);
               
               infoBar[10*(i/4)+3*(i%4)-2]->setValue(feOverArea[0][i-1-(i/4)]);
               infoBar[10*(i/4)+3*(i%4)-1]->setValue(feOverArea[1][i-1-(i/4)]);
               infoBar[10*(i/4)+3*(i%4)-0]->setValue(feOverArea[2][i-1-(i/4)]);
          }
          else{
               infoLabel[10*(i/4)]->setText(feNameTopOut[i/4]);
          }
     }

}

void MyWidget::swap(double& v1, double& v2, QString& n1, QString& n2){
     double tmp = 0;
     QString tmpN;
     tmp = v1; v1 = v2; v2 = tmp;
     tmpN.sprintf("%s",qPrintable(n1)); 
     n1.sprintf("%s",qPrintable(n2)); 
     n2.sprintf("%s",qPrintable(tmpN));
   
}

void MyWidget::openfile(){
     qDebug()<<"---- openfile";
     
     QFileDialog myFileDialog(this);
     QString fileName = myFileDialog.getOpenFileName(this, "Open File",
               QDir::currentPath (), "CSV Files (*.csv *.xls)");

     if (fileName.isEmpty()) {
          ST_state->setText ("No csv file...");
     } else {
          FO_check_switch = true;
          readfile(fileName);
     }
}

void MyWidget::readfile(QString _fileName){
     QFile file(_fileName);

     if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
          qDebug() << "Cannot open file for reading: " << qPrintable(file.errorString());
          exit(0);
     }

     QStringList list;
     list.clear();
     QTextStream in(&file);

     while(!in.atEnd()){
          QString fileLine = in.readLine();
          list = fileLine.split(",", QString::SkipEmptyParts);
          inFile.push_back(list);
     }
     file.close();
}

void MyWidget::IA_check(int combo_index){     
     qDebug() << "combo_index = " << combo_index;     
     IA_index.clear();
     IA_index.push_back(combo_index);
     IA_check_switch = true;
}

void MyWidget::progress(){
     bool image_check_switch = true;
     
     // check
     if(!FO_check_switch){
          qDebug()<<"error: csv file not open.";
          ST_state->setText("error: csv file not open.");
     }
     else if (!IA_check_switch){
          qDebug()<<"error: Alignment not select.";
          ST_state->setText("error: Alignment not select.");
     }
     else if(ST_check_switch){

          float  sb_val = 0.0;

          int defCol = 0, refCol = 0;
          for(int i=0; i<inFile[0].size(); ++i){
               if(inFile[0].at(i).contains("DEF",Qt::CaseSensitive))
                    defCol = i;
               if(inFile[0].at(i).contains("REF",Qt::CaseSensitive))
                    refCol = i;
          }

          qDebug() << "defCol = " << defCol;
          qDebug() << "refCol = " << refCol;

          QString defName, refName;
          ST_state->setText("Open Image ...");

          for(int i=1; i<inFile.size(); ++i){
               
               defName = inFile[i].at(defCol);
               defName = defName.replace(QString("\""),QString(""));
               defName = defName.simplified();

               refName = inFile[i].at(refCol);
               refName = refName.replace(QString("\""),QString(""));
               refName = refName.simplified();

               defImg.push_back(imread(defName.toStdString(),CV_LOAD_IMAGE_GRAYSCALE));
               refImg.push_back(imread(refName.toStdString(),CV_LOAD_IMAGE_GRAYSCALE));
               
               sb_val += 100./inFile.size();
               SB_pbar->setValue(sb_val+1);
               qDebug() << refName << defName;
          }
     
          for(int i=0; i<defImg.size(); ++i){
               if(!defImg[i].data || !refImg[i].data){
                    image_check_switch = false;
                    ST_state->setText("error: image is not open ...");
                    qDebug() << "error: NO."<< i << " image is not open ...";
                    exit(-1);
               }
          }
     }

     if(image_check_switch && ST_check_switch){
          qDebug() << "tag-0-(ImPro)";

          ST_state->setText("Calibration ...");
          do_calibration();
     
          allSelectFunc();

          QVector<double> _feCal;
          for(int j=1; j<inFile.size(); ++j)
               _feCal.push_back(0.0);
          for(int i=0; i<FE_index.size(); ++i)
               feCal.push_back(_feCal);
          _feCal.clear();

          ST_state->setText("Feature Extraction ...");
          do_feature_extraction();

          qDebug() << "tag-2-(ImPro)";
          ST_state->setText("Output csv ...");
          writefile();

          qDebug() << "tag-3-(ImPro)";
          overAreaCal();
      
          ST_state->setText("Finish ...");
          ST_check_switch = false;
     }
}

void MyWidget::allSelectFunc(){
     FE_index.clear();
     for(int i=0; i<FeatureN; ++i)
          FE_index.push_back(i);
}

void MyWidget::writefile(){
     QFileDialog myFileDialog(this);

     QDate dt_now = QDate::currentDate();
     QTime tm_now = QTime::currentTime();
     
     QString IA_NAME;
     IA_NAME.sprintf("%03d",IA_index[0]);
     
     QString FE_NAME,FE_buf;
     for(int i=0; i<FE_index.size(); ++i){
          FE_buf.sprintf("%03d",FE_index[i]);
          FE_NAME.append(FE_buf);
     }
     
     QString _fileName;
     _fileName.sprintf("%s-%s-%s-%s.csv", qPrintable(dt_now.toString("yyyy-MM-dd")), qPrintable(tm_now.toString("hh-mm-ss")), qPrintable(IA_NAME), qPrintable(FE_NAME));

     QFile file(_fileName);
     if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
          qDebug() << "Cannot open file for writing: "<< qPrintable(file.errorString());
          exit(0);
     }
     
     QTextStream out(&file); 
     QString name, value;
     float sb_val = 0.0;

     for(int i=0; i<inFile.size(); ++i){
          outFile.push_back(inFile[i]);
          if(i == 0){
               for(int j=0; j<FE_index.size(); ++j){
                    name.sprintf("Feature-%d", FE_index[j]);
                    outFile[i].append(name);     
               }
          }
          else if(i != 0){
               for(int j=0; j<FE_index.size(); ++j){
                    value.sprintf("%lf",feCal[j][i-1]);
                    outFile[i].append(value);
               }
          }
          sb_val += 100./inFile.size();
          SB_pbar->setValue(sb_val+1);    
     }   

     for(int i=0; i<outFile.size(); ++i){
          for(int j=0; j<outFile[0].size(); ++j){
               out<<outFile[i].at(j)<<",";
          }
          out<<"\n";
     }

     file.close();
     outFile.clear();
}

void MyWidget::overAreaCal(){
     QVector<int> SpIndex;
     QVector<double> rst;

     bool ok = true;

     QVector<double> t0Value;
     QVector<double> t1Value;

     QVector<double> _feOverArea;
     for(int i=0; i<3*SpName.size(); ++i)
          _feOverArea.push_back(0.0);

     for(int i=0; i<feCal.size(); ++i)
         feOverArea.push_back(_feOverArea);

     for(int i=0; i<SpName.size(); ++i){
          if(i==0){
               for(int j=0; j<inFile[0].size(); ++j){
                    if(inFile[0].at(j).contains(SpName[0],Qt::CaseSensitive)){
                         SpIndex.push_back(j);
                         //qDebug() << i << j << SpName[0];
                         break;
                    }
               }

               for(int j=0; j<feCal.size(); ++j){
                    for(int k=0; k<feCal[0].size(); ++k){
                         if(inFile[k+1].at(SpIndex[0]).toInt(&ok,10) == 0){
                              t0Value.push_back(feCal[j][k]);
                         }
                         if(inFile[k+1].at(SpIndex[0]).toInt(&ok,10) == 1){
                              t1Value.push_back(feCal[j][k]);
                         }
                    }
                    overAreaCal_Type(t0Value, t1Value, rst);
                    feOverArea[j][i*3+0] = rst[0];
                    feOverArea[j][i*3+1] = rst[1];
                    feOverArea[j][i*3+2] = rst[2];
                    
                    t0Value.clear();
                    t1Value.clear();
               }      
          }
          else{
               
               for(int j=0; j<inFile[0].size(); ++j){
                    if(inFile[0].at(j).contains(SpName[0],Qt::CaseSensitive)){
                         SpIndex.push_back(j);
                         break;
                    }
               }
               
               for(int j=0; j<inFile[0].size(); ++j){
                    if(inFile[0].at(j).contains(SpName[i],Qt::CaseSensitive)){
                         SpIndex.push_back(j);
                         break;
                    }
               }
               for(int j=0; j<feCal.size(); ++j){
                    for(int k=0; k<feCal[0].size(); ++k){
                         if((inFile[k+1].at(SpIndex[1]).toInt(&ok,10) == 1)&&(inFile[k+1].at(SpIndex[0]).toInt(&ok,10) == 0)){
                              t0Value.push_back(feCal[j][k]);
                         }
                         if((inFile[k+1].at(SpIndex[1]).toInt(&ok,10) == 1)&&(inFile[k+1].at(SpIndex[0]).toInt(&ok,10) == 1)){
                              t1Value.push_back(feCal[j][k]);
                         }
                    }
                    overAreaCal_Type(t0Value, t1Value, rst);
                    feOverArea[j][i*3+0] = rst[0];
                    feOverArea[j][i*3+1] = rst[1];
                    feOverArea[j][i*3+2] = rst[2];
                    
                    t0Value.clear();
                    t1Value.clear();
               }    
          }

          SpIndex.clear();
     }
     selectMin();
//     qDebug() << "feOverArea" << feOverArea;
}

void MyWidget::overAreaCal_Type(QVector<double> t0Value, QVector<double> t1Value, QVector<double>& rst){
     rst.clear();

     QVector<int> overArea;
     QVector<int> t0Area;
     QVector<int> t1Area;
     QVector<int> sum(3,0);
     double max = 0.0;

     for(int i=0;i<t0Value.size(); ++i)
          max = (max>t0Value[i])?max:t0Value[i];

     for(int i=0;i<t1Value.size(); ++i)
          max = (max>t1Value[i])?max:t1Value[i];
     max = max + 5;
     for(int i=0; i<max; ++i){
          overArea.push_back(0); t0Area.push_back(0); t1Area.push_back(0);
     }

     for(int i=0; i<t0Value.size(); ++i)
          t0Area[round(t0Value[i])]++;

     for(int i=0; i<t1Value.size(); ++i)
          t1Area[round(t1Value[i])]++;
     
     for(int i=0; i<max; ++i)
          overArea[i] = (t0Area[i]<t1Area[i])?(t0Area[i]):(t1Area[i]);
     
     for(int i=0; i<max; ++i){
          sum[0] += overArea[i]; sum[1] += t0Area[i]; sum[2] += t1Area[i];
     }


     if(sum[0] == 0){
          rst.push_back(0);
          rst.push_back(0);
          rst.push_back(0);          
     }
     else if(sum[1] == 0){
          rst.push_back(0);
          rst.push_back(100.*(double)sum[0]/(double)sum[2]);
          rst.push_back(100.*(double)sum[0]/(double)(sum[1]+sum[2]-sum[0]));
     }
     else if(sum[2] == 0){
          rst.push_back(100.*(double)sum[0]/(double)sum[1]);
          rst.push_back(0);
          rst.push_back(100.*(double)sum[0]/(double)(sum[1]+sum[2]-sum[0]));          
     }
     else{
          rst.push_back(100.*(double)sum[0]/(double)sum[1]);
          rst.push_back(100.*(double)sum[0]/(double)sum[2]);
          rst.push_back(100.*(double)sum[0]/(double)(sum[1]+sum[2]-sum[0]));    
     }
}

void MyWidget::do_calibration(){
     float  sb_val = 0.0;
     float sb_step = 100./defImg.size();
     double thresh = 128;
     double max_value = 255;

     Alignment af;

     switch(IA_index[0]){
          case 0:
               for(int i=0; i<defImg.size();++i) af.alignECC_Translation(defImg[i],refImg[i]);
               break;
          case 1:
               for(int i=0; i<defImg.size();++i){
                    af.alignECC_Euclidean(defImg[i],refImg[i]);
                    sb_val += sb_step;
                    SB_pbar->setValue(sb_val+1);
               } 
               break;
          case 2:
               for(int i=0; i<defImg.size();++i){
                    af.alignECC_Affine(defImg[i],refImg[i]);
                    sb_val += sb_step;
                    SB_pbar->setValue(sb_val+1);
               } 
               break;
          case 3:
               for(int i=0; i<defImg.size();++i){
                    af.alignECC_Homography(defImg[i],refImg[i]);
                    sb_val += sb_step;
                    SB_pbar->setValue(sb_val+1);   
               } 
               break;
          case 4:
               for(int i=0; i<defImg.size();++i){
                    af.alignDCT(defImg[i],refImg[i]);
                    sb_val += sb_step;
                    SB_pbar->setValue(sb_val+1); 
               } 
               break;
          case 5:
               for(int i=0; i<defImg.size();++i){
                    af.alignDFT(defImg[i],refImg[i]);
                    sb_val += sb_step;
                    SB_pbar->setValue(sb_val+1); 
               } 
               break;
          case 6:
               for(int i=0; i<defImg.size();++i){
                    af.histogramEqualization(defImg[i],refImg[i]);
                    sb_val += sb_step;
                    SB_pbar->setValue(sb_val+1);
               } 
               break;
          case 7:
               for(int i=0; i<defImg.size();++i){
                    af.threshOtsu(defImg[i],refImg[i], thresh, max_value);
                    sb_val += sb_step;
                    SB_pbar->setValue(sb_val+1);
               } 
               break;
          case 8:
               for(int i=0; i<defImg.size();++i){
                    af.threshBinary(defImg[i],refImg[i], thresh, max_value);
                    sb_val += sb_step;
                    SB_pbar->setValue(sb_val+1);
               } 
               break;
          case 9:
               for(int i=0; i<defImg.size();++i){
                    af.threshBinaryINV(defImg[i],refImg[i], thresh, max_value);
                    sb_val += sb_step;
                    SB_pbar->setValue(sb_val+1);
               } 
               break;
          case 10:
               for(int i=0; i<defImg.size();++i){
                    af.threshTrunc(defImg[i],refImg[i], thresh, max_value);
                    sb_val += sb_step;
                    SB_pbar->setValue(sb_val+1);
               } 
               break;
          case 11:
               for(int i=0; i<defImg.size();++i){
                    af.threshToZero(defImg[i],refImg[i], thresh, max_value);
                    sb_val += sb_step;
                    SB_pbar->setValue(sb_val+1);
               } 
               break;
          case 12:
               for(int i=0; i<defImg.size();++i){
                    af.threshToZeroINV(defImg[i],refImg[i], thresh, max_value);
                    sb_val += sb_step;
                    SB_pbar->setValue(sb_val+1);
               } 
               break;
          default:
               break;
     }
}

QString MyWidget::alignmentName(int index){
     QString name;

     switch(index){
          case 0:
               name.sprintf("alignECC_Translation");
               break;
          case 1:
               name.sprintf("alignECC_Euclidean");
               break;
          case 2:
               name.sprintf("alignECC_Affine");
               break;
          case 3:
               name.sprintf("alignECC_Homography");
               break;
          case 4:
               name.sprintf("alignDCT");
               break;
          case 5:
               name.sprintf("alignDFT");
               break;
          case 6:
               name.sprintf("histogramEqualization");
               break;
          case 7:
               name.sprintf("threshOtsu");
               break;
          case 8:
               name.sprintf("threshBinary");
               break;
          case 9:
               name.sprintf("threshBinaryINV");
               break;
          case 10:
               name.sprintf("threshTrunc");
               break;
          case 11:
               name.sprintf("threshToZero");
               break;
          case 12:
               name.sprintf("threshToZeroINV");
               break;
          default:
               break;
     }
     return name;
}

void MyWidget::do_feature_extraction(){
     float  sb_val = 0.0;
     float sb_step = 100./FE_index.size()*defImg.size();
     
     Feature fe,feI(defImg[0],refImg[0]);
     
     int color_gap = 3;
     int pool_size = 4;
     int thresh = 128;
     int block_size = 16;

     for(int i=0; i<FE_index.size(); ++i){  
          switch(FE_index[i]){
               case 0:
                    for(int j=0; j<defImg.size(); ++j){
                         feCal[i][j] = (double)fe.diffGlobalMean(defImg[j], refImg[j]);   
                         sb_val += sb_step;
                         SB_pbar->setValue(sb_val+1);
                    } 
                    break;
               case 1:
                    for(int j=0; j<defImg.size(); ++j){
                         feCal[i][j] = (double)fe.diffPixelsCount(defImg[j], refImg[j], color_gap);
                         sb_val += sb_step;
                         SB_pbar->setValue(sb_val+1);
                    }
                    break;
               case 2:
                    for(int j=0; j<defImg.size(); ++j){
                         feCal[i][j] = fe.getPSNR(defImg[j], refImg[j]);
                         sb_val += sb_step;
                         SB_pbar->setValue(sb_val+1);
                    }
                    break;
               case 3:
                    for(int j=0; j<defImg.size(); ++j){
                         feCal[i][j] = fe.localmeanSSE(defImg[j], refImg[j], pool_size);
                         sb_val += sb_step;
                         SB_pbar->setValue(sb_val+1);
                    }
                    break;
               case 4:
                    for(int j=0; j<defImg.size(); ++j){
                         feCal[i][j] = fe.localmeanSAE(defImg[j], refImg[j], pool_size);
                         sb_val += sb_step;
                         SB_pbar->setValue(sb_val+1);
                    }
                    break;
               case 5:
                    for(int j=0; j<defImg.size(); ++j){
                         Feature feI(defImg[j], refImg[j]);
                         feCal[i][j] = feI.maxNumBlackPixelsInBlock(thresh, block_size);
                         sb_val += sb_step;
                         SB_pbar->setValue(sb_val+1);
                    }
                    break;
               case 6:
                    for(int j=0; j<defImg.size(); ++j){
                         Feature feI(defImg[j], refImg[j]);
                         feCal[i][j] = feI.maxNumBlackPixelsInBlock(defImg[j],thresh, block_size);
                         sb_val += sb_step;
                         SB_pbar->setValue(sb_val+1);
                    }
                    break;
               case 7:
                    for(int j=0; j<defImg.size(); ++j){
                         Feature feI(defImg[j], refImg[j]);
                         feCal[i][j] = feI.maxNumDiffPixelsInBlock(thresh, block_size);
                         sb_val += sb_step;
                         SB_pbar->setValue(sb_val+1);
                    }
                    break;
               default:
                    break;
          }
     }
     defImg.clear();
     refImg.clear();
}

QString MyWidget::featureName(int index){
     QString name;

          switch(index){
               case 0:
                    name.sprintf("diffGlobalMean");
                    break;
               case 1:
                    name.sprintf("diffPixelsCount");
                    break;
               case 2:
                    name.sprintf("getPSNR");
                    break;
               case 3:
                    name.sprintf("localmeanSSE");
                    break;
               case 4:
                    name.sprintf("localmeanSAE");
                    break;
               case 5:
                    name.sprintf("maxNumBlackPixelsInBlock");
                    break;
               case 6:
                    name.sprintf("maxNumBlackPixelsInBlock");
                    break;
               case 7:
                    name.sprintf("maxNumDiffPixelsInBlock");
                    break;
               default:
                    break;
          }
     return name;
}




















