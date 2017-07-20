
#include <QtWidgets/QPushButton>

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>

#include <QtCore/QDebug>
#include <QtCore/QVector>
#include <qcustomplot.h>

using namespace std;


class MyWidget : public QWidget{
	Q_OBJECT
     
     public:
          MyWidget(QWidget *parent = 0);

};

