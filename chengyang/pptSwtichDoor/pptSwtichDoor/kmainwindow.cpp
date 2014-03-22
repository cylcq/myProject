#include "kmainwindow.h"
#include "kcentralwidget.h"
#include <QToolBar>
#include <QAction>
KMainWindow::KMainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
	, m_pSwitchDoor(0)
{
	m_pCentralWidget = new KCentralWidget(this);
	setCentralWidget(m_pCentralWidget);
	m_pToolBar = this->addToolBar("Tools");
	m_pSwitchDoor = new QAction("Door",m_pToolBar);
	m_pToolBar->addAction(m_pSwitchDoor);;
	connect(m_pSwitchDoor, SIGNAL(triggered()), m_pCentralWidget, SLOT(switchDoor()));
}

KMainWindow::~KMainWindow()
{

}