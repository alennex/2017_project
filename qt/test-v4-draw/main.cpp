#include <QtGui/QtGui>
#include "mywidget.h"
 
int main( int argc, char **argv )
{
  QApplication app( argc, argv );
 
  MyWidget myWidget;
  myWidget.show();
 
  return app.exec();
}