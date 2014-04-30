#ifndef __PPTSWITCHDOOR_KCENTRALWIDGET_H__
#define __PPTSWITCHDOOR_KCENTRALWIDGET_H__

#include "kswitchstyle.h"
#include "kbaseswitch.h"
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
		void slot_SwitchDoor();
		void slot_SwitchPrestige();
		void slot_SwitchCurtain();

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
	KBaseSwitch *m_pBaseSwitch;
	
};

#endif // __PPTSWITCHDOOR_KCENTRALWIDGET_H__
