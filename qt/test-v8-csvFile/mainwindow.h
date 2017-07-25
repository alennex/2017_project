
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QGroupBox>


#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QDebug>
#include <QtCore/QVector>
#include <QtCore/QThread>


using namespace std;


class MyWidget : public QWidget{
	Q_OBJECT
     
     public:
          MyWidget(QWidget *parent = 0);
     protected:
     		QLineEdit* FO_line;
     		QVector<QStringList> inFile;
     		QVector<QStringList> outFile;

     	protected slots:
     		void showfile();
     		void readfile(QString);
     		void writefile();

};

