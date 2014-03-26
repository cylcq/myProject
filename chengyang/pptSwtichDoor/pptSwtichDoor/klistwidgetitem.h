#ifndef __PPTSWITCHDOOR_KLISTWIDGETITEM_H__
#define __PPTSWITCHDOOR_KLISTWIDGETITEM_H__

#include <QListWidgetItem>

class KLanternSlide;
class QListWidget;
class KListWidgetItem : public QListWidgetItem
{
	//Q_OBJECT

public:
	KListWidgetItem(QListWidget *parent, int type);
	~KListWidgetItem();

	void setLanterSlideData(KLanternSlide*& slideData);
	KLanternSlide*& getLanterSlideData();
private:
	KLanternSlide*	m_pLanternSlideData;
};

#endif // __PPTSWITCHDOOR_KLISTWIDGETITEM_H__
