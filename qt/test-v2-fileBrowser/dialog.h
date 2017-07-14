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

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class MyWidget : public QWidget{
	Q_OBJECT
     
     public:
          MyWidget(QWidget *parent = 0);

     private slots:
          void showfile();
     
     private:
     		QGridLayout *mainLayout;

          QPushButton *Bu_fileOpen;
          QLineEdit *fileLineEdit;
          
          QLabel *myLabel;
          QPushButton *Bu_Quit;
          
          QVBoxLayout *FE_Layout;
          QGroupBox *FE_GroupBox;
          QCheckBox *check1;
          QCheckBox *check2;
          QCheckBox *check3;

          QVBoxLayout *AL_Layout;
		QGroupBox *AL_GroupBox;
          QRadioButton *rcheck1;
          QRadioButton *rcheck2;
          QRadioButton *rcheck3;
};