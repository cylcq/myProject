#ifndef __PPTSWITCHDOOR_KMAINWINDOW_H__
#define __PPTSWITCHDOOR_KMAINWINDOW_H__

#include <QtGui/QMainWindow>

class KCentralWidget;
class QAction;
class QToolBar;
class KMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	KMainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~KMainWindow();

private:
	KCentralWidget*	m_pCentralWidget;
	QAction *m_pSwitchDoor;
	QToolBar *m_pToolBar;
};

#endif // __PPTSWITCHDOOR_KMAINWINDOW_H__
