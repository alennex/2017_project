
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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QProgressBar>
#include <QtCore/QThread>
#include <QtCore/QDebug>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class QPushButton;
class QCheckBox;
class QRadioButton;

class MyWidget : public QWidget{
	Q_OBJECT
     
     public:
          MyWidget(QWidget *parent = 0);

     protected:
          QLineEdit* FO_line;
          QProgressBar* SB_pbar;
          

     protected slots:
          void showfile();
          void progress();
};

#endif