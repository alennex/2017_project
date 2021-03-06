#include "dialog.h"


MyWidget::MyWidget(QWidget *parent):
QWidget(parent){

     // ========= File Open
     qDebug()<<"File Open";
     QGroupBox* FileOpen = new QGroupBox("Step 1: File Open", this);
     QVBoxLayout* FO_layout = new QVBoxLayout(this);
    
     QPushButton* FO_bu = new QPushButton("open CSV file",this);
     FO_line =  new QLineEdit(this);
     
     connect(FO_bu, SIGNAL(clicked()), this,SLOT(showfile()));
     
     FO_layout->addWidget(FO_bu);
     FO_layout->addWidget(FO_line);
     FileOpen->setLayout(FO_layout);

     // ========= Image Alignment
     qDebug()<<"Image Alignment";
     QGroupBox* ImageAlignment = new QGroupBox("Step 2: Image Alignment", this);
     QVBoxLayout* IA_layout = new QVBoxLayout(this);

     QComboBox* IA_combo = new QComboBox(this);
     IA_line = new QLineEdit(this);

     IA_combo->setEditable(false);
     IA_combo->insertItem(0,tr("Alignment-0"));
     IA_combo->insertItem(1,tr("Alignment-1"));
     IA_combo->insertItem(2,tr("Alignment-2"));
     IA_combo->insertItem(3,tr("Alignment-3"));

     connect(IA_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(show_IA_LINE(int)));

     IA_layout->addWidget(IA_combo);
     IA_layout->addWidget(IA_line);
     ImageAlignment->setLayout(IA_layout);

     // ========= Feature Extraction
     qDebug()<<"Feature Extraction";
     QGroupBox* FeatureExtraction = new QGroupBox("Step 3: Feature Extraction", this);
     QGridLayout* FE_layout = new QGridLayout(this);

     char name[20];
     
     for(int i=0; i<FE_N; ++i){
          sprintf(name, "Feature-%d", i);
          FE_ch.at(i) = new QCheckBox(name, this);
          //connect(FE_ch[i], SIGNAL(stateChanged(int)), this, SLOT(show_chbox(int)));
     }
     printf("hi");
     for(int i=0; i<FE_N; ++i){
          FE_layout->addWidget(FE_ch.at(i), (i/4) ,(i%4));
     }

     FeatureExtraction->setLayout(FE_layout);

     // ========= Start Button
     qDebug()<<"Start Button";
     QGroupBox* StartButton = new QGroupBox("Step 4: Start", this);
     QVBoxLayout* SB_layout = new QVBoxLayout(this);
    
     QPushButton* SB_bu = new QPushButton("Start",this);
     SB_pbar = new QProgressBar(this);
     
     SB_pbar->setRange(0,100);
     SB_pbar->setValue(0);
     connect(SB_bu, SIGNAL(clicked()), this,SLOT(progress()));
     
     SB_layout->addWidget(SB_bu);
     SB_layout->addWidget(SB_pbar);
     StartButton->setLayout(SB_layout);


     // ========= List Widget of result
     qDebug()<<"List Widget of result";
     QGroupBox* ListResult = new QGroupBox("List of Result", this);
     QVBoxLayout* LR_layout = new QVBoxLayout(this);

     QListWidget* LR_list = new QListWidget(this);
     LR_list->insertItem(0,"Feature-0");
     LR_list->insertItem(1,"Feature-1");
     LR_list->insertItem(3,"Feature-3");
     LR_list->insertItem(4,"Feature-4");
     LR_list->insertItem(6,"Feature-6");
     LR_list->insertItem(7,"Feature-7");

     LR_layout->addWidget(LR_list);
     ListResult->setLayout(LR_layout);

     // ========= Preview Image
     qDebug()<<"Preview Image";
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

}

void MyWidget::show_chbox(int check){
     printf("hello %d\n", check);
     /*
     for(int i=0; i<*FE_N; ++i){
          if(FE_ch[i].isChecked()){
               FE_check[i] = true;
          }
          else{
               FE_check[i] = false;
          }
     }

     for(int i=0; i<*FE_N; ++i)
          printf("\t%b",FE_check[i]);
     printf("\n");
     */
}

void MyWidget::show_IA_LINE(int combo_index){     
     QString line;
     line.sprintf("Alignment-%d",combo_index);
     IA_line->setText(line);
}

void MyWidget::progress(){
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

     if (fileName.isEmpty ()) {
          FO_line->setText (myFileDialog.directory().absolutePath());
     } else {
          FO_line->setText (fileName);
     }
}


/*
     qDebug()<<"Feature Extraction";
     QGroupBox* FeatureExtraction = new QGroupBox("Step 3: Feature Extraction", this);
     QGridLayout* FE_layout = new QGridLayout(this);

     QCheckBox* FE_ch0 = new QCheckBox("Feature-0",this);
     QCheckBox* FE_ch1 = new QCheckBox("Feature-1",this);
     QCheckBox* FE_ch2 = new QCheckBox("Feature-2",this);
     QCheckBox* FE_ch3 = new QCheckBox("Feature-3",this);
     QCheckBox* FE_ch4 = new QCheckBox("Feature-4",this);
     QCheckBox* FE_ch5 = new QCheckBox("Feature-5",this);
     QCheckBox* FE_ch6 = new QCheckBox("Feature-6",this);
     QCheckBox* FE_ch7 = new QCheckBox("Feature-7",this);
     QCheckBox* FE_ch8 = new QCheckBox("Feature-8",this);
     
     FE_line = new QLineEdit(this);

     connect(FE_ch0, SIGNAL(stateChanged(int)), this, SLOT(setLineEdit1Enabled(int)));    
     
     FE_layout->addWidget(FE_ch0,0,0);
     FE_layout->addWidget(FE_ch1,0,1);
     FE_layout->addWidget(FE_ch2,0,2);
     FE_layout->addWidget(FE_ch3,0,3);
     FE_layout->addWidget(FE_ch4,1,0);
     FE_layout->addWidget(FE_ch5,1,1);
     FE_layout->addWidget(FE_ch6,1,2);
     FE_layout->addWidget(FE_ch7,1,3);
     FE_layout->addWidget(FE_ch8,2,0);

     FeatureExtraction->setLayout(FE_layout);

*/
