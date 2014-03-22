#ifndef KCENTRALWIDGET_H
#define KCENTRALWIDGET_H

#include "kswitchstyle.h"
#include <QWidget>

class QLabel;
class QDoubleSpinBox;
class KSliderpptList;
class KSwitchView;

class KCentralWidget : public QWidget
{
	Q_OBJECT

public:
	KCentralWidget(QWidget *parent = 0);
	~KCentralWidget();

	void initWindow();
	void setWindowWidgetSize();
	void getMainWindowSizeInfo();

signals:
	void switchPPT(SWITCHSTYLE style);

	public slots:
		void switchDoor();

protected:
	void resizeEvent(QResizeEvent *event);
	void paintEvent(QPaintEvent *event);

private:
	int	m_nMainWindowWide;
	int	m_nMainWindowHeight;
	QLabel*	m_pLabelDuration; 
	KSliderpptList*	m_pListSliderWidget;
	QDoubleSpinBox* m_pSpinBoxDuration;
	KSwitchView* m_pSwitchWidget;
	
};

#endif // KCENTRALWIDGET_H
