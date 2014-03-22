#ifndef KSLIDERPPTLIST_H
#define KSLIDERPPTLIST_H

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

#endif // KSLIDERPPTLIST_H
