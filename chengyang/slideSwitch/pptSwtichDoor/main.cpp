#include "kmainwindow.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	KMainWindow w;
	w.setMinimumSize(1000,500);
	w.show();
	return a.exec();
}
