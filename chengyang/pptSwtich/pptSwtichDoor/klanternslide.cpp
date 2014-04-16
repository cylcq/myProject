#include "klanternslide.h"

KLanternSlide::KLanternSlide()
	: m_fWidth(0)
	, m_fHeight(0)
	, m_color(Qt::red)
	, m_fDuration(2)
	, m_switchStyle(SWITCHSTYLE_NONE)
{

}

KLanternSlide::~KLanternSlide()
{
}

void KLanternSlide::setWidth(const float& width)
{
	m_fWidth = width;
}

float KLanternSlide::getWidth()
{
	return m_fWidth;
}

void KLanternSlide::setHeight(const float& height)
{
	m_fHeight = height;
}

void KLanternSlide::setColor(const QColor &color)
{
	m_color = color;
}

QColor& KLanternSlide::getColor()
{
	return m_color;
}

void KLanternSlide::setImage(const QImage& image)
{
	m_image = image;
}

QImage& KLanternSlide::getImage()
{
	return m_image;
}

void KLanternSlide::setDuration(const float& duration)
{
	m_fDuration = duration;
}

float KLanternSlide::getDuration()
{
	return m_fDuration;
}

void KLanternSlide::setSwitchStyle(const SWITCHSTYLE& style)
{
	m_switchStyle = style;
}
SWITCHSTYLE KLanternSlide::getSwitchStyle()
{
	return m_switchStyle;
}