///////////////////////////////////////////////////////////////
//
// FileName : k_shape.h
// Creator : chengyang
// Date : 2014-03-06
// Comment :����ͼ�εĻ���
//
///////////////////////////////////////////////////////////////
#include "kshape.h"

KShape::KShape(QWidget *parent)
	:QWidget(parent)
	,m_nShapeHeight(0)
	,m_nShapeWidth(0)
	,m_nShapeType(KShape::None)
	,m_fShapeRotateRate(0)
	,m_bShapeBeScaled(false)
{

}

KShape::~KShape()
{

}

/**
 * @brief :����ͼ�ε����ϵ�����꣬ͬʱ���ݸ�����ı������������
 * @param Pos:��굱ǰ����
 * @return None
*/
void KShape::setTopLeftPoint(const QPoint &point)
{
	m_pointTopLeftPoint = point;

	//����ͼ�εĿ�͸�
	m_nShapeWidth = abs(m_pointTopLeftPoint.x() - m_pointBottomRightPoint.x());
	m_nShapeHeight = abs(m_pointTopLeftPoint.x() - m_pointBottomRightPoint.y());

	//����ͼ�ε����ĵ�
	m_pointShapeCenterPoint = QPoint((m_pointTopLeftPoint.x() +m_pointBottomRightPoint.x()) / 2,
		(m_pointTopLeftPoint.y() + m_pointBottomRightPoint.y()) / 2);

	//����ͼ�ε���ת��
	//��ת��������ĵ�ĸ߶�Ϊ���ĵ�������� - ��/2 - 15;
	m_pointShapeRotatePoint = QPoint(m_pointShapeCenterPoint.x(), 
		m_pointShapeCenterPoint.y() - m_nShapeHeight / 2 - 15);

	m_pointBottomLeftPoint = QPoint(m_pointTopLeftPoint.x(), m_pointTopLeftPoint.y() + m_nShapeHeight);
	m_pointTopRightPoint = QPoint(m_pointTopLeftPoint.x() + m_nShapeWidth, m_pointTopLeftPoint.y());
}

/**
 * @brief :����ͼ�ε����µ�����꣬ͬʱ���ݸ�����ı������������
 * @param Pos:��굱ǰ����
 * @return None
*/
void KShape::setBottomRightPoint(const QPoint &point)
{
	m_pointBottomRightPoint = point;
	//����ͼ�εĿ�͸�
	m_nShapeWidth = abs(m_pointTopLeftPoint.x() - m_pointBottomRightPoint.x());
	m_nShapeHeight = abs(m_pointTopLeftPoint.y() - m_pointBottomRightPoint.y());
	//����ͼ�ε����ĵ�
	m_pointShapeCenterPoint = QPoint((m_pointTopLeftPoint.x() +m_pointBottomRightPoint.x()) / 2,
		(m_pointTopLeftPoint.y() + m_pointBottomRightPoint.y()) / 2);
	//����ͼ�ε���ת��
	//��ת��������ĵ�ĸ߶�Ϊ���ĵ�������� - ��/2 - 15;
	m_pointShapeRotatePoint = QPoint(m_pointShapeCenterPoint.x(), 
		m_pointShapeCenterPoint.y() - m_nShapeHeight / 2 - 15);

	m_pointBottomLeftPoint = QPoint(m_pointBottomRightPoint.x() - m_nShapeWidth, m_pointBottomRightPoint.y());
	m_pointTopRightPoint = QPoint(m_pointBottomRightPoint.x(), m_pointBottomRightPoint.y() - m_nShapeHeight);

}

/**
 * @brief :����ͼ�ε����ϵ�����꣬ͬʱ���ݸ�����ı������������
 * @param Pos:��굱ǰ����
 * @return None
*/
void KShape::setTopRightPoint(const QPoint &point)
{
	m_pointTopRightPoint = point;

	//����ͼ�εĿ�͸�
	m_nShapeWidth = abs(m_pointTopRightPoint.x() - m_pointBottomLeftPoint.x());
	m_nShapeHeight = abs(m_pointTopRightPoint.y() - m_pointBottomLeftPoint.y());

	m_pointTopLeftPoint = QPoint(m_pointTopRightPoint.x() - m_nShapeWidth, m_pointTopRightPoint.y());
	m_pointBottomRightPoint = QPoint(m_pointTopRightPoint.x(), m_pointTopRightPoint.y() + m_nShapeHeight);
}

/**
 * @brief :����ͼ�ε����µ�����꣬ͬʱ�������µ������ı������������
 * @param Pos:��굱ǰ����
 * @return None
*/
void KShape::setBottomLeftPoint(const QPoint &point)
{
	m_pointBottomLeftPoint = point;

	//����ͼ�εĿ�͸�
	m_nShapeWidth = abs(m_pointTopRightPoint.x() - m_pointBottomLeftPoint.x());
	m_nShapeHeight = abs(m_pointTopRightPoint.y() - m_pointBottomLeftPoint.y());

	m_pointTopLeftPoint = QPoint(m_pointBottomLeftPoint.x(), m_pointBottomLeftPoint.y() - m_nShapeHeight);
	m_pointBottomRightPoint = QPoint(m_pointBottomLeftPoint.x() + m_nShapeWidth, m_pointBottomLeftPoint.y());
}

void KShape::paint(QPainter &painter)
{

}

/**
 * @brief :�ж�����Ƿ���ͼ������
 * @param Pos:��ǰ��������ֵ
 * @return ��������ͼ���з���true,���򷵻�false
*/
bool KShape::judgeMouseInShape(const QPoint &Pos)
{
	return false;
}

/**
 * @brief : ����QPainter������
 * @param : painter: QPainter����
 * @return None
*/
void KShape::setShapePainter(QPainter &painter)
{
	if (m_fShapeRotateRate >= g_cfNearZero || m_fShapeRotateRate <= -g_cfNearZero)
	{
		//ͼ����ת�ĽǶȲ�����0����ͼ����ת
		if (this->m_bShapeBeScaled)
		{
			//���ͼ�����ڱ����죬���ƶ�����ԭ�㵽ԭͼ�ε����ĵ�
			painter.translate(m_pointOldShapeCenterPoint);
		}
		else
		{
			painter.translate(m_pointShapeCenterPoint);
		}
		//��ת
		painter.rotate(m_fShapeRotateRate);

		//������ԭ���Ƶ�֮ǰ������ԭ�㴦
		if (this->m_bShapeBeScaled)
		{
			painter.translate(-m_pointOldShapeCenterPoint);
		}
		else
		{
			painter.translate(-m_pointShapeCenterPoint);
		}
	}
}

/**
 * @brief :����������ľ���
 * @param Pa:��
 * &param Pb:��
 * @return ��������֮��ľ���
*/
float KShape::calLineLength(const QPoint &pa,const QPoint &pb)
{
	QLineF line(pa,pb);
	return line.length();
}