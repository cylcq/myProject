#ifndef KSWITCHVIEW_H
#define KSWITCHVIEW_H

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
	void drawCurImageEffect(QPainter &painter, QImage &curImage);
	void drawPreImageEffect(QPainter &painter, QImage &preImage);

	void setTransparent(QImage &image, int alpha);

	void calTimeOutOffset(const float &imageMeetScale,const float &duration, 
		const float &width, const float &height);

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
	float   m_fIncreasedWidth;
	float   m_fIncreasedHeight;
	float   m_fIncreasedAngle;
	float	m_fAlphaDecrease;
	float	m_fCurImageWidth; 
	float   m_fCurImageHeight;
	float   m_fRotateAngle;
	float	m_fAlpha;
	const float  m_fTimeInterval;
	const float  m_fImageMeetScale;	//两个Image在矩形的一定比例出相遇
};

#endif // KSWITCHVIEW_H
