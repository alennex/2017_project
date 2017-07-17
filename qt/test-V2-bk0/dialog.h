
#ifndef WINDOW_H
#define WINDOW_H

#include <QtWidgets/QApplication>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QGridLayout>
#include <QtCore/QDebug>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class QPushButton;
class MyWidget : public QWidget{
	Q_OBJECT
     
     public:
          MyWidget(QWidget *parent = 0);

     protected:
     	  
          QPushButton *Bu_fileOpen;
          QLineEdit *fileLineEdit;
          
          QLabel *myLabel;
          QPushButton *Bu_Quit;
          
          QCheckBox *check1;
          QCheckBox *check2;
          QCheckBox *check3;

          QRadioButton *rcheck1;
          QRadioButton *rcheck2;
          QRadioButton *rcheck3;

     protected slots:
          void showfile();
};

#endif