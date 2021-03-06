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
#include <QtCore/QDate>
#include <QtCore/QTime>
//#pragmaexecution_character_set("utf-8")
#include "qcustomplot.h"

#include "include/alignment.h"
#include "include/feature.h"

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#define FeatureN 8
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
          QLineEdit* ST_state;
          QProgressBar* SB_pbar;
          QVector<QStringList> inFile;
          QVector<QStringList> outFile;
          QVector<QString> SpName;

          QVector<QLabel*> infoLabel;
          QVector<QProgressBar*> infoBar;
          QVector<QGridLayout*> infoLayout;
          QVector<QGroupBox*> infoGroup;

          QComboBox* IA_combo;

          QVector<int> IA_index;
          QVector<int> FE_index;
          QVector<Mat> defImg;
          QVector<Mat> refImg;
          QVector<QVector<double>> feCal;
          QVector<QVector<double>> feOverArea;
          QVector<QVector<QString>> feOverAreaSortFname;

          bool FO_check_switch;
          bool IA_check_switch;
          bool FE_check_switch;
          bool ST_check_switch;

     protected slots:
          void overAreaCal_Type(QVector<double>, QVector<double>, QVector<double>&);
          void overAreaCal();

          void openfile();
          void readfile(QString);
          void writefile();
          void allSelectFunc();

          void IA_check(int);
          void progress();

          void do_calibration();
          void do_feature_extraction();
          void swap(double&, double&, QString&, QString&);
          void selectMin();

          QString alignmentName(int);
          QString featureName(int);
};

