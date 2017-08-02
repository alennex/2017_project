#include "mainwindow.h"
#include <QMenuBar>
#include <QMessageBox>
#include <QToolBar>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle(tr("Main Window"));
    openAction = new QAction(QIcon("open.png"), tr("開啟檔案"), this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("開啟檔案"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

    QMenu *file = menuBar()->addMenu(tr("&File"));
    file->addAction(openAction);

    QToolBar *toolBar = addToolBar(tr("&File"));
    toolBar->addAction(openAction);

    statusBar() ;
}

void MainWindow::open()
{
    QMessageBox::information(this, tr("消息對話框"), tr("觸發開啟檔案動作"));
}
