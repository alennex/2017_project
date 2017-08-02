#include "mainwindow.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle(tr("QTableWidget"));
    myTableWidget = new QTableWidget(this);
    myTableWidget->setRowCount(4);     //設定QTableWidget列數
    myTableWidget->setColumnCount(3);  //設定QTableWidget行數
    myTableWidget->resize(250, 200);

    QStringList header;
    header << tr("名字") << tr("分數");
    myTableWidget->setHorizontalHeaderLabels(header); //設定QTableWidget標頭

    myTableWidget->setItem(0, 0, new QTableWidgetItem("peter"));
    myTableWidget->setItem(1, 0, new QTableWidgetItem("bill"));
    myTableWidget->setItem(0, 1, new QTableWidgetItem(QIcon("good.jpg"), "良好"));
    myTableWidget->setItem(1, 1, new QTableWidgetItem(QIcon("normal.jpg"), "普通"));
}
