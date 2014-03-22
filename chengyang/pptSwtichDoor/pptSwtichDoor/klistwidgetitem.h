#ifndef KLISTWIDGETITEM_H
#define KLISTWIDGETITEM_H

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

#endif // KLISTWIDGETITEM_H
