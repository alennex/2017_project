#include <QtGui>
#include <QtCore>
#include "layoutdlg.h"
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    layoutdlg helloworld;
    helloworld.show();
    return app.exec();
}