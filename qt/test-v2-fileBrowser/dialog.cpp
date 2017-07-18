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
     QLineEdit* IA_line = new QLineEdit(this);

     IA_combo->setEditable(false);
     IA_combo->insertItem(0,tr("Alignment-0"));
     IA_combo->insertItem(1,tr("Alignment-1"));
     IA_combo->insertItem(2,tr("Alignment-2"));
     IA_combo->insertItem(3,tr("Alignment-3"));
     IA_line->setText("select result");

     IA_layout->addWidget(IA_combo);
     IA_layout->addWidget(IA_line);
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

     // ========= Start Button
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

void MyWidget::progress(){

     for (int i = 0; i < 100; i++){
          SB_pbar->setValue(i+1);
          QThread::msleep(100);
     }

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