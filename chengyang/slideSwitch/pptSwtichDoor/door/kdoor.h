#ifndef __PPT_SWITCHDOOR_KDOOR_H__
#define __PPT_SWITCHDOOR_KDOOR_H__

#include "kbaseswitch.h"

class KDoor:public KBaseSwitch
{
public:
	KDoor(void);
	virtual ~KDoor(void);

public:
	void drawCurImageEffect(QPainter &painter, const QImage &curImage, const float& frame);
	void drawPreImageEffect(QPainter &painter,const  QImage &preImage, const float& frame);
	void switchSlide(QPainter& painter, const QImage& preImage, const QImage& curImage, float frame);

private:
	const float  m_fImageMeetScale;	//两个Image在矩形的该比例出相遇
};
#endif; //__PPT_SWITCHDOOR_KDOOR_H__
