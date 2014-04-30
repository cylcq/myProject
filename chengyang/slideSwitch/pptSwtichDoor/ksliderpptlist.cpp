#include "ksliderpptlist.h"
#include "klanternslide.h"
#include "klistwidgetitem.h"
#include <QImage>
#include <QAction>
#include <QListWidgetItem>
#include <QPainter>
#include <QColorDialog>
KSliderpptList::KSliderpptList(QWidget *parent)
	: QListWidget(parent)
{
	
	this->setStyleSheet("background-color:rgb(176, 196, 222)");

	this->setFixedWidth(960 * 0.2);
	addItemAndSelected();
	createContextMenu();
}

KSliderpptList::~KSliderpptList()
{
	delete m_pActionCopyPPT;
	KListWidgetItem* item = 0;
	for (int i = 0; i < this->count(); ++i)
	{
		 item = (KListWidgetItem*)this->item(i);
		 delete item->getLanterSlideData();
		 item =NULL;
	}
}

void KSliderpptList::addItemAndSelected()
{
	addItemData();
	m_pWidgetItem->setSelected(true);
}

void KSliderpptList::createContextMenu()
{
	m_pActionCopyPPT = new QAction(tr("New"), this);
	connect(m_pActionCopyPPT, SIGNAL(triggered()), this ,SLOT(addItemData()));
	this->addAction(m_pActionCopyPPT);

	m_pActionChangColor = new  QAction(tr("Color"), this);
	connect(m_pActionChangColor, SIGNAL(triggered()), this, SLOT(changeBackColor()));
	this->addAction(m_pActionChangColor);

	this->setContextMenuPolicy(Qt::ActionsContextMenu);
}
void  KSliderpptList::addItemData(const QColor& color)
{
	m_iconImage = QImage(QSize(this->width() * 0.92,this->width() *0.7), QImage::Format_ARGB32_Premultiplied);
	m_iconImage.fill(color);

	m_pWidgetItem = new KListWidgetItem(this, QListWidgetItem::UserType);
	m_pWidgetItem->setSizeHint(QSize(this->width() * 0.92,this->width() *0.7));
	m_pWidgetItem->setBackgroundColor(color);
	
	KLanternSlide* m_pLanternSlideData = new KLanternSlide;
	m_pLanternSlideData->setColor(color);
	m_pLanternSlideData->setImage(m_iconImage);
	m_pWidgetItem->setLanterSlideData(m_pLanternSlideData);
}

void KSliderpptList::amendItemValue(double value)
{
	KListWidgetItem *curItem = (KListWidgetItem*)(this->currentItem());
	curItem->getLanterSlideData()->setDuration(value);
}

void KSliderpptList::changeBackColor()
{
	m_itemBackColor =  QColorDialog::getColor(Qt::white, this);
	if (m_itemBackColor.isValid())
	{
		this->currentItem()->setBackground(m_itemBackColor);
		KListWidgetItem* item = (KListWidgetItem*)this->currentItem();
		item->getLanterSlideData()->setColor(m_itemBackColor);

		emit currentItemChanged(this->currentItem(), 0);
	}
}