#include "kshape.h"
#include <math.h>

KShape::KShape()
	:m_fTopLeftPointX(0)
	,m_fTopLeftPointY(0)
	,m_fBottomRightPointX(0)
	,m_fBottomRightPointY(0)
	,m_fShapeHeight(0)
	,m_fShapeWidth(0)
	,m_nShapeType(SHAPETYPE::None)
	,m_fShapeRotateRate(0)
{

}

KShape::~KShape()
{

}
void KShape::setTopLeftPointX(const float &x)
{
	m_fTopLeftPointX = x;
}
void KShape::setTopLeftPointY(const float &y)
{
	m_fTopLeftPointY = y;
	m_fShapeWidth = abs(m_fTopLeftPointX - m_fBottomRightPointX);
	m_fShapeHeight = abs(m_fTopLeftPointY - m_fBottomRightPointY);
}
void KShape::setBottomRightPointX(const float &x)
{
	m_fBottomRightPointX = x;
}
void KShape::setBottomRightPointY(const float &y)
{
	m_fBottomRightPointY = y;
	m_fShapeWidth = abs(m_fTopLeftPointX - m_fBottomRightPointX);
	m_fShapeHeight = abs(m_fTopLeftPointY - m_fBottomRightPointY);
}
