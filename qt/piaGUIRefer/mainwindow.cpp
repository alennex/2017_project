#include "mainwindow.h"

MyWidget::MyWidget(QWidget *parent):
QWidget(parent){
     
     FO_check_switch = false;
     IA_check_switch = false;
     FE_check_switch = false;
     ST_check_switch = true;

     QString name;

     // ========= File Open

     QGroupBox* FileOpen = new QGroupBox("Step 1: File Open", this);
     QVBoxLayout* FO_layout = new QVBoxLayout(this);
    
     QPushButton* FO_bu = new QPushButton("open CSV file",this);
     FO_line =  new QLineEdit(this);

     FO_line->setReadOnly(true);
     
     connect(FO_bu, SIGNAL(clicked()), this,SLOT(showfile()));
     
     FO_layout->addWidget(FO_bu);
     FO_layout->addWidget(FO_line);
     FileOpen->setLayout(FO_layout);

     // ========= Image Alignment

     QGroupBox* ImageAlignment = new QGroupBox("Step 2: Image Alignment", this);
     QGridLayout* IA_layout = new QGridLayout(this);

     IA_line = new QLineEdit(this);
     IA_combo = new QComboBox(this);
     QPushButton* IA_bu = new QPushButton("show",this);

     IA_combo->setEditable(false);
     IA_line->setReadOnly(true);
     IA_line->setAlignment(Qt::AlignCenter);

     for(int i=0; i<AlignmentN; ++i){
          QString Aname = alignmentName(i);
          name.sprintf("Alignment-%d(%s)",i,qPrintable(Aname));
          IA_combo->insertItem(i,name);
     }
     
     connect(IA_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(IA_check(int)));
     connect(IA_bu,SIGNAL(clicked()), this, SLOT(alignmentImageShow()));

     IA_layout->addWidget(IA_combo,0,0,1,2);
     IA_layout->addWidget(IA_line,1,0);
     IA_layout->addWidget(IA_bu,1,1);
     ImageAlignment->setLayout(IA_layout);

     // ========= Feature Extraction

     QGroupBox* FeatureExtraction = new QGroupBox("Step 3: Feature Extraction", this);
     QGridLayout* FE_layout = new QGridLayout(this);

     QListWidget* FE_list = new QListWidget(this);
     QVector<QListWidgetItem*> FE_item;
     allSelect = new QCheckBox("All", this);

     for(int i=0; i<FeatureN; ++i){
          QString Fname = featureName(i);
          name.sprintf("Feature-%d(%s)", i,qPrintable(Fname));      
          FE_ch.push_back(new QCheckBox(name, this));
     }

     connect(allSelect, SIGNAL(stateChanged(int)), this, SLOT(allSelectFunc()));
     
     for(int i=0; i<FeatureN; ++i){
          connect(FE_ch[i], SIGNAL(stateChanged(int)), this, SLOT(FE_check()));
     }

     for(int i=0; i<(1+FE_ch.size()); ++i){
          
          FE_item.push_back(new QListWidgetItem);
          FE_list->addItem(FE_item[i]);
          
          if(i==0)
               FE_list->setItemWidget(FE_item[i], allSelect);
          else          
               FE_list->setItemWidget(FE_item[i], FE_ch[i-1]);
     }

     FE_layout->addWidget(FE_list);
     FeatureExtraction->setLayout(FE_layout);

     // ========= Start Button

     QGroupBox* StartButton = new QGroupBox("Step 4: Start", this);
     QVBoxLayout* SB_layout = new QVBoxLayout(this);
    
     ST_state = new QLineEdit(this);
     QPushButton* SB_bu = new QPushButton("Start",this);
     SB_pbar = new QProgressBar(this);
     
     ST_state->setReadOnly(true);
     ST_state->setAlignment(Qt::AlignCenter);
     SB_pbar->setRange(0,100);
     SB_pbar->setValue(0);
     connect(SB_bu, SIGNAL(clicked()), this,SLOT(progress()));
     
     SB_layout->addWidget(SB_bu);
     SB_layout->addWidget(SB_pbar);
     SB_layout->addWidget(ST_state);
     StartButton->setLayout(SB_layout);

     // ========= List Widget of result

     QGroupBox* ListResult = new QGroupBox("List of Result", this);
     QVBoxLayout* LR_layout = new QVBoxLayout(this);

     LR_list = new QListWidget(this);
     //LR_list->setIconSize(QSize(300,200));
     connect(LR_list, SIGNAL(itemPressed(QListWidgetItem*)),this, SLOT(previewImg()));

     LR_layout->addWidget(LR_list);
     ListResult->setLayout(LR_layout);

     // ========== preview Img

     QGroupBox* PreviewImage = new QGroupBox("Preview Image", this);
     QVBoxLayout* PI_layout = new QVBoxLayout(this);

     PI_label = new QLabel(this);
     PI_label2 = new QLabel(this);

     QImage PI_image = QImage(640,240,QImage::Format_RGB888);
     PI_label->resize(640,240);
     PI_label->setPixmap(QPixmap::fromImage(PI_image));

     QImage PI_image2 = QImage(640,240,QImage::Format_RGB888);
     PI_label2->resize(640,240);
     PI_label2->setPixmap(QPixmap::fromImage(PI_image2));

     PI_layout->addWidget(PI_label);
     PI_layout->addWidget(PI_label2);
     PreviewImage->setLayout(PI_layout);

     // ========= Mainmenu Layout
     QGridLayout *mainLayout = new QGridLayout(this);
     mainLayout->addWidget(FileOpen,0,0);
     mainLayout->addWidget(ImageAlignment,0,1);
     mainLayout->addWidget(FeatureExtraction,0,2);
     mainLayout->addWidget(StartButton,0,3);
     mainLayout->addWidget(ListResult,2,0,1,1);
     mainLayout->addWidget(PreviewImage,2,1,1,3);
     setLayout(mainLayout);
}

void MyWidget::previewImg(){
     int currentRow = LR_list->currentRow();
     PI_label->setPixmap(QPixmap::fromImage(_PI_image[currentRow]));
     PI_label2->setPixmap(QPixmap::fromImage(_PI_image2[currentRow]));
}

void MyWidget::build_list(){
     QString name;

     for(int i=0; i<FE_index.size(); ++i){
          QString fname = featureName(FE_index[i]);
          name.sprintf("(%d,%d,%d)Feature-%d(%s)", overArea[i*3],overArea[i*3+1],overArea[i*3+2], 
               FE_index[i],qPrintable(fname));
          LR_list->insertItem(i,name);
     }
}

void MyWidget::allSelectFunc(){
     bool check = false;
     FE_index.clear();

     if(allSelect->isChecked()){
          for(int i=0; i<FeatureN; ++i){
               check = true;
               FE_index.push_back(i);
               FE_ch[i]->setChecked(true);
          }
     }

     else{
          for(int i=0; i<FeatureN; ++i)
               FE_ch[i]->setChecked(false);
     }
     
     if(check)
          FE_check_switch = true;
     else
          FE_check_switch = false;     
     qDebug() << "FE_index"<<FE_index;
}

void MyWidget::FE_check(){
     bool check = false;
     FE_index.clear();

     for(int i=0; i<FeatureN; ++i){
          if(FE_ch[i]->isChecked()){
               check = true;
               FE_index.push_back(i);
          }
     }

     if(check)
          FE_check_switch = true;
     else
          FE_check_switch = false;
     qDebug() << "FE_index"<<FE_index; 
}

void MyWidget::IA_check(int combo_index){     
     QString line;

     IA_index.clear();
     qDebug() << "combo_index = " << combo_index;
     line.sprintf("Calibration-%d",combo_index);
     IA_line->setText(line);
     
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
     else if (!FE_check_switch){
          qDebug()<<"error: Extraction not select.";
          ST_state->setText("error: Extraction not select.");
     }
     else if(ST_check_switch){

          // image open
          float  sb_val = 0.0;
          int defCol = 0;
          int refCol = 0;

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

          mat2qimage(defImg, initDefImg);
          mat2qimage(refImg, initRefImg);

          qDebug() << "open image done.";
     
          for(int i=0; i<defImg.size(); ++i){
               if(!defImg[i].data || !refImg[i].data){
                    image_check_switch = false;
                    ST_state->setText("error: image is not open ...");
                    qDebug() << "error: NO."<< i << " image is not open ...";
                    exit(-1);
               }
          }

          defName.clear();
          refName.clear();
     }

     qDebug() << "image_check_switch = "<< image_check_switch;
     qDebug() << "ST_check_switch = " << ST_check_switch;

     if(image_check_switch && ST_check_switch){
          qDebug() << "tag-0-(ImPro)";

          ST_state->setText("Calibration ...");
          do_alignment();
          mat2qimage(defImg, proAlDefImg);
          mat2qimage(refImg, proAlRefImg);
          ALIS_build_list();

          QVector<double> _feCal;
          for(int j=1; j<inFile.size(); ++j)
               _feCal.push_back(0.0);
          for(int i=0; i<FE_index.size(); ++i)
               feCal.push_back(_feCal);
          _feCal.clear();

          qDebug() << "tag-1-(ImPro)";          
          ST_state->setText("Feature Extraction ...");
          do_feature_extraction();

          qDebug() << "tag-2-(ImPro)";
          ST_state->setText("Output csv ...");
          writefile();

          qDebug() << "tag-3-(ImPro)";
          ST_state->setText("Ploting ...");
          do_plot_histogram();


          ST_state->setText("Build List ...");
          build_list();
      
          ST_state->setText("Finish ...");
          ST_check_switch = false;
     }
}

void MyWidget::showfile(){
     qDebug()<<"---- showfile";
     
     QFileDialog myFileDialog(this);
     QString fileName = myFileDialog.getOpenFileName(this, "Open File",
               QDir::currentPath (), "CSV Files (*.csv *.xls)");

     if (fileName.isEmpty()) {
          FO_line->setText (myFileDialog.directory().absolutePath());
     } else {
          FO_line->setText (fileName);
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
          qDebug() << i <<  outFile[i];
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

void MyWidget::do_alignment(){
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

void MyWidget::do_plot_histogram(){

     QString name;
     QVector<double> overlap(4,0);

     int bin = 30, interval = 0.0;

     QVector<double> MaxMin(5,0);  // y, y0, y1, x, xmin
     QVector<double> sum(3,0);     // y, y0, y1
     QVector<double> mean(3,0);    // y, y0, y1
     QVector<double> stnd(3,0);    // y, y0, y1

     QString text;
     QVector<QCustomPlot *> hist, hist2;
     QVector<double> feCal_0, feCal_1;
     QVector<double> x, y, y0, y1;
     
     float sb_val = 0.0;
     int type_col = 0;
     bool ok = true;

     for(int i=0; i<inFile[0].size(); ++i){
          if(inFile[0].at(i).contains("Type",Qt::CaseSensitive)){
               type_col = i;
               break;
          }
     }

     qDebug() << "Type_col" << type_col;
    
     for(int i=0; i<feCal.size(); ++i){
          
          for(int j=0; j<feCal[i].size(); ++j){
               MaxMin[3] = (MaxMin[3] > feCal[i][j])?(MaxMin[3]):(feCal[i][j]);
               MaxMin[4] = (MaxMin[4] < feCal[i][j])?(MaxMin[4]):(feCal[i][j]);
          }

          qDebug() << "max/min" <<MaxMin[3]<<MaxMin[4];

          interval = abs((MaxMin[3] - MaxMin[4])/bin) + 1;
          
          qDebug() << "interval" <<interval;
          
          for(int j=0; j<(MaxMin[3] + interval); j=j+interval){
               x.push_back((double)j);
               y.push_back(0.0);
               y0.push_back(0.0);
               y1.push_back(0.0);
          }
     
          for(int j=0; j<feCal[i].size(); ++j){
               
               int buf = abs(feCal[i][j]/interval);
               y[ buf ]++;

               if(inFile[j+1].at(type_col).toInt(&ok,10) == 0){
                    y0[ buf ]++;
                    feCal_0.push_back(feCal[i][j]);
               }
               else if(inFile[j+1 ].at(type_col).toInt(&ok,10) == 1){
                    y1[ buf ]++;
                    feCal_1.push_back(feCal[i][j]);
               }
          }
       
          his_mean_stnd(feCal[i], mean[0], stnd[0]);
          his_mean_stnd(feCal_0,  mean[1], stnd[1]);
          his_mean_stnd(feCal_1,  mean[2], stnd[2]);

          qDebug() <<"mean"<< mean;
          qDebug() << "stnd"<< stnd;

          for(int j=0; j<x.size(); ++j){
               sum[0] += y[j];
               sum[1] += y0[j];
               sum[2] += y1[j];
               if(y0[j]>0&&y1[j]>0){
                    overlap[3] += (y0[j]<y1[j])?(y0[j]):(y1[j]);
               }
          }

          overlap[0] = 100.0*overlap[3]/sum[0];
          overlap[1] = 100.0*overlap[3]/sum[1];
          overlap[2] = 100.0*overlap[3]/sum[2];

          for(int j=0; j<x.size(); ++j){
               if(y[j] == 0) y[j] = 0;
               else y[j] = y[j]/sum[0];

               if(y0[j] == 0) y0[j] = 0;
               else y0[j] = y0[j]/sum[1];
               
               if(y1[j] == 0) y1[j] = 0;
               else y1[j] = y1[j]/sum[2];
          }

          for(int j=0; j<x.size(); ++j){
               MaxMin[0] = (MaxMin[0] > y[j])?(MaxMin[0]):(y[j]);
               MaxMin[1] = (MaxMin[1] > y0[j])?(MaxMin[1]):(y0[j]);
               MaxMin[2] = (MaxMin[2] > y1[j])?(MaxMin[2]):(y1[j]);
          }

          qDebug() << "x[j]" << x;
          qDebug() << "y[j]" << y;
          qDebug() << "y0[j]" << y0;
          qDebug() << "y1[j]" << y1; 
          qDebug() << "max/min = "<< MaxMin[3] << MaxMin[4];

          // ===============================================
          
          hist.push_back(new QCustomPlot(this));
          hist[i]->legend->setVisible(true);
          hist[i]->legend->setFont(QFont("Helvetica", 9));

          hist[i]->resize(640,240);
          hist[i]->addGraph();
          hist[i]->graph(0)->setName("Patch");
          hist[i]->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(QColor(10,140,70,200),2.5),QBrush(Qt::white),4));
          hist[i]->graph(0)->setData(x,y);

          QCPBars *bar1 = new QCPBars(hist[i]->xAxis,hist[i]->yAxis);
          bar1->setData(x,y);
          bar1->setWidth((double)interval);
          bar1->setPen(Qt::NoPen);
          bar1->setBrush(QColor(10,140,70,160));

          hist[i]->xAxis->setLabel("Feature Value");
          hist[i]->yAxis->setLabel("Density");

          hist[i]->xAxis->setRange(0, 1.1*MaxMin[3]);
          hist[i]->yAxis->setRange(0, 1.1*MaxMin[0]);

          text.sprintf("Mean = %.2lf\nSTD = %.2lf",mean[0],stnd[0]);
          QCPItemText *phaseTracerText = new QCPItemText(hist[i]);
          phaseTracerText->position->setType(QCPItemPosition::ptAxisRectRatio);
          phaseTracerText->setPositionAlignment(Qt::AlignRight|Qt::AlignBottom);
          phaseTracerText->position->setCoords(1.0, 0.95); // lower right corner of axis rect
          phaseTracerText->setText(text);
          phaseTracerText->setTextAlignment(Qt::AlignLeft);
          phaseTracerText->setFont(QFont(font().family(), 9));
          phaseTracerText->setPadding(QMargins(8, 0, 0, 0));

          hist[i]->replot();
 
          name.sprintf("Feature-%d.png",FE_index[i]);
          hist[i]->savePng(name,640,240,1);
          _PI_image.push_back(QImage(name));
          // ===============================================

          hist2.push_back(new QCustomPlot(this));
          hist2[i]->legend->setVisible(true);
          hist2[i]->legend->setFont(QFont("Helvetica", 9));

          hist2[i]->resize(640,240);
          hist2[i]->addGraph();
          hist2[i]->graph(0)->setName("Type0");
          hist2[i]->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(QColor(10,250,10,200),2.5),QBrush(Qt::white),4));
          hist2[i]->graph(0)->setData(x,y0);

          QCPBars *bar2 = new QCPBars(hist2[i]->xAxis,hist2[i]->yAxis);
          bar2->setData(x,y0);
          bar2->setWidth((double)interval);
          bar2->setPen(Qt::NoPen);
          bar2->setBrush(QColor(10,255,10,160));

          hist2[i]->addGraph();
          hist2[i]->graph(1)->setName("Type1");
          hist2[i]->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(QColor(255,10,10,200),2.5),QBrush(Qt::white),4));
          hist2[i]->graph(1)->setData(x,y1);

          QCPBars *bar3 = new QCPBars(hist2[i]->xAxis,hist2[i]->yAxis);
          bar3->setData(x,y1);
          bar3->setWidth((double)interval);
          bar3->setPen(Qt::NoPen);
          bar3->setBrush(QColor(255,10,10,160));
          
          hist2[i]->xAxis->setLabel("Feature Value");
          hist2[i]->yAxis->setLabel("Density");
          hist2[i]->xAxis->setRange(0, 1.1*MaxMin[3]);

          double yLabelSize = (MaxMin[1]>MaxMin[2])?(MaxMin[1]):(MaxMin[2]);
          hist2[i]->yAxis->setRange(0, 1.1*yLabelSize);

          text.sprintf("Mean(Type0) = %.2lf\nMaen(Type1) = %.2lf\nSTD(Type0) = %.2lf\nSTD(Type1) = %.2lf\nOverlapping(Type0+1) = %.2lf%%\nOverlapping(Type0) = %.2lf%%\nOverlapping(Type1) = %.2lf%%",
               mean[1],mean[2],stnd[1],stnd[2],overlap[0],overlap[1],overlap[2]);
          phaseTracerText = new QCPItemText(hist2[i]);
          phaseTracerText->position->setType(QCPItemPosition::ptAxisRectRatio);
          phaseTracerText->setPositionAlignment(Qt::AlignRight|Qt::AlignBottom);
          phaseTracerText->position->setCoords(1.0, 0.95); // lower right corner of axis rect
          phaseTracerText->setText(text);
          phaseTracerText->setTextAlignment(Qt::AlignLeft);
          phaseTracerText->setFont(QFont(font().family(), 9));
          phaseTracerText->setPadding(QMargins(8, 0, 0, 0));

          hist2[i]->replot();
          name.sprintf("Feature2-%d.png",FE_index[i]);
          hist2[i]->savePng(name,640,240,1);
          _PI_image2.push_back(QImage(name));
     
          x.clear(); y.clear(); y0.clear(); y1.clear();
          feCal_0.clear();
          feCal_1.clear();

          overArea.push_back(overlap[0]);
          overArea.push_back(overlap[1]);
          overArea.push_back(overlap[2]);
          
          for(int j=0; j<overlap.size(); ++j)
               overlap[j] = 0.0;

          for(int j=0;j<MaxMin.size(); ++j)
               MaxMin[j] = 0.0;
          MaxMin[MaxMin.size()-1] = 100000.0;

          for(int j=0;j<mean.size(); ++j){
               sum[j] = 0.0;
               mean[j] = 0.0;
               stnd[j] = 0.0;
          }

          sb_val += 100./feCal.size();
          SB_pbar->setValue(sb_val+1);
     }
     
     qDebug() << "FE_index.size() = "   << FE_index.size();
     qDebug() << "IA_index.size() = "   << IA_index.size();
     qDebug() << "feCal.size() = "   << feCal.size();
     qDebug() << "feCal[0].size() = "   << feCal[0].size();
     qDebug() << "inFile.size() = "   << inFile.size();
     qDebug() << "inFile[0].size() = "  << inFile[0].size();
     SB_pbar->setValue(100);
}

void MyWidget::his_mean_stnd(QVector<double> inVal, double& mean, double& stnd){
     double sum = 0;
     double sum_diff = 0;
     
     for(int i=0; i<inVal.size(); ++i){
          sum += inVal[i];
     }

     mean = sum/(double)inVal.size();

     for(int i=0; i<inVal.size(); ++i){
          sum_diff += pow((inVal[i] - mean),2);
     }
     stnd = sqrt(sum_diff/(double)inVal.size()); 

     qDebug() << "sum/mean/stnd = " << sum << mean << stnd;
}

void MyWidget::alignmentImageShow(){

     QWidget* window = new QWidget;
     QGridLayout* ALIS_layout = new QGridLayout(window);

     ALIS_list = new QListWidget(window);
     ALIS_label1 = new QLabel(window);
     ALIS_label2 = new QLabel(window);
     ALIS_label3 = new QLabel(window);
     
     QImage ALIS_image1 = QImage(250,250,QImage::Format_RGB888);
     ALIS_label1->resize(250,250);
     ALIS_label1->setPixmap(QPixmap::fromImage(ALIS_image1));

     QImage ALIS_image2 = QImage(250,250,QImage::Format_RGB888);
     ALIS_label2->resize(250,250);
     ALIS_label2->setPixmap(QPixmap::fromImage(ALIS_image2));

     QImage ALIS_image3 = QImage(250,250,QImage::Format_RGB888);
     ALIS_label3->resize(250,250);
     ALIS_label3->setPixmap(QPixmap::fromImage(ALIS_image3));

     ALIS_layout->addWidget(ALIS_label1,0,0);
     ALIS_layout->addWidget(ALIS_label2,1,0);
     ALIS_layout->addWidget(ALIS_label3,2,0);
     ALIS_layout->addWidget(ALIS_list, 0,1,3,1);
     window->show();
}

void MyWidget::ALIS_build_list(){
     QString name;

     for(int i=0; i<initDefImg.size(); ++i){
          name.sprintf("Image-%d",i);
          ALIS_list->insertItem(i,name);
     }
}

void MyWidget::ALIS_previewImg(){
     
     int currentRow = ALIS_list->currentRow();

     ALIS_label1->setPixmap(QPixmap::fromImage(initRefImg[currentRow]));
     ALIS_label2->setPixmap(QPixmap::fromImage(initDefImg[currentRow]));
     ALIS_label3->setPixmap(QPixmap::fromImage(proAlDefImg[currentRow]));
}

void MyWidget::mat2qimage(QVector<Mat> in, QVector<QImage>& out){
     qDebug() << "in mat2qimage";

     if(in[0].type() == CV_8UC1){
          for(int i=0; i<in.size(); ++i){
               out.push_back(QImage((const uchar*)in[i].data, in[i].cols, in[i].rows, in[i].step, QImage::Format_Grayscale8));
               out[i].bits();
          }
     }

     if(in[0].type() == CV_8UC3){
          Mat temp;
          for(int i=0; i<in.size(); ++i){
               cvtColor(in[i], temp,CV_BGR2RGB);
               out.push_back(QImage((uchar*)in[i].data, in[i].cols, in[i].rows, in[i].step, QImage::Format_RGB888));
          }
     }
}



