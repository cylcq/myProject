///////////////////////////////////////////////////////////////
//
// FileName : k_ellipse.cpp
// Creator : chengyang
// Date : 2014-03-10
// Comment :画一个椭圆
//
///////////////////////////////////////////////////////////////

#include "kellipse.h"

KEllipse::KEllipse(QWidget *parent /* = 0 */)
	: KShape(parent)
{
	m_nShapeType = KShape::Ellipse;
}

KEllipse::~KEllipse()
{

}
void KEllipse::paint(QPainter &painter)
{
	setShapePainter(painter);
	QRect rect(m_pointTopLeftPoint, m_pointBottomRightPoint);
	painter.drawEllipse(rect);
}
bool KEllipse::judgeMouseInShape(const QPoint &Pos)
{
	QRect rect(m_pointTopLeftPoint, m_pointBottomRightPoint);
	QPainterPath path; 
	path.addEllipse(rect);     //向path中添加图形
	return path.contains(Pos);
}