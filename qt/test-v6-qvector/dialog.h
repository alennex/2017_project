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
#include <QtCore/QDir>
#include <QtCore/QFile>

#include "include/alignment.h"
#include "include/feature.h"

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#define FeatureN 5
#define AlignmentN 13

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
          //QLineEdit* ST_state;
          QProgressBar* SB_pbar;
          QVector<QStringList> inFile;
          QVector<QStringList> outFile;

          QLineEdit* IA_line;
          QComboBox* IA_combo;
          
          QVector<QCheckBox*> FE_ch;
          QListWidget* LR_list;

          QVector<int> IA_index;
          QVector<int> FE_index;
          QVector<Mat> defImg;
          QVector<Mat> refImg;
          QVector<QVector<double>> feCal;

          bool FO_check_switch;
          bool IA_check_switch;
          bool FE_check_switch;
          bool ST_check_switch;

     protected slots:
          void showfile();
          void readfile(QString);
          void writefile();

          void IA_check(int);
          void FE_check();
          void LR_check(QListWidgetItem*);
          void build_list();
          void progress();

          void do_alignment();
          void do_feature_extraction();
};

