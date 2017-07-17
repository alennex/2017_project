#include <QtGui>
#include "layoutdlg.h"
layoutdlg::layoutdlg(QDialog *parent)
 : QDialog(parent)
{
    QLabel *label1 = new QLabel("Lable1");
    QWidget *pageWidget1 = new QWidget;
    QVBoxLayout *layout1 = new QVBoxLayout();
    layout1->addWidget(label1);
    pageWidget1->setLayout(layout1);

    QLabel *label2 = new QLabel("Lable2");
    QWidget *pageWidget2 = new QWidget;
    QVBoxLayout *layout2 = new QVBoxLayout();
    layout2->addWidget(label2);
    pageWidget2->setLayout(layout2);

    QStackedLayout *stackedLayout = new QStackedLayout;
    stackedLayout->addWidget(pageWidget1);
    stackedLayout->addWidget(pageWidget2);

    QComboBox *pageComboBox = new QComboBox;
    pageComboBox->addItem(tr("Set Lable1"));
    pageComboBox->addItem(tr("Set Lable2"));
    connect(pageComboBox, SIGNAL(activated(int)),
            stackedLayout, SLOT(setCurrentIndex(int)));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(pageComboBox);
    mainLayout->addLayout(stackedLayout);
    setLayout(mainLayout);
}
