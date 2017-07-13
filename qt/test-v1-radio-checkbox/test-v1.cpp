#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QVBoxLayout>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
private:
    QVBoxLayout *myLayout;
    QGroupBox *myGroupBox;
    QRadioButton *check1;
    QRadioButton *check2;
    QRadioButton *check3;
};

#endif


Widget::Widget(QWidget *parent) :
    QWidget(parent)
{  
    setWindowTitle(tr("按鍵組"));
    myGroupBox = new QGroupBox(tr("這是一組按鍵"),this);
    myGroupBox->setGeometry(30,30,100,100);
    check1 = new QRadioButton(tr("按鍵1"),this);
    check1->setChecked(true);                //設定check1為打勾狀態
    check2 = new QRadioButton(tr("按鍵2"),this);
    check3 = new QRadioButton(tr("按鍵3"),this);
    myLayout = new QVBoxLayout;
    myLayout->addWidget(check1);
    myLayout->addWidget(check2);
    myLayout->addWidget(check3);
    myGroupBox->setLayout(myLayout);
}

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}

