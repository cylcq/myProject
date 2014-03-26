///////////////////////////////////////////////////////////////
//
// FileName : mainwindow.cpp
// Creator : chengyang
// Date : 2014-03-06
// Comment :�����ڣ����ڷ��ø��ֲ���
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

	/// ���ù�����
	m_barTool = this->addToolBar("Tools");
	m_actionGroup = new QActionGroup(m_barTool);

	//����Action
	createActions();

	/// ����״̬��
	m_labelStatusMsg = new QLabel(this);
	statusBar()->addWidget(m_labelStatusMsg);

	/// �������Ĳ���
	m_pPaintWidget = new KPaintWidget(this);
	setCentralWidget(m_pPaintWidget);
	//�������Ĳ����������Ĳ˵�
	m_pPaintWidget->addAction(m_pCombine);
	m_pPaintWidget->setContextMenuPolicy(Qt::ActionsContextMenu);

	connect(this, SIGNAL(changeCurrentShape(KShape::ShapeType)), m_pPaintWidget, SLOT(setCurrentShape(KShape::ShapeType)));
}
MainWindow::~MainWindow()
{

}
/**
 * @brief ������������Ҫ��Action
 * @return None
*/
void MainWindow::createActions()
{
	m_pCombine = new QAction(tr("Combine"),this);
	m_pCombine->setToolTip(tr("Combine Shape"));
	m_pCombine->setStatusTip(tr("Combine Shape"));
	connect(m_pCombine,SIGNAL(triggered()),m_pPaintWidget,SLOT(combineShape()));
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
void MainWindow::drawLineActionTriggered()
{
	emit changeCurrentShape(KShape::Line);   // �趨��״Ϊ���Σ������ź�
}

void MainWindow::drawRectActionTriggered()
{
	emit changeCurrentShape(KShape::Rect);   // �趨��״ΪRectangle���Σ������ź�
}
void MainWindow::drawElliseActionTriggered()
{
	emit changeCurrentShape(KShape::Ellipse);	//�趨�����ͼ��ΪEllipse
}
