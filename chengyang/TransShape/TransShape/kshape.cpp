///////////////////////////////////////////////////////////////
//
// FileName : k_shape.h
// Creator : chengyang
// Date : 2014-03-06
// Comment :所有图形的基类
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
 * @brief :设置图形的左上点的坐标，同时根据该坐标改变其他点的坐标
 * @param Pos:鼠标当前坐标
 * @return None
*/
void KShape::setTopLeftPoint(const QPoint &point)
{
	m_pointTopLeftPoint = point;

	//设置图形的宽和高
	m_nShapeWidth = abs(m_pointTopLeftPoint.x() - m_pointBottomRightPoint.x());
	m_nShapeHeight = abs(m_pointTopLeftPoint.x() - m_pointBottomRightPoint.y());

	//设置图形的中心点
	m_pointShapeCenterPoint = QPoint((m_pointTopLeftPoint.x() +m_pointBottomRightPoint.x()) / 2,
		(m_pointTopLeftPoint.y() + m_pointBottomRightPoint.y()) / 2);

	//设置图形的旋转点
	//旋转点距离中心点的高度为中心点的纵坐标 - 高/2 - 15;
	m_pointShapeRotatePoint = QPoint(m_pointShapeCenterPoint.x(), 
		m_pointShapeCenterPoint.y() - m_nShapeHeight / 2 - 15);

	m_pointBottomLeftPoint = QPoint(m_pointTopLeftPoint.x(), m_pointTopLeftPoint.y() + m_nShapeHeight);
	m_pointTopRightPoint = QPoint(m_pointTopLeftPoint.x() + m_nShapeWidth, m_pointTopLeftPoint.y());
}

/**
 * @brief :设置图形的右下点的坐标，同时根据该坐标改变其他点的坐标
 * @param Pos:鼠标当前坐标
 * @return None
*/
void KShape::setBottomRightPoint(const QPoint &point)
{
	m_pointBottomRightPoint = point;
	//设置图形的宽和高
	m_nShapeWidth = abs(m_pointTopLeftPoint.x() - m_pointBottomRightPoint.x());
	m_nShapeHeight = abs(m_pointTopLeftPoint.y() - m_pointBottomRightPoint.y());
	//设置图形的中心点
	m_pointShapeCenterPoint = QPoint((m_pointTopLeftPoint.x() +m_pointBottomRightPoint.x()) / 2,
		(m_pointTopLeftPoint.y() + m_pointBottomRightPoint.y()) / 2);
	//设置图形的旋转点
	//旋转点距离中心点的高度为中心点的纵坐标 - 高/2 - 15;
	m_pointShapeRotatePoint = QPoint(m_pointShapeCenterPoint.x(), 
		m_pointShapeCenterPoint.y() - m_nShapeHeight / 2 - 15);

	m_pointBottomLeftPoint = QPoint(m_pointBottomRightPoint.x() - m_nShapeWidth, m_pointBottomRightPoint.y());
	m_pointTopRightPoint = QPoint(m_pointBottomRightPoint.x(), m_pointBottomRightPoint.y() - m_nShapeHeight);

}

/**
 * @brief :设置图形的右上点的坐标，同时根据该坐标改变其他点的坐标
 * @param Pos:鼠标当前坐标
 * @return None
*/
void KShape::setTopRightPoint(const QPoint &point)
{
	m_pointTopRightPoint = point;

	//设置图形的宽和高
	m_nShapeWidth = abs(m_pointTopRightPoint.x() - m_pointBottomLeftPoint.x());
	m_nShapeHeight = abs(m_pointTopRightPoint.y() - m_pointBottomLeftPoint.y());

	m_pointTopLeftPoint = QPoint(m_pointTopRightPoint.x() - m_nShapeWidth, m_pointTopRightPoint.y());
	m_pointBottomRightPoint = QPoint(m_pointTopRightPoint.x(), m_pointTopRightPoint.y() + m_nShapeHeight);
}

/**
 * @brief :设置图形的左下点的坐标，同时根据左下点的坐标改变其他点的坐标
 * @param Pos:鼠标当前坐标
 * @return None
*/
void KShape::setBottomLeftPoint(const QPoint &point)
{
	m_pointBottomLeftPoint = point;

	//设置图形的宽和高
	m_nShapeWidth = abs(m_pointTopRightPoint.x() - m_pointBottomLeftPoint.x());
	m_nShapeHeight = abs(m_pointTopRightPoint.y() - m_pointBottomLeftPoint.y());

	m_pointTopLeftPoint = QPoint(m_pointBottomLeftPoint.x(), m_pointBottomLeftPoint.y() - m_nShapeHeight);
	m_pointBottomRightPoint = QPoint(m_pointBottomLeftPoint.x() + m_nShapeWidth, m_pointBottomLeftPoint.y());
}

void KShape::paint(QPainter &painter)
{

}

/**
 * @brief :判断鼠标是否在图形里面
 * @param Pos:当前鼠标的坐标值
 * @return 如果鼠标在图形中返回true,否则返回false
*/
bool KShape::judgeMouseInShape(const QPoint &Pos)
{
	return false;
}

/**
 * @brief : 设置QPainter的属性
 * @param : painter: QPainter对象
 * @return None
*/
void KShape::setShapePainter(QPainter &painter)
{
	if (m_fShapeRotateRate >= g_cfNearZero || m_fShapeRotateRate <= -g_cfNearZero)
	{
		//图形旋转的角度不等于0，将图形旋转
		if (this->m_bShapeBeScaled)
		{
			//如果图形正在被拉伸，则移动坐标原点到原图形的中心点
			painter.translate(m_pointOldShapeCenterPoint);
		}
		else
		{
			painter.translate(m_pointShapeCenterPoint);
		}
		//旋转
		painter.rotate(m_fShapeRotateRate);

		//将坐标原点移到之前的坐标原点处
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
 * @brief :计算两个点的距离
 * @param Pa:点
 * &param Pb:点
 * @return 返回两点之间的距离
*/
float KShape::calLineLength(const QPoint &pa,const QPoint &pb)
{
	QLineF line(pa,pb);
	return line.length();
}