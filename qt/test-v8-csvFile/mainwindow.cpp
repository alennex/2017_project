#include "mainwindow.h"

MyWidget::MyWidget(QWidget *parent):
QWidget(parent){
    
    // ========= File Open

     QGroupBox* FileOpen = new QGroupBox("Step 1: File Open", this);
     QVBoxLayout* FO_layout = new QVBoxLayout(this);
    
     QPushButton* FO_bu = new QPushButton("open CSV file",this);
     FO_line =  new QLineEdit(this);

     QPushButton* SA_bu = new QPushButton("save CSV file",this);

     connect(FO_bu, SIGNAL(clicked()), this,SLOT(showfile()));
     connect(SA_bu, SIGNAL(clicked()), this, SLOT(writefile()));

     FO_layout->addWidget(FO_bu);
     FO_layout->addWidget(FO_line);
     FileOpen->setLayout(FO_layout);

     QGridLayout *mainLayout = new QGridLayout(this);
     mainLayout->addWidget(FileOpen,0,0);

}

void MyWidget::showfile(){
     
     QFileDialog myFileDialog(this);
     QString fileName = myFileDialog.getOpenFileName(this, "Open File",
     QDir::currentPath (), "CSV Files (*.csv *.xls)");

     if (fileName.isEmpty ()) {
          FO_line->setText (myFileDialog.directory().absolutePath());
     } else {
          FO_line->setText (fileName);
          readfile(fileName);
     }
}


void MyWidget::readfile(QString _fileName){


     QFile file(_fileName);
     
     if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
          qDebug() << "Cannot open file for reading: " << qPrintable(file.errorString());
          exit(0);
     }

     QStringList list;
     
     list.clear();
     QTextStream in(&file);

     while(!in.atEnd()){
          QString fileLine = in.readLine();
          list = fileLine.split(",", QString::SkipEmptyParts);
          inFile.push_back(list);
       
//          if(list.at(0).contains("322",Qt::CaseSensitive)){
//             qDebug() << list;    
//          }

     }

      printf("row = %d, col = %d\n", inFile.size(), inFile[0].size());
     file.close();

     for(int i=0; i<inFile.size(); ++i)
          qDebug() << inFile[i];
}

void MyWidget::writefile(){
     QFileDialog myFileDialog(this);

      QString _fileName = "outFile.csv";//myFileDialog.getSaveFileName(this, tr("Excel file"), qApp->applicationDirPath(),tr("Files (*.csv)"));
      if(_fileName.isEmpty())
          _fileName.append(myFileDialog.directory().absolutePath());

     QFile file(_fileName);
      if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
          qDebug() << "Cannot open file for writing: "<< qPrintable(file.errorString());
          exit(0);
       }
     
     QTextStream out(&file); 

     for(int i=0; i<inFile.size(); ++i){
          outFile.push_back(inFile[i]);
          outFile[i].append("hello");
     }   

     for(int i=0; i<outFile.size(); ++i){
          for(int j=0; j<outFile[0].size(); ++j){
               out<<outFile[i].at(j)<<",";
          }
          out<<"\n";
     }
     file.close();
}




