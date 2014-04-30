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

	m_pSwitchDoor = new QAction("Door" ,m_pToolBar);
	m_pToolBar->addAction(m_pSwitchDoor);;
	connect(m_pSwitchDoor, SIGNAL(triggered()), m_pCentralWidget, SLOT(slot_SwitchDoor()));

	m_pSwitchPrestige = new QAction("Prestige", m_pToolBar);
	m_pToolBar->addAction(m_pSwitchPrestige);
	connect(m_pSwitchPrestige,SIGNAL(triggered()),m_pCentralWidget,SLOT(slot_SwitchPrestige()));

	m_pSwitchCurtain = new QAction("Curtain", m_pToolBar);
	m_pToolBar->addAction(m_pSwitchCurtain);
	connect(m_pSwitchCurtain,SIGNAL(triggered()),m_pCentralWidget,SLOT(slot_SwitchCurtain()));
}

KMainWindow::~KMainWindow()
{

}