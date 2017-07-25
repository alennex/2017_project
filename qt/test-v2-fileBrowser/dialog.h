
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

#define FE_N 9
#define AL_N 9

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
          QLineEdit* FE_line;
          
          QVector<QCheckBox*> FE_ch(9);
          bool* FE_check;

     protected slots:
          void showfile();
          void show_IA_LINE(int);
          void show_chbox(int);
          void progress();
};

