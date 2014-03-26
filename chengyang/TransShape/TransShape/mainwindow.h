///////////////////////////////////////////////////////////////
//
// FileName : mainwindow.h
// Creator : chengyang
// Date : 2014-03-06
// Comment :�����ڣ����ڷ��ø��ֲ���
//
///////////////////////////////////////////////////////////////
#ifndef __TRANSSHAPE_MAINWINDOW_H
#define __TRANSSHAPE_MAINWINDOW_H

#include <QtGui>

#include "kshape.h"
#include "kpaintwidget.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

	void	createActions();

signals:
	void	changeCurrentShape(KShape::ShapeType newShape);

	private slots:
		void	drawLineActionTriggered();
		void	drawRectActionTriggered();
		void	drawElliseActionTriggered();

private:
	KPaintWidget	*m_pPaintWidget;			///���Ĳ���
	QToolBar		*m_barTool;					///������
	QLabel			*m_labelStatusMsg;			///״̬��
	QActionGroup	*m_actionGroup;				///ActionȺ��
	QAction			*m_pCombine;				///���
	QAction			*m_pDrawLineAction;			///����
	QAction			*m_pdrawRectAction;			///������
	QAction			*m_pDrawEllipseAction;		///����Բ
};

#endif // __TRANSSHAPE_MAINWINDOW_H