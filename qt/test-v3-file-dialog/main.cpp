#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QFileDialog>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    QLabel *label = new QLabel("<center>FileDialog</center>");
    label->setWindowTitle("FileDialog");
    label->resize(500, 100);

    label->setFont(QFont( "Times", 18, QFont::Bold ));
    label->show();

    QString fileName = QFileDialog::getOpenFileName(label, "Open File",
                "", "CSV Files (*.csv *.xls)");

    if(fileName != NULL) {
        label->setText("<center>" + fileName + "</center>");
    }

    return app.exec();
}