#include <stdio.h>
#include <stdlib.h>

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
#include <QtCore/QVector>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#define FeatureN 9
#define AlignmentN 9

using namespace cv;
using namespace std;

class QPushButton;
class QCheckBox;
class QRadioButton;

class MyWidget : public QWidget{
	Q_OBJECT
     
     public:
          MyWidget(QWidget *parent = 0);


     signals:
     
     protected:
          QLineEdit* FO_line;
          QProgressBar* SB_pbar;
          
          QLineEdit* IA_line;
          QComboBox* IA_combo;
          
          QVector<QCheckBox*> FE_ch;
          QListWidget* LR_list;

     protected slots:
          void showfile();
          void IA_check(int);
          void FE_check();
          void LR_check(QListWidgetItem*);
          void build_list();
          void progress();
};

