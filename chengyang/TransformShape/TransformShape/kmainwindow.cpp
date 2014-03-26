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

	/// ���ù�����
	m_barTool = this->addToolBar("Tools");
	m_actionGroup = new QActionGroup(m_barTool);

	//����Action
	createActions();

	/// ����״̬��
	m_labelStatusMsg = new QLabel(this);
	statusBar()->addWidget(m_labelStatusMsg);

	
	//�������Ĳ����������Ĳ˵�
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
 * @brief ������������Ҫ��Action
 * @return None
*/
void KMainWindow::createActions()
{
	m_pCombine = new QAction(tr("Combine"),this);
	m_pCombine->setToolTip(tr("Combine Shape"));
	m_pCombine->setStatusTip(tr("Combine Shape"));
	connect(m_pCombine,SIGNAL(triggered()),m_pTransparentWidget,SLOT(combineShape()));
	/// ����Line����
	m_pDrawLineAction = new QAction("Line",m_barTool);
	m_pDrawLineAction->setToolTip(tr("Draw a line."));
	m_pDrawLineAction->setStatusTip(tr("Draw a line."));

	m_actionGroup->addAction(m_pDrawLineAction);
	m_barTool->addAction(m_pDrawLineAction);

	/// ����Rect����
	m_pdrawRectAction = new QAction("Rectangle",m_barTool);
	m_pdrawRectAction->setToolTip(tr("Draw a rectangle."));
	m_pdrawRectAction->setStatusTip(tr("Draw a rectangle."));

	m_actionGroup->addAction(m_pdrawRectAction);
	m_barTool->addAction(m_pdrawRectAction);

	///����Ellipse����

	m_pDrawEllipseAction = new QAction("Ellipse",m_barTool);
	m_pDrawEllipseAction->setToolTip(tr("Draw a ellipse."));
	m_pDrawEllipseAction->setStatusTip(tr("Draw a ellipse."));

	m_actionGroup->addAction(m_pDrawEllipseAction);
	m_barTool->addAction(m_pDrawEllipseAction);

	/// �ź�����
	connect(m_pDrawLineAction, SIGNAL(triggered()), this, SLOT(drawLineActionTriggered()));
	connect(m_pdrawRectAction, SIGNAL(triggered()), this, SLOT(drawRectActionTriggered()));
	connect(m_pDrawEllipseAction, SIGNAL(triggered()), this, SLOT(drawElliseActionTriggered()));
}

/**
 * @brief :�����źţ�����Ҫ�����ͼ������
 * @return None
*/
void KMainWindow::drawLineActionTriggered()
{
	emit changeCurrentShape(SHAPETYPE::Line);   // �趨��״Ϊ���Σ������ź�
}

void KMainWindow::drawRectActionTriggered()
{
	emit changeCurrentShape(SHAPETYPE::Rect);   // �趨��״ΪRectangle���Σ������ź�
}

void KMainWindow::drawElliseActionTriggered()
{
	emit changeCurrentShape(SHAPETYPE::Ellipse);	//�趨�����ͼ��ΪEllipse
}
