/* PushButton */

QApplication app(argc, argv);

QPushButton *btn = new QPushButton("Close");
btn->setWindowTitle("Signal & Slot");
btn->setFont("Time", 18, Bold);
btn->resize(250,50);

QObject::connect(btn, SIGNAL(clicked()). &app, SLOT(on_pushButton_clicked));
btn->show();

return app.exec();

/* file browser */
void Widget::on_pushButton_clicked(){
	QString FileName:
	FileName = QFileDialog::getOpenFileName(this, "Open my file");
	if(!FileName.isEmpty()){
		ui->lineEdit->setText(FileName);
	}
}