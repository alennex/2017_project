#include "mainwindow.h"

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
     IA_line->setAlignment(Qt::AlignCenter);

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
     connect(LR_list, SIGNAL(itemPressed(QListWidgetItem*)),this, SLOT(previewImg()));

     LR_layout->addWidget(LR_list);
     ListResult->setLayout(LR_layout);

     // ========== preview Img

     QGroupBox* PreviewImage = new QGroupBox("Preview Image", this);
     QVBoxLayout* PI_layout = new QVBoxLayout(this);

     PI_label = new QLabel(this);
     PI_label2 = new QLabel(this);

     QImage PI_image = QImage(800,240,QImage::Format_RGB888);
     PI_label->resize(800,240);
     PI_label->setPixmap(QPixmap::fromImage(PI_image));

     QImage PI_image2 = QImage(800,240,QImage::Format_RGB888);
     PI_label2->resize(800,240);
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
          }

          qDebug() << "open image done.";

          for(int i=0; i<defImg.size(); ++i){
               if(!defImg[i].data || !refImg[i].data){
                    image_check_switch = false;
                    ST_state->setText("error: image is not open ...");
                    qDebug() << "error:"<< i << " image is not open ...";
                    exit(-1);
               }
          }
     }

     qDebug() << "image_check_switch = "<< image_check_switch;
     qDebug() << "ST_check_switch = " << ST_check_switch;

     if(image_check_switch && ST_check_switch){
          qDebug() << "tag-0-(ImPro)";

          ST_state->setText("Alignment ...");
          do_alignment();

          QVector<double> _feCal;
          for(int j=1; j<inFile.size(); ++j)
               _feCal.push_back(0.0);
          for(int i=0; i<FE_index.size(); ++i)
               feCal.push_back(_feCal);

          qDebug() << "tag-1-(ImPro)";          
          ST_state->setText("Feature Extraction ...");
          do_feature_extraction();

          qDebug() << "tag-2-(ImPro)";
          ST_state->setText("Build List ...");
          build_list();

          qDebug() << "tag-3-(ImPro)";
          ST_state->setText("Output csv ...");
          writefile();

          ST_state->setText("Ploting ...");
          do_plot_histogram();
      
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
}

void MyWidget::do_plot_histogram(){

     QString name;
     double min_x = 100000.0, max_x = 0.0, max_y = 0.0, max_y0 = 0.0, max_y1 = 0.0;
     double mean_y  = 0.0, stnd_y  = 0.0;
     double mean_y0 = 0.0, stnd_y0 = 0.0;
     double mean_y1 = 0.0, stnd_y1 = 0.0;

     QString text;
     QVector<QCustomPlot *> hist, hist2;
     QVector<double> feCal_0, feCal_1;
     QVector<double> x;
     QVector<double> y, y0, y1;
     
     float sb_val = 0.0;
     int type_col = 0;
     bool ok = true;

     for(int i=0; i<inFile[0].size(); ++i){
          if(inFile[0].at(i).contains("Type",Qt::CaseSensitive)){
               type_col = i;
               break;
          }
     }

     qDebug() << "Type_col = " << type_col;
    
     for(int i=0; i<feCal.size(); ++i){
          
          for(int j=0; j<feCal[i].size(); ++j){
               max_x = (max_x > feCal[i][j])?(max_x):(feCal[i][j]);
               min_x = (min_x < feCal[i][j])?(min_x):(feCal[i][j]);
          }

          for(int j=0; j<(max_x+1); ++j){
               x.push_back((double)j);
               y.push_back(0.0);
               y0.push_back(0.0);
               y1.push_back(0.0);
          }
     
          for(int j=0; j<feCal[i].size(); ++j){

               y[ feCal[i][j] ]++;

               if(inFile[j+1].at(type_col).toInt(&ok,10) == 0){
                    y0[ feCal[i][j] ]++;
                    feCal_0.push_back(feCal[i][j]);
               }
               else if(inFile[j+1 ].at(type_col).toInt(&ok,10) == 1){
                    y1[ feCal[i][j] ]++;
                    feCal_1.push_back(feCal[i][j]);
               }
          }
       
          his_mean_stnd(feCal[i],  mean_y,  stnd_y );
          his_mean_stnd(feCal_0, mean_y0, stnd_y0);
          his_mean_stnd(feCal_1, mean_y1, stnd_y1);

          for(int j=0; j<(max_x+1); ++j){
               max_y  = (max_y  > y[j] )?(max_y ):(y[j] );
               max_y0 = (max_y0 > y0[j])?(max_y0):(y0[j]);
               max_y1 = (max_y1 > y1[j])?(max_y1):(y1[j]);
          }

          max_x = max_x + 0.0001;
          max_y = max_y + 0.0001;
          max_y0 = max_y0 + 0.0001;
          max_y1 = max_y1 + 0.0001;

          for(int j=0; j<(max_x+1); ++j){
               x[j] = x[j]/max_x;
               y[j] = y[j]/max_y;
               y0[j] = y0[j]/max_y0;
               y1[j] = y1[j]/max_y1;
          }

          // ===============================================
          qDebug() << "max/min = "<< min_x << max_x;

          hist.push_back(new QCustomPlot(this));
          hist[i]->addGraph(); 
          hist[i]->legend->setVisible(true);
          hist[i]->legend->setFont(QFont("Helvetica", 9));

          hist[i]->resize(800,240);
          hist[i]->graph(0)->setPen(QPen(Qt::blue));
          hist[i]->graph(0)->setBrush(QColor(0, 0, 255, 100));
          hist[i]->graph(0)->setData(x,y);
          
          hist[i]->xAxis->setLabel("Feature Value");
          hist[i]->yAxis->setLabel("Num");

          hist[i]->xAxis->setRange(0, 1.5);
          hist[i]->yAxis->setRange(0, 1.2);

          text.sprintf("Mean = %.2lf\nSTD = %.2lf",mean_y,stnd_y);
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
          hist[i]->savePng(name,800,240,1);
          _PI_image.push_back(QImage(name));
          // ===============================================

          hist2.push_back(new QCustomPlot(this));
          hist2[i]->legend->setVisible(true);
          hist2[i]->legend->setFont(QFont("Helvetica", 9));

          hist2[i]->resize(800,240);
          hist2[i]->addGraph();
          hist2[i]->graph(0)->setName("Type 0");
          hist2[i]->graph(0)->setPen(QPen(Qt::blue));
          hist2[i]->graph(0)->setBrush(QColor(0, 0, 255, 100));
          hist2[i]->graph(0)->setData(x,y0);
          
          hist2[i]->addGraph();
          hist2[i]->graph(1)->setName("Type 1");
          hist2[i]->graph(1)->setPen(QPen(Qt::red));
          hist2[i]->graph(1)->setBrush(QColor(255, 0, 0, 100));
          hist2[i]->graph(1)->setData(x,y1);
          
          hist2[i]->xAxis->setLabel("Feature Value");
          hist2[i]->yAxis->setLabel("Num");
          hist2[i]->xAxis->setRange(0, 1.5);
          hist2[i]->yAxis->setRange(0, 1.2);

          text.sprintf("Mean(0) = %.2lf\nMaen(1) = %.2lf\nSTD(0) = %.2lf\nSTD(1) = %.2lf",mean_y0,mean_y1,stnd_y0,stnd_y1);
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
          hist2[i]->savePng(name,800,240,1);
          _PI_image2.push_back(QImage(name));
     
          x.clear(); y.clear(); y0.clear(); y1.clear();
          feCal_0.clear();
          feCal_1.clear();

          max_x = 0.0, max_y = 0.0, max_y0 = 0.0, max_y1 = 0.0;
          mean_y = 0.0, mean_y0 = 0.0, mean_y1 = 0.0;
          stnd_y = 0.0, stnd_y0 = 0.0, stnd_y1 = 0.0;
          
          sb_val += 100./feCal.size();
          qDebug() << sb_val;
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





