#ifndef __TRANSFORMSHAPE_KMAINWINDOW_H__
#define __TRANSFORMSHAPE_KMAINWINDOW_H__

#include "global.h"
#include <QtGui>

class KController;
class KShapeView;
class KModel;
class KTransparentWidget;
class KMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	KMainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~KMainWindow();
	void	createActions();

signals:
	void	changeCurrentShape(const SHAPETYPE &type);

	private slots:
		void	drawLineActionTriggered();
		void	drawRectActionTriggered();
		void	drawElliseActionTriggered();

private:
	KShapeView*			m_pShapeView;
	KTransparentWidget*	m_pTransparentWidget;
	KController*		m_pController;
	KModel*				m_pModel;
	QToolBar*			m_barTool;
	QLabel*				m_labelStatusMsg;
	QActionGroup*		m_actionGroup;
	QAction*			m_pCombine;
	QAction*			m_pDrawLineAction;
	QAction*			m_pdrawRectAction;
	QAction*			m_pDrawEllipseAction;
};

#endif // __TRANSFORMSHAPE_KMAINWINDOW_H__
