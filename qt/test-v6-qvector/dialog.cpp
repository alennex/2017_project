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
    
     //ST_state = new QLineEdit(this);
     QPushButton* check = new QPushButton("check",this);
     QPushButton* SB_bu = new QPushButton("Start",this);
     SB_pbar = new QProgressBar(this);
     
     SB_pbar->setRange(0,100);
     SB_pbar->setValue(0);
     connect(SB_bu, SIGNAL(clicked()), this,SLOT(progress()));
     
     SB_layout->addWidget(check);
     SB_layout->addWidget(SB_bu);
     SB_layout->addWidget(SB_pbar);
     StartButton->setLayout(SB_layout);

     // ========= List Widget of result

     QGroupBox* ListResult = new QGroupBox("List of Result", this);
     QVBoxLayout* LR_layout = new QVBoxLayout(this);

     LR_list = new QListWidget(this);
     connect(check, SIGNAL(clicked()), this, SLOT(build_list()));
     connect(LR_list, SIGNAL(itemPressed(QListWidgetItem*)),this, SLOT(LR_check(QListWidgetItem*)));

     LR_layout->addWidget(LR_list);
     ListResult->setLayout(LR_layout);

     // ========= Preview Image

     QGroupBox* PreviewImage = new QGroupBox("Preview Image", this);
     QVBoxLayout* PI_layout = new QVBoxLayout(this);

     QLabel* PI_label = new QLabel(this);
     QImage PI_image = QImage(640,480,QImage::Format_RGB888);
     PI_label->resize(640,480);
     PI_label->setPixmap(QPixmap::fromImage(PI_image));

     PI_layout->addWidget(PI_label);
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

void MyWidget::LR_check(QListWidgetItem* item){
     cout << (item->text()).toStdString() << std::endl;
     //int currentRow = LR_list->currentRow();
     //printf("currentRow = %d \n",currentRow);
}

void MyWidget::build_list(){
     char name[20];

     for(int i=0; i<FeatureN; ++i){
          if(FE_ch[i]->isChecked()){
               sprintf(name, "Feature-%d", i);
               LR_list->insertItem(i,tr(name));
          }
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
     }
     else if (!IA_check_switch){
          qDebug()<<"error: Alignment not select.";
     }
     else if (!FE_check_switch){
          qDebug()<<"error: Extraction not select.";
     }
     else if(ST_check_switch){

          // image open
          
          int defCol = 0;
          int refCol = 0;

          for(int i=0; i<inFile[0].size(); ++i){
               if(inFile[0].at(i).contains("DEF",Qt::CaseSensitive))
                    defCol = i;
               if(inFile[0].at(i).contains("REF",Qt::CaseSensitive))
                    refCol = i;
          }

          QString defName, refName;

          for(int i=1; i<inFile.size(); ++i){
               defName = inFile[i].at(defCol);
               defName = defName.replace(QString("\""),QString(""));
               defName = defName.simplified();

               refName = inFile[i].at(refCol);
               refName = refName.replace(QString("\""),QString(""));
               refName = refName.simplified();

               defImg.push_back(imread(defName.toStdString(),CV_LOAD_IMAGE_GRAYSCALE));
               refImg.push_back(imread(refName.toStdString(),CV_LOAD_IMAGE_GRAYSCALE));
          }

 
          for(int i=0; i<defImg.size(); ++i){
               if(!defImg[i].data || !refImg[i].data)
                    image_check_switch = false;
          }
     }

     if(image_check_switch && ST_check_switch){
          
          do_alignment();

          QVector<double> _feCal;
          for(int j=0; j<inFile.size(); ++j)
               _feCal.push_back(0.0);
          for(int i=0; i<FE_index.size(); ++i)
               feCal.push_back(_feCal);
          
          do_feature_extraction();
     }

     
     qDebug()<<"---- progress";
     for (int i = 0; i < 100; i++){
          SB_pbar->setValue(i+1);
          QThread::msleep(100);
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

     for(int i=0; i<inFile.size(); ++i){
          outFile.push_back(inFile[i]);
          outFile[i].append("hello");
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