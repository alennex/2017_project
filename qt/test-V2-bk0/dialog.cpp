#include "dialog.h"


MyWidget::MyWidget(QWidget *parent):
QWidget(parent){

     Bu_fileOpen = new QPushButton("open CSV file", this);
     fileLineEdit =  new QLineEdit(this);
     //connect(Bu_fileOpen, SIGNAL(clicked()), fileLineEdit,SLOT(showfile()));
     qDebug() << "fileLineEdit = " << fileLineEdit <<  endl;

     // ========= check box

     QGroupBox *FE_GroupBox = new QGroupBox("Filter Extraction",this);
     QVBoxLayout *FE_Layout = new QVBoxLayout(this);

     check1 = new QCheckBox("FE1",this);
     check2 = new QCheckBox("FE2",this);
     check3 = new QCheckBox("FE3",this);

     FE_Layout->addWidget(check1);
     FE_Layout->addWidget(check2);
     FE_Layout->addWidget(check3);
     FE_GroupBox->setLayout(FE_Layout);

     // =========Radio button

     QGroupBox *AL_GroupBox = new QGroupBox("Aliment",this);
     QVBoxLayout * AL_Layout = new QVBoxLayout(this);

     rcheck1 = new QRadioButton("AL1",this);
     rcheck2 = new QRadioButton("AL2",this);
     rcheck3 = new QRadioButton("AL3",this);
     
     AL_Layout->addWidget(rcheck1);
     AL_Layout->addWidget(rcheck2);
     AL_Layout->addWidget(rcheck3);
     AL_GroupBox->setLayout(AL_Layout);


     // ========= push button to exit
     Bu_Quit = new QPushButton("Quit", this);
     connect(Bu_Quit, SIGNAL(clicked()), qApp, SLOT(quit()));

     // ========= QVBoxLayout

     QGridLayout *mainLayout = new QGridLayout(this);
     mainLayout->addWidget(Bu_fileOpen,1,0);
     mainLayout->addWidget(fileLineEdit,1,1);
     
     mainLayout->addWidget(FE_GroupBox,3,0);
     //mainLayout->addWidget(AL_GroupBox,2,0);
     mainLayout->addWidget(Bu_Quit,4,0);

     //mainLayout->setMargin(10);
     //mainLayout->setSpacing(10);
}


void MyWidget::showfile(){
     
     QFileDialog myFileDialog(this);
     QString fileName = myFileDialog.getOpenFileName(this, "Open File",
               QDir::currentPath (), "CSV Files (*.csv *.xls)");

     if (fileName.isEmpty ()) {
          fileLineEdit->setText (myFileDialog.directory().absolutePath());
     } else {
          fileLineEdit->setText (fileName);
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