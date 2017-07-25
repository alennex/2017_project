#include "dialog.h"

MyWidget::MyWidget(QWidget *parent):
QWidget(parent){
     
     FO_check_switch = false;
     IA_check_switch = false;
     FE_check_switch = false;
     ST_check_switch = true;

     char name[20];

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
     QVBoxLayout* IA_layout = new QVBoxLayout(this);

     IA_line = new QLineEdit(this);
     IA_combo = new QComboBox(this);

     IA_combo->setEditable(false);
     IA_line->setReadOnly(true);

     for(int i=0; i<AlignmentN; ++i){
          sprintf(name, "Alignment-%d", i);
          IA_combo->insertItem(i,name);
     }
    
     connect(IA_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(IA_check(int)));

     IA_layout->addWidget(IA_combo);
     IA_layout->addWidget(IA_line);
     ImageAlignment->setLayout(IA_layout);

     // ========= Feature Extraction

     QGroupBox* FeatureExtraction = new QGroupBox("Step 3: Feature Extraction", this);
     QGridLayout* FE_layout = new QGridLayout(this);

     for(int i=0; i<FeatureN; ++i){
          sprintf(name, "Feature-%d", i);
          FE_ch.push_back(new QCheckBox(name, this));
     }

     for(int i=0; i<FeatureN; ++i){
          connect(FE_ch[i], SIGNAL(stateChanged(int)), this, SLOT(FE_check()));
     }

     for(int i=0; i<FeatureN; ++i){
          FE_layout->addWidget(FE_ch.at(i), (i/4) ,(i%4));
     }

     FeatureExtraction->setLayout(FE_layout);

     // ========= Start Button

     QGroupBox* StartButton = new QGroupBox("Step 4: Start", this);
     QVBoxLayout* SB_layout = new QVBoxLayout(this);
    
     ST_state = new QLineEdit(this);
     QPushButton* SB_bu = new QPushButton("Start",this);
     SB_pbar = new QProgressBar(this);
     
     ST_state->setReadOnly(true);
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
     //connect(check, SIGNAL(clicked()), this, SLOT(build_list()));
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
     mainLayout->addWidget(ListResult,2,0);
     mainLayout->addWidget(PreviewImage,2,1,2,3);
     setLayout(mainLayout);

}

void MyWidget::previewImg(){
     int currentRow = LR_list->currentRow();
     PI_label->setPixmap(QPixmap::fromImage(_PI_image[currentRow]));
     PI_label2->setPixmap(QPixmap::fromImage(_PI_image2[currentRow]));
}

void MyWidget::build_list(){
     char name[20];

     for(int i=0; i<FE_index.size(); ++i){
          sprintf(name, "Feature-%d", FE_index[i]);
          LR_list->insertItem(i,tr(name));
     }
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
}

void MyWidget::IA_check(int combo_index){     
     QString line;

     IA_index.clear();
     qDebug() << "combo_index = " << combo_index;
     line.sprintf("Alignment-%d",combo_index);
     IA_line->setText(line);
     
     IA_index.push_back(combo_index);
     IA_check_switch = true;
}

void MyWidget::progress(){

     bool image_check_switch = true;
     float  sb_val = 0.0, sb_step = 0.0;
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
          
          int defCol = 5;
          int refCol = 6;

          //for(int i=0; i<inFile[0].size(); ++i){
          //     if(inFile[0].at(i).contains("DEF",Qt::CaseSensitive))
          //          defCol = i;
          //     if(inFile[0].at(i).contains("REF",Qt::CaseSensitive))
          //          refCol = i;
          //}

          qDebug() << "defCol = " << defCol;
          qDebug() << "refCol = " << refCol;

          QString defName, refName;
          sb_step = 20.0/inFile.size();
          ST_state->setText("Open Image ...");

          for(int i=1; i<inFile.size(); ++i){
               
               defName = inFile[i].at(defCol);
               defName = defName.replace(QString("\""),QString(""));
               defName = defName.simplified();

               refName = inFile[i].at(refCol);
               refName = refName.replace(QString("\""),QString(""));
               refName = refName.simplified();

               qDebug() << "defName/refName = "<< defName << refName;

               defImg.push_back(imread(defName.toStdString(),CV_LOAD_IMAGE_GRAYSCALE));
               refImg.push_back(imread(refName.toStdString(),CV_LOAD_IMAGE_GRAYSCALE));
               
               sb_val += sb_step;
               SB_pbar->setValue(sb_val+1);
          }

          for(int i=0; i<defImg.size(); ++i){
               if(!defImg[i].data || !refImg[i].data){
                    image_check_switch = false;
                    ST_state->setText("error: image is not open ...");
                    qDebug() << "error:"<< i << " image is not open ...";
                    exit(-1);
               }
          }
     }

     if(image_check_switch && ST_check_switch){
          ST_state->setText("Alignment ...");
          do_alignment();
          sb_val += 5;
          SB_pbar->setValue(sb_val+1);

          QVector<double> _feCal;
          for(int j=1; j<inFile.size(); ++j)
               _feCal.push_back(0.0);
          for(int i=0; i<FE_index.size(); ++i)
               feCal.push_back(_feCal);
          
          ST_state->setText("Feature Extraction ...");
          do_feature_extraction();
          sb_val += 30;
          SB_pbar->setValue(sb_val+1);

          ST_state->setText("Build List ...");
          build_list();
          sb_val += 5;
          SB_pbar->setValue(sb_val+1);

          ST_state->setText("Output csv ...");
          writefile();
          sb_val += 20;
          SB_pbar->setValue(sb_val+1);

          ST_state->setText("Ploting ...");
          do_plot_histogram();
          sb_val += 20;
          SB_pbar->setValue(sb_val+1);
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
     
     QString _fileName = "outFile.csv";

     QFile file(_fileName);
     if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
          qDebug() << "Cannot open file for writing: "<< qPrintable(file.errorString());
          exit(0);
     }
     
     QTextStream out(&file); 
     QString name, value;

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
     }   

     for(int i=0; i<outFile.size(); ++i){
          for(int j=0; j<outFile[0].size(); ++j){
               out<<outFile[i].at(j)<<",";
          }
          out<<"\n";
     }

     file.close();
}

void MyWidget::do_alignment(){
     Alignment af;

     switch(IA_index[0]){
          case 0:
               for(int i=0; i<defImg.size();++i) af.alignECC_Translation(defImg[i],refImg[i]);
               break;
          case 1:
               for(int i=0; i<defImg.size();++i) af.alignECC_Euclidean(defImg[i],refImg[i]);
               break;
          case 2:
               for(int i=0; i<defImg.size();++i) af.alignECC_Affine(defImg[i],refImg[i]);
               break;
          case 3:
               for(int i=0; i<defImg.size();++i) af.alignECC_Homography(defImg[i],refImg[i]);
               break;
          case 4:
               for(int i=0; i<defImg.size();++i) af.alignDCT(defImg[i],refImg[i]);
               break;
          case 5:
               for(int i=0; i<defImg.size();++i) af.alignDFT(defImg[i],refImg[i]);
               break;
          case 6:
               for(int i=0; i<defImg.size();++i) af.histogramEqualization(defImg[i],refImg[i]);
               break;
          case 7:
               for(int i=0; i<defImg.size();++i) af.threshOtsu(defImg[i],refImg[i]);
               break;
          case 8:
               for(int i=0; i<defImg.size();++i) af.threshBinary(defImg[i],refImg[i]);
               break;
          case 9:
               for(int i=0; i<defImg.size();++i) af.threshBinaryINV(defImg[i],refImg[i]);
               break;
          case 10:
               for(int i=0; i<defImg.size();++i) af.threshTrunc(defImg[i],refImg[i]);
               break;
          case 11:
               for(int i=0; i<defImg.size();++i) af.threshToZero(defImg[i],refImg[i]);
               break;
          case 12:
               for(int i=0; i<defImg.size();++i) af.threshToZeroINV(defImg[i],refImg[i]);
               break;
          default:
               break;
     }
}

void MyWidget::do_feature_extraction(){
     Feature fe;
     int color_gap = 3;
     int pool_size = 4;

     for(int i=0; i<FE_index.size(); ++i){
          switch(FE_index[i]){
               case 0:
                    for(int j=0; j<defImg.size(); ++j){
                         feCal[i][j] = (double)fe.diffGlobalMean(defImg[j], refImg[j]);   
                    } 
                    break;
               case 1:
                    for(int j=0; j<defImg.size(); ++j){
                         feCal[i][j] = (double)fe.diffPixelsCount(defImg[j], refImg[j], color_gap);
                    }
                    break;
               case 2:
                    for(int j=0; j<defImg.size(); ++j){
                         feCal[i][j] = fe.getPSNR(defImg[j], refImg[j]);
                    }
                    break;
               case 3:
                    for(int j=0; j<defImg.size(); ++j){
                         feCal[i][j] = fe.localmeanSSE(defImg[j], refImg[j], pool_size);
                    }
                    break;
               case 4:
                    for(int j=0; j<defImg.size(); ++j){
                         feCal[i][j] = fe.localmeanSAE(defImg[j], refImg[j], pool_size);
                    }
                    break;
               default:
                    break;
          }
     }
}

void MyWidget::do_plot_histogram(){

     QString name;
     double min_x = 100000.0, max_x = 0.0, max_y = 0.0, max_y0 = 0.0, max_y1 = 0.0;

     QVector<QCustomPlot *> hist, hist2;
     QVector<double> x;
     QVector<double> y, y0, y1;
     
     int type_col = 4;
     bool ok = true;

     //for(int i=0; i<inFile[0].size(); ++i){
     //     if(inFile[0].at(i).contains("Type",Qt::CaseSensitive)){
     //          type_col = i;
     //          break;
     //     }
     //}

     qDebug() << "Type_col = " << type_col;
    
     for(int i=0; i<feCal.size(); ++i){
          
          for(int j=0; j<feCal[i].size(); ++j){
               max_x = (max_x > feCal[i][j])?(max_x):(feCal[i][j]);
               min_x = (min_x < feCal[i][j])?(min_x):(feCal[i][j]);
          }

          for(int j=0; j<max_x; ++j){
               x.push_back((double)j);
               y.push_back(0.0);
               y0.push_back(0.0);
               y1.push_back(0.0);
          }
     
          for(int j=0; j<feCal[i].size(); ++j){

               y[ feCal[i][j] ]++;

               if(inFile[j+1].at(type_col).toInt(&ok,10) == 0){
                    y0[ feCal[i][j] ]++;
               }
               else if(inFile[j+1 ].at(type_col).toInt(&ok,10) == 1){
                    y1[ feCal[i][j] ]++;
               }
          }

          for(int j=0; j<max_x; ++j){
               max_y  = (max_y  > y[j] )?(max_y ):(y[j] );
               max_y0 = (max_y0 > y0[j])?(max_y0):(y0[j]);
               max_y1 = (max_y1 > y1[j])?(max_y1):(y1[j]);
          }

          for(int j=0; j<max_x; ++j){
               y[j]  = y[j]/max_y;
               y0[j] = y0[j]/max_y0;
               y1[j] = y1[j]/max_y1;
          }

          for(int j=0; j<max_x; ++j)
               qDebug() << "j, x[j], y[j] , y0[j], y1[j] = " << j << x[j] << y[j] << y0[j] << y1[j];

          hist.push_back(new QCustomPlot(this));
          hist[i]->addGraph(); 
          hist[i]->resize(640,240);
          hist[i]->graph(0)->setData(x,y);
          hist[i]->graph(0)->setPen(QPen(Qt::blue));
          hist[i]->graph(0)->setBrush(QColor(0, 0, 255, 20));
          hist[i]->graph(0)->setLineStyle(QCPGraph::lsStepCenter);

          hist[i]->xAxis->setLabel("Feature Value");
          hist[i]->yAxis->setLabel("Num");
          hist[i]->xAxis->setRange(min_x, max_x);
          hist[i]->yAxis->setRange(0, 1);
          hist[i]->replot();
 
          name.sprintf("Feature-%d.png",FE_index[i]);
          hist[i]->savePng(name,640,240,1);
          _PI_image.push_back(QImage(name));




          hist2.push_back(new QCustomPlot(this));
          hist2[i]->addGraph();
          hist2[i]->resize(640,240);
          hist2[i]->graph(0)->setPen(QPen(Qt::blue));
          hist2[i]->graph(0)->setBrush(QColor(0, 0, 255, 20));
          hist2[i]->graph(0)->setData(x,y0);
          hist2[i]->graph(0)->setName("Type 0");
          hist2[i]->graph(0)->setLineStyle(QCPGraph::lsStepCenter);
 
          hist2[i]->addGraph();
          hist2[i]->graph(1)->setPen(QPen(Qt::red));
          hist2[i]->graph(1)->setBrush(QColor(255, 0, 0, 20));
          hist2[i]->graph(1)->setData(x,y1);
          hist2[i]->graph(1)->setName("Type 1");
          hist2[i]->graph(1)->setLineStyle(QCPGraph::lsStepCenter);

          hist2[i]->xAxis->setLabel("Feature Value");
          hist2[i]->yAxis->setLabel("Num");
          hist2[i]->xAxis->setRange(min_x, max_x);
          hist2[i]->yAxis->setRange(0, 1);
          hist2[i]->replot();
          name.sprintf("Feature2-%d.png",FE_index[i]);
          hist2[i]->savePng(name,640,240,1);
          _PI_image2.push_back(QImage(name));
     
          for(int j=0; j<max_x; ++j){
               x.pop_back();
               y.pop_back(); y0.pop_back(); y1.pop_back();
          }

          max_x = 0.0, max_y = 0.0, max_y0 = 0.0, max_y1 = 0.0;
     }
     
     qDebug() << "FE_index.size() = "   << FE_index.size();
     qDebug() << "IA_index.size() = "   << IA_index.size();
     qDebug() << "feCal.size() = "   << feCal.size();
     qDebug() << "feCal[0].size() = "   << feCal[0].size();
     qDebug() << "inFile.size() = "   << inFile.size();
     qDebug() << "inFile[0].size() = "  << inFile[0].size();
}


void MyWidget::Mean_std(QVector<double> inVal, double mean, double std){
     double sum = 0;
     double sum_diff = 0;

     for(int i=0; i<inVal.size(); ++i){
          sum += inVal[i];
     }

     mean = sum/(double)inVal.size();

     for(int i=0; i<inVal.size(); ++i){
          sum_diff += pow((inVal[i] - mean),2);
     }
     std = sqrt(sum_diff/(double)inVal.size()); 

}