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
          QLineEdit* FO_line;
          QLineEdit* ST_state;
          QProgressBar* SB_pbar;
          QVector<QStringList> inFile;
          QVector<QStringList> outFile;

          QLineEdit* IA_line;
          QComboBox* IA_combo;
          QCheckBox* allSelect;
          
          QListWidget* LR_list;
          QListWidget* ALIS_list;
          QLabel* PI_label;
          QLabel* PI_label2;
          QLabel* ALIS_label1;
          QLabel* ALIS_label2;
          QLabel* ALIS_label3;

          QVector<QCheckBox*> FE_ch;
          QVector<QImage> _PI_image;
          QVector<QImage> _PI_image2;
          QVector<QImage> initDefImg;
          QVector<QImage> initRefImg;
          QVector<QImage> proAlDefImg;
          QVector<QImage> proAlRefImg;

          QVector<int> IA_index;
          QVector<int> FE_index;
          QVector<int> overArea;
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
          void allSelectFunc();

          void IA_check(int);
          void FE_check();
          void build_list();
          void progress();
          void previewImg();

          void do_plot_histogram();
          void do_alignment();
          void do_feature_extraction();
          void his_mean_stnd(QVector<double>, double&, double&);
          void alignmentImageShow();
          void mat2qimage(QVector<Mat>, QVector<QImage>&);
          void ALIS_build_list();
          void ALIS_previewImg();
          QString alignmentName(int);
          QString featureName(int);

};

