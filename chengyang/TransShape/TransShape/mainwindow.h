///////////////////////////////////////////////////////////////
//
// FileName : mainwindow.h
// Creator : chengyang
// Date : 2014-03-06
// Comment :主窗口，用于放置各种部件
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
	KPaintWidget	*m_pPaintWidget;			///中心部件
	QToolBar		*m_barTool;					///工具栏
	QLabel			*m_labelStatusMsg;			///状态栏
	QActionGroup	*m_actionGroup;				///Action群组
	QAction			*m_pCombine;				///组合
	QAction			*m_pDrawLineAction;			///画线
	QAction			*m_pdrawRectAction;			///画矩形
	QAction			*m_pDrawEllipseAction;		///画椭圆
};

#endif // __TRANSSHAPE_MAINWINDOW_H