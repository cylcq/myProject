#ifndef __PPTSWITCHDOOR_KSWITCHVIEW_H__
#define __PPTSWITCHDOOR_KSWITCHVIEW_H__

#include "kswitchstyle.h"
#include <QWidget>

class KSliderpptList;
class QListWidgetItem;
class QTimer;
class KSwitchView : public QWidget
{
	Q_OBJECT

public:
	KSwitchView(QWidget *parent = 0);
	~KSwitchView();

	void setSliderListWidget(KSliderpptList *&slide);
	void stopSwitch();
	void drawCurImageEffect(QPainter &painter, const QImage &curImage, const float& frame);
	void drawPreImageEffect(QPainter &painter,const  QImage &preImage, const float& frame);
	void switchSlideDoor(QPainter& painer, const QImage& preImage, const QImage& curImage, float frame);

signals:
	void amendDuration(double);

public slots:
	void switchPPTColor(QListWidgetItem * current, QListWidgetItem * previous);
	void switchPPT(SWITCHSTYLE style);

protected:
	void paintEvent(QPaintEvent *);
	void resizeEvent(QResizeEvent *);

private:
	KSliderpptList* m_pSliderListWidget;
	QImage  m_imagePrePPT;
	QImage  m_imageCurPPT;
	QColor  m_preImageColor;
	QColor  m_curImageColor;
	QRectF  m_rectImageArea;
	QTimer* m_timeDuration;
	float   m_fSwitchImageDuration;
	float   m_fTransformTime;		//���ڼ�¼���л��õ�Ƭʱ�Ѿ��л��˶೤ʱ��
	const float  m_fTimeInterval;
	const float  m_fImageMeetScale;	//����Image�ھ��εĸñ���������
};

#endif // __PPTSWITCHDOOR_KSWITCHVIEW_H__
