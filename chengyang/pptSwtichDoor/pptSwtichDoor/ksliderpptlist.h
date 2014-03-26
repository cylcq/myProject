#ifndef __PPTSWITCHDOOR_KSLIDERPPTLIST_H__
#define __PPTSWITCHDOOR_KSLIDERPPTLIST_H__

#include <QListWidget>

class KListWidgetItem;
class QImage;
class Action;
class KSliderpptList : public QListWidget
{
	Q_OBJECT

public:
	KSliderpptList(QWidget *parent);
	~KSliderpptList();
	
	void createContextMenu();
	void addItemAndSelected();

public slots:
	void  addItemData(const QColor& color = Qt::red);
	void  amendItemValue(double value);
	void changeBackColor();

protected:
	//void paintEvent(QPaintEvent *e);
private:
	QImage m_iconImage;
	QColor				m_itemBackColor;
	KListWidgetItem*	m_pWidgetItem;
	
	QAction*			m_pActionCopyPPT;
	QAction*			m_pActionChangColor;
};

#endif // __PPTSWITCHDOOR_KSLIDERPPTLIST_H__
