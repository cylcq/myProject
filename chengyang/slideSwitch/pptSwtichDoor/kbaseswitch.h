#ifndef __PPTSWITCHDOOR_KBASESWITCH_H__
#define __PPTSWITCHDOOR_KBASESWITCH_H__

#include <QPainter>
#include <QImage>
class KBaseSwitch
{
public:
	KBaseSwitch();
	virtual ~KBaseSwitch();

public:
	/*virtual void setWidth(const int& w);
	virtual void setHeight(const int& h);
	virtual int	getWidth();
	virtual int getHeight();*/
	virtual void initial(const float& width, const float& height,const QImage& preImage, const QImage& curImage);
	virtual void switchSlide(QPainter& painter, const QImage& preImage, const QImage& curImage, float frame);
};

#endif; //__PPTSWITCHDOOR_KBASESWITCH_H__
