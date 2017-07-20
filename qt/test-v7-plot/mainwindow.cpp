#include "mainwindow.h"

MyWidget::MyWidget(QWidget *parent):
QWidget(parent){
     QCustomPlot *customPlot = new QCustomPlot(this);
     customPlot->resize(320,240);
     QString demoName = "Quadratic Demo";
     QVector<double> x(101), y(101); // initialize with entries 0..100
     for (int i=0; i<101; ++i){
          x[i] = i/50.0 - 1; // x goes from -1 to 1
          y[i] = x[i]*x[i];// let's plot a quadratic function
     }
     // create graph and assign data to it:
     customPlot->addGraph();
     customPlot->graph(0)->setData(x, y);

     QColor color(150,0, 0, 128);
     customPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
     customPlot->graph(0)->setPen(QPen(color.lighter(200)));
     customPlot->graph(0)->setBrush(QBrush(color));

     // give the axes some labels:
     customPlot->xAxis->setLabel("x");
     customPlot->yAxis->setLabel("y");
     // set axes ranges, so we see all data:
     customPlot->xAxis->setRange(-1, 1);
     customPlot->yAxis->setRange(0, 1);

     setWindowTitle("QCustomPlot: "+demoName);
     //statusBar()->clearMessage();

     customPlot->replot();

     QLabel* PI_label = new QLabel(this);
     QImage PI_image = QImage(320,240,QImage::Format_RGB888);
     customPlot->savePng("123.png",320,240,1);
     PI_label->resize(320,240);
     PI_label->setPixmap(QPixmap::fromImage(PI_image));

     QGridLayout *mainLayout = new QGridLayout(this);
     mainLayout->addWidget(customPlot,0,0,1,1);
     mainLayout->addWidget(PI_label,2,0);


}
