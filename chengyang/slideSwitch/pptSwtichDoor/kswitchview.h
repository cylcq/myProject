#ifndef __PPTSWITCHDOOR_KSWITCHVIEW_H__
#define __PPTSWITCHDOOR_KSWITCHVIEW_H__

#include "kswitchstyle.h"
#include <QGLWidget>
#include "kbaseswitch.h"

class KSliderpptList;
class QListWidgetItem;
class QTimer;

class KSwitchView : public QGLWidget
{
	Q_OBJECT

public:
	KSwitchView(QWidget *parent = 0);
	~KSwitchView();

	void setSliderListWidget(KSliderpptList *&slide);
	void stopSwitch();
	void switchSlidePrestige(QPainter& painer, const QImage& preImage, const QImage& curImage, float frame);
	void setBaseSwitch(KBaseSwitch *baseSwitch);

signals:
	void amendDuration(double);

public slots:
	void switchPPTColor(QListWidgetItem * current, QListWidgetItem * previous);
	void switchPPT(SWITCHSTYLE style);

protected:
	void initializeGL();
	void paintEvent(QPaintEvent *);
	//void resizeEvent(QResizeEvent *);
	void resizeGL(int w, int h);

private:
	SWITCHSTYLE m_switchStyle;
	KSliderpptList* m_pSliderListWidget;
	QImage  m_imagePrePPT;
	QImage  m_imageCurPPT;
	QColor  m_preImageColor;
	QColor  m_curImageColor;
	QRectF  m_rectImageArea;
	QTimer* m_timeDuration;
	float   m_fSwitchImageDuration;
	float   m_fTransformTime;		//用于记录在切换幻灯片时已经切换了多长时间
	const float  m_fTimeInterval;
	KBaseSwitch *m_pBaseSwitch;
};

#endif // __PPTSWITCHDOOR_KSWITCHVIEW_H__
