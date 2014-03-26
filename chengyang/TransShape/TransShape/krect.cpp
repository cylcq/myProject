///////////////////////////////////////////////////////////////
//
// FileName : k_rect.cpp
// Creator : chengyang
// Date : 2014-03-06
// Comment :画一个矩形
//
///////////////////////////////////////////////////////////////
#include "krect.h"

KRect::KRect(QWidget *parent)
	:KShape(parent)
{
	m_nShapeType = KShape::Rect;
}
KRect::~KRect()
{

}
/**
 * @brief :画一个矩形
 * @param painter:通过设置painter画出不同风格的图形
 * @return None
*/
void KRect::paint(QPainter &painter)
{
	setShapePainter(painter);///设置painter属性
	painter.drawRect(m_pointTopLeftPoint.x(), m_pointTopLeftPoint.y(),
		m_pointBottomRightPoint.x() - m_pointTopLeftPoint.x(), m_pointBottomRightPoint.y() - m_pointTopLeftPoint.y());
}

/**
 * @brief 用于判断鼠标是否在一个图形中
 * @param Pos:鼠标当前的位置
 * @return bool:如果鼠标在图形区则返回true，否则返回false
*/
bool KRect::judgeMouseInShape(const QPoint &Pos)
{
	QRect rect(this->getTopLeftPoint(), this->getBottomRightPoint());
	return (rect.contains(Pos, false));
}