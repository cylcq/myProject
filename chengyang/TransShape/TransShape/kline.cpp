///////////////////////////////////////////////////////////////
//
// FileName : k_line.cpp
// Creator : chengyang
// Date : 2014-03-06
// Comment :��һ����
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
 * @brief :��һ����
 * @param painter:ͨ������painter������ͬ����ͼ��
 * @return None
*/
void KLine::paint(QPainter &painter)
{
	setShapePainter(painter);///����painter����
	painter.drawLine(m_pointTopLeftPoint, m_pointBottomRightPoint);
}

/**
 * @brief :�ж�����Ƿ���ͼ������
 * @param Pos:��ǰ��������ֵ
 * @return ��������ͼ���з���true,���򷵻�false
*/
bool KLine::judgeMouseInShape(const QPoint &Pos)
{
	///�����ߵĳ���
	float a = calLineLength(m_pointBottomRightPoint,Pos);
	float b = calLineLength(m_pointTopLeftPoint,Pos);
	float c = calLineLength(m_pointTopLeftPoint,m_pointBottomRightPoint);

	int deviationValue = 5;
	///�������֮�ͼ�ȥ������������deviationValue֮�䣬�򷵻�true
	return (((a + b - c) >= -deviationValue) && ((a + b - c) <= deviationValue));
}

