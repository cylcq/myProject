#include "kpaintshape.h"
#include <QPointF>
#include <QPainter>
KPaintShape::KPaintShape(QObject *parent)
	: QObject(parent)
{
	initialPaintShapeFunc();
}

KPaintShape::~KPaintShape()
{

}

QPointF KPaintShape::calShapeCentralPoint(const QPointF &startPoint, const QPointF &endPoint)
{
	return QRectF(startPoint,endPoint).center();
}

void KPaintShape::setShapePainter(QPainter &painter, QPointF &centralPoint, float angle, bool bScale)
{
	if (angle >= g_cfNearZero || angle <= -g_cfNearZero)
	{
		if (bScale)
		{
			//painter.translate(m_pointOldShapeCenterPoint);
		}
		else
		{
			painter.translate(centralPoint);
		}
		
		painter.rotate(angle);

		if (bScale)
		{
			//painter.translate(-m_pointOldShapeCenterPoint);
		}
		else
		{
			painter.translate(-centralPoint);
		}
	}
}

void KPaintShape::initialPaintShapeFunc()
{
	m_mapPaintShape[SHAPETYPE::Line] = &KPaintShape::paintLine;
	m_mapPaintShape[SHAPETYPE::Rect] = &KPaintShape::paintRect;
	m_mapPaintShape[SHAPETYPE::Ellipse] = &KPaintShape::paintEllipse;
}

void KPaintShape::paintLine(QPainter &painter, const QPointF &startPoint,
	const QPointF &endPoint, float rotateAngle, bool bScale)
{
	QPointF central = calShapeCentralPoint(startPoint,endPoint);
	setShapePainter(painter, central, rotateAngle, bScale); 
}

void KPaintShape::paintRect(QPainter &painter, const QPointF &startPoint,
	const QPointF &endPoint, float rotateAngle, bool bScale)
{
	QPointF central = calShapeCentralPoint(startPoint,endPoint);
	setShapePainter(painter, central, rotateAngle, bScale); 
	painter.drawRect(QRectF(startPoint,endPoint));
}

void KPaintShape::paintEllipse(QPainter &painter, const QPointF &startPoint, 
	const QPointF &endPoint, float rotateAngle, bool bScale)
{
	QPointF central = calShapeCentralPoint(startPoint,endPoint);
	setShapePainter(painter, central, rotateAngle, bScale); 
	QRectF rectF(startPoint, endPoint);
	painter.drawEllipse(rectF);
}
