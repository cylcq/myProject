#ifndef __PPTSWITCHDOOR_KLANTERNSLIDER_H__
#define __PPTSWITCHDOOR_KLANTERNSLIDER_H__

#include "kswitchstyle.h"
#include <QImage>
#include <QColor>

class KLanternSlide
{
public:
	KLanternSlide();
	~KLanternSlide();

public:
	void setWidth(const float& width);
	float getWidth();
	void setHeight(const float& height);
	float getHeight();
	void setColor(const QColor &color);
	QColor& getColor();
	void setImage(const QImage& image);
	QImage& getImage();
	void setDuration(const float& duration);
	float getDuration();
	void setSwitchStyle(const SWITCHSTYLE &style);
	SWITCHSTYLE getSwitchStyle();
private:
	float m_fWidth;
	float m_fHeight;
	QColor m_color;
	QImage m_image;
	float m_fDuration;
	SWITCHSTYLE m_switchStyle;
	
};

#endif //__PPTSWITCHDOOR_KLANTERNSLIDER_H__
