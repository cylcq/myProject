///////////////////////////////////////////////////////////////
//
// FileName : k_line.cpp
// Creator : chengyang
// Date : 2014-03-06
// Comment :画一条线
//
///////////////////////////////////////////////////////////////
#include "kline.h"

KLine::KLine(QWidget *parent /* = 0 */)
	:KShape(parent)
{
	m_nShapeType = KShape::Line;
}

KLine::~KLine()
{

}

/**
 * @brief :画一条线
 * @param painter:通过设置painter画出不同风格的图形
 * @return None
*/
void KLine::paint(QPainter &painter)
{
	setShapePainter(painter);///设置painter属性
	painter.drawLine(m_pointTopLeftPoint, m_pointBottomRightPoint);
}

/**
 * @brief :判断鼠标是否在图形里面
 * @param Pos:当前鼠标的坐标值
 * @return 如果鼠标在图形中返回true,否则返回false
*/
bool KLine::judgeMouseInShape(const QPoint &Pos)
{
	///三条边的长度
	float a = calLineLength(m_pointBottomRightPoint,Pos);
	float b = calLineLength(m_pointTopLeftPoint,Pos);
	float c = calLineLength(m_pointTopLeftPoint,m_pointBottomRightPoint);

	int deviationValue = 5;
	///如果两边之和减去第三边在正负deviationValue之间，则返回true
	return (((a + b - c) >= -deviationValue) && ((a + b - c) <= deviationValue));
}

