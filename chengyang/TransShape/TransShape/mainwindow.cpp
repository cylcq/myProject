///////////////////////////////////////////////////////////////
//
// FileName : mainwindow.cpp
// Creator : chengyang
// Date : 2014-03-06
// Comment :主窗口，用于放置各种部件
//
///////////////////////////////////////////////////////////////
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	m_pPaintWidget = NULL;
	m_barTool = NULL;
	m_labelStatusMsg = NULL;
	m_actionGroup = NULL;
	m_pCombine = NULL;
	m_pDrawLineAction = NULL;
	m_pdrawRectAction = NULL;
	m_pDrawEllipseAction = NULL;

	/// 设置工具栏
	m_barTool = this->addToolBar("Tools");
	m_actionGroup = new QActionGroup(m_barTool);

	//创建Action
	createActions();

	/// 设置状态栏
	m_labelStatusMsg = new QLabel(this);
	statusBar()->addWidget(m_labelStatusMsg);

	/// 设置中心部件
	m_pPaintWidget = new KPaintWidget(this);
	setCentralWidget(m_pPaintWidget);
	//设置中心部件的上下文菜单
	m_pPaintWidget->addAction(m_pCombine);
	m_pPaintWidget->setContextMenuPolicy(Qt::ActionsContextMenu);

	connect(this, SIGNAL(changeCurrentShape(KShape::ShapeType)), m_pPaintWidget, SLOT(setCurrentShape(KShape::ShapeType)));
}
MainWindow::~MainWindow()
{

}
/**
 * @brief 创建窗体所需要的Action
 * @return None
*/
void MainWindow::createActions()
{
	m_pCombine = new QAction(tr("Combine"),this);
	m_pCombine->setToolTip(tr("Combine Shape"));
	m_pCombine->setStatusTip(tr("Combine Shape"));
	connect(m_pCombine,SIGNAL(triggered()),m_pPaintWidget,SLOT(combineShape()));
	/// 设置Line动作
	m_pDrawLineAction = new QAction("Line",m_barTool);
	m_pDrawLineAction->setToolTip(tr("Draw a line."));
	m_pDrawLineAction->setStatusTip(tr("Draw a line."));

	m_actionGroup->addAction(m_pDrawLineAction);
	m_barTool->addAction(m_pDrawLineAction);

	/// 设置Rect动作
	m_pdrawRectAction = new QAction("Rectangle",m_barTool);
	m_pdrawRectAction->setToolTip(tr("Draw a rectangle."));
	m_pdrawRectAction->setStatusTip(tr("Draw a rectangle."));

	m_actionGroup->addAction(m_pdrawRectAction);
	m_barTool->addAction(m_pdrawRectAction);

	///设置Ellipse动作

	m_pDrawEllipseAction = new QAction("Ellipse",m_barTool);
	m_pDrawEllipseAction->setToolTip(tr("Draw a ellipse."));
	m_pDrawEllipseAction->setStatusTip(tr("Draw a ellipse."));

	m_actionGroup->addAction(m_pDrawEllipseAction);
	m_barTool->addAction(m_pDrawEllipseAction);

	/// 信号连接
	connect(m_pDrawLineAction, SIGNAL(triggered()), this, SLOT(drawLineActionTriggered()));
	connect(m_pdrawRectAction, SIGNAL(triggered()), this, SLOT(drawRectActionTriggered()));
	connect(m_pDrawEllipseAction, SIGNAL(triggered()), this, SLOT(drawElliseActionTriggered()));
}
/**
 * @brief :发射信号，设置要插入的图形类型
 * @return None
*/
void MainWindow::drawLineActionTriggered()
{
	emit changeCurrentShape(KShape::Line);   // 设定形状为线形，发射信号
}

void MainWindow::drawRectActionTriggered()
{
	emit changeCurrentShape(KShape::Rect);   // 设定形状为Rectangle矩形，发射信号
}
void MainWindow::drawElliseActionTriggered()
{
	emit changeCurrentShape(KShape::Ellipse);	//设定插入的图形为Ellipse
}
