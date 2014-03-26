#include "kmainwindow.h"
#include "kcontroller.h"
#include "kshapeview.h"
#include "ktransparentwidget.h"
#include "kmodel.h"

KMainWindow::KMainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	m_pShapeView = NULL;
	m_pTransparentWidget = NULL;
	m_pModel = NULL;
	m_pController = NULL;
	m_barTool = NULL;
	m_labelStatusMsg = NULL;
	m_actionGroup = NULL;
	m_pCombine = NULL;
	m_pDrawLineAction = NULL;
	m_pdrawRectAction = NULL;
	m_pDrawEllipseAction = NULL;

	m_pController = new KController();
	m_pModel = KModel::getInstance();
	m_pShapeView = new KShapeView(this);
	m_pTransparentWidget = new KTransparentWidget(m_pShapeView);

	m_pShapeView->setController(m_pController);
	m_pTransparentWidget->setController(m_pController);
	m_pController->setModel(m_pModel);
	m_pController->setTransparentView(m_pTransparentWidget);
	m_pController->setShapeView(m_pShapeView);

	setCentralWidget(m_pShapeView);

	/// 设置工具栏
	m_barTool = this->addToolBar("Tools");
	m_actionGroup = new QActionGroup(m_barTool);

	//创建Action
	createActions();

	/// 设置状态栏
	m_labelStatusMsg = new QLabel(this);
	statusBar()->addWidget(m_labelStatusMsg);

	
	//设置中心部件的上下文菜单
	m_pShapeView->addAction(m_pCombine);
	m_pShapeView->setContextMenuPolicy(Qt::ActionsContextMenu);

	connect(this, SIGNAL(changeCurrentShape(const SHAPETYPE&)), m_pTransparentWidget, SLOT(insertShape(const SHAPETYPE&)));
}

KMainWindow::~KMainWindow()
{
	delete m_pShapeView;
	delete m_pModel;
	delete m_pController;
}

/**
 * @brief 创建窗体所需要的Action
 * @return None
*/
void KMainWindow::createActions()
{
	m_pCombine = new QAction(tr("Combine"),this);
	m_pCombine->setToolTip(tr("Combine Shape"));
	m_pCombine->setStatusTip(tr("Combine Shape"));
	connect(m_pCombine,SIGNAL(triggered()),m_pTransparentWidget,SLOT(combineShape()));
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
void KMainWindow::drawLineActionTriggered()
{
	emit changeCurrentShape(SHAPETYPE::Line);   // 设定形状为线形，发射信号
}

void KMainWindow::drawRectActionTriggered()
{
	emit changeCurrentShape(SHAPETYPE::Rect);   // 设定形状为Rectangle矩形，发射信号
}

void KMainWindow::drawElliseActionTriggered()
{
	emit changeCurrentShape(SHAPETYPE::Ellipse);	//设定插入的图形为Ellipse
}
