#include "dialog.h"


MyWidget::MyWidget(QWidget *parent):
QWidget(parent){

     // ========= File Open
     QGroupBox* FileOpen = new QGroupBox("Step 1: File Open", this);
     QVBoxLayout* FO_layout = new QVBoxLayout(this);
    
     QPushButton* FO_bu = new QPushButton("open CSV file",this);
     FO_line =  new QLineEdit(this);
     
     connect(FO_bu, SIGNAL(clicked()), this,SLOT(showfile()));
     
     FO_layout->addWidget(FO_bu);
     FO_layout->addWidget(FO_line);
     FileOpen->setLayout(FO_layout);

     // ========= Image Alignment
     QGroupBox* ImageAlignment = new QGroupBox("Step 2: Image Alignment", this);
     QVBoxLayout* IA_layout = new QVBoxLayout(this);

     QComboBox* IA_combo = new QComboBox(this);
     IA_combo->setEditable(false);
     IA_combo->insertItem(0,tr("Alignment-0"));
     IA_combo->insertItem(1,tr("Alignment-1"));
     IA_combo->insertItem(2,tr("Alignment-2"));
     IA_combo->insertItem(3,tr("Alignment-3"));
     
     IA_layout->addWidget(IA_combo);
     ImageAlignment->setLayout(IA_layout);

     // ========= Feature Extraction
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

     // ========= Mainmenu Layout
     QGridLayout *mainLayout = new QGridLayout(this);
     mainLayout->addWidget(FileOpen,0,0);
     mainLayout->addWidget(ImageAlignment,0,1);
     mainLayout->addWidget(FeatureExtraction,0,2);

}


void MyWidget::showfile(){
     
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
     // ========= open / show image
     Mat opencv_image = imread("ref128.png", CV_LOAD_IMAGE_COLOR);
     Mat dest ;
     cvtColor(opencv_image, dest,CV_BGR2RGB);
     QImage image((uchar*)dest.data, dest.cols, dest.rows,QImage::Format_RGB888);

     myLabel = new QLabel(this);
     myLabel->resize(150,150);
     //myLabel->setGeometry(30,30,150,150);
     myLabel->setPixmap(QPixmap::fromImage(image));
     myLabel->show();

     // =========Radio button
     QGroupBox *AL_GroupBox = new QGroupBox("Aliment",this);
     AL_GroupBox->resize(100,100);
     rcheck1 = new QRadioButton("AL1",this);
     rcheck2 = new QRadioButton("AL2",this);
     rcheck3 = new QRadioButton("AL3",this);
     
     QVBoxLayout * AL_Layout = new QVBoxLayout(this);
     AL_Layout->addWidget(rcheck1);
     AL_Layout->addWidget(rcheck2);
     AL_Layout->addWidget(rcheck3);
     AL_GroupBox->setLayout(AL_Layout);

*/