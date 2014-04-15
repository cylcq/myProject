#include "klistwidgetitem.h"
#include <QListWidget>

KListWidgetItem::KListWidgetItem(QListWidget *parent, int type)
	: QListWidgetItem(parent, type)
	, m_pLanternSlideData(0)
{
	
}

KListWidgetItem::~KListWidgetItem()
{

}

void KListWidgetItem::setLanterSlideData(KLanternSlide*& slideData)
{
	m_pLanternSlideData = slideData;
}

KLanternSlide*& KListWidgetItem::getLanterSlideData()
{
	return m_pLanternSlideData;
}
