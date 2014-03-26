///////////////////////////////////////////////////////////////
//
// FileName : main.cpp
// Creator : chengyang
// Date : 2014-03-06
// Comment :Ö÷º¯Êý
//
///////////////////////////////////////////////////////////////
#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	MainWindow w;
	w.setWindowTitle(QObject::tr("Paint Application"));
	w.resize(800, 600);
	w.show();
	return app.exec();
}