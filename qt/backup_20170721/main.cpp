#include <dialog.h>

int main(int argc, char *argv[]){

     QApplication app(argc, argv);
     MyWidget widget;
     widget.setWindowTitle(QObject::tr("Patch Image Alaystics"));
     
     widget.show();
     return app.exec();
}