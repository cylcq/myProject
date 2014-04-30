#include "kcentralwidget.h"
#include "ksliderpptlist.h"
#include "kswitchview.h"
#include "door/kdoor.h"
#include "prestige/kprestige.h"
#include "curtain/kcurtain.h"
#include <QColor>
#include <QColorDialog>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPainter>
#include <QPaintEvent>
KCentralWidget::KCentralWidget(QWidget *parent)
	: QWidget(parent)
	, m_nMainWindowWide(0)
	, m_nMainWindowHeight(0)
	, m_pLabelDuration(0)
	, m_pListSliderWidget(0)
	, m_pSpinBoxDuration(0)
	, m_pBaseSwitch(0)
{
	
	QPalette palette;
	palette.setBrush(this->backgroundRole(),QBrush(QColor(255, 255, 255)));
	this->setPalette(palette);
	this->setAutoFillBackground(true);
	initWindow();
}

KCentralWidget::~KCentralWidget()
{
	
}

void KCentralWidget::initWindow()
{
	m_pListSliderWidget = new KSliderpptList(this);

	m_pSpinBoxDuration = new QDoubleSpinBox(this);
	m_pSpinBoxDuration->setMaximumHeight(40);
	m_pSpinBoxDuration->setMaximum(59.00);
	m_pSpinBoxDuration->setMinimum(0.1);
	connect(m_pSpinBoxDuration, SIGNAL(valueChanged(double)), m_pListSliderWidget, SLOT(amendItemValue(double)));
	
	m_pLabelDuration = new QLabel(tr("Duration:"),this);
	m_pLabelDuration->setMaximumHeight(40);
	m_pLabelDuration->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	m_pLabelDuration->setBuddy(m_pSpinBoxDuration);

	m_pSwitchWidget = new KSwitchView(this);
	m_pSwitchWidget->setSliderListWidget(m_pListSliderWidget);

	connect(m_pSwitchWidget, SIGNAL(amendDuration(double)), m_pSpinBoxDuration, SLOT(setValue(double)));
	connect(this, SIGNAL(switchPPT(SWITCHSTYLE)), m_pSwitchWidget, SLOT(switchPPT(SWITCHSTYLE)));
	connect(m_pListSliderWidget,SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
		m_pSwitchWidget, SLOT(switchPPTColor(QListWidgetItem*, QListWidgetItem*)));

}

void KCentralWidget::getMainWindowSizeInfo()
{
	m_nMainWindowWide = this->rect().width();
	m_nMainWindowHeight = this->rect().height();
}

void KCentralWidget::setWindowWidgetSize()
{
	m_pLabelDuration->setGeometry(m_nMainWindowWide * 0.6, m_nMainWindowHeight * 0.02,
		m_nMainWindowWide * 0.2, m_nMainWindowHeight * 0.07);

	m_pSpinBoxDuration->setGeometry(m_nMainWindowWide * 0.8, m_nMainWindowHeight * 0.02,
		m_nMainWindowWide * 0.19, m_nMainWindowHeight * 0.07);

	m_pListSliderWidget->setGeometry(m_nMainWindowWide * 0.02, m_nMainWindowHeight * 0.09,
		m_nMainWindowWide * 0.27, m_nMainWindowHeight * 0.9);

	m_pSwitchWidget->setGeometry(m_nMainWindowWide * 0.27, m_nMainWindowHeight * 0.09,
		m_nMainWindowWide * 0.72, m_nMainWindowHeight * 0.9);
}

void KCentralWidget::resizeEvent(QResizeEvent *event)
{
	getMainWindowSizeInfo();
	setWindowWidgetSize();
}
void KCentralWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
}

void KCentralWidget::slot_SwitchDoor()
{
	m_pBaseSwitch = new KDoor();
	m_pSwitchWidget->setBaseSwitch(m_pBaseSwitch);
	emit switchPPT(SWITCHSTYLE_DOOR);
}
void KCentralWidget::slot_SwitchPrestige()
{
	m_pBaseSwitch = new KPrestige();
	m_pSwitchWidget->setBaseSwitch(m_pBaseSwitch);
	emit switchPPT(SWITCHSTYLE_PRESTIGE);
}
void KCentralWidget::slot_SwitchCurtain()
{
	m_pBaseSwitch = new KCurtain();
	m_pSwitchWidget->setBaseSwitch(m_pBaseSwitch);
	emit switchPPT(SWITCHSTYLE_CURTAIN);
}