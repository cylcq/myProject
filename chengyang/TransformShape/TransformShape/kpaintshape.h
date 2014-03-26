#ifndef KPAINTSHAPE_H
#define KPAINTSHAPE_H

#include "global.h"
#include <QObject>
#include <QMap>

class QPointF;
class QPainter;
class KPaintShape : public QObject
{
	Q_OBJECT

public:
	KPaintShape(QObject *parent = 0);
	~KPaintShape();

	typedef void (KPaintShape::*fPaintShape)(QPainter &painter, const QPointF &startPoint, 
		const QPointF &endPoint, float rotateAngle, bool bScale);

public:
	QPointF calShapeCentralPoint(const QPointF &startPoint, const QPointF &endPoint);
	void setShapePainter(QPainter &painter, QPointF &centralPoint, float angle, bool bScale = false);
	void initialPaintShapeFunc();
	void paintLine(QPainter &painter, const QPointF &startPoint, 
		const QPointF &endPoint, float rotateAngle, bool bScale);
	void paintRect(QPainter &painter, const QPointF &startPoint, 
		const QPointF &endPoint, float rotateAngle, bool bScale);
	void paintEllipse(QPainter &painter, const QPointF &startPoint, 
		const QPointF &endPoint, float rotateAngle, bool bScale);

public:
	QMap<SHAPETYPE, fPaintShape>		m_mapPaintShape;
};

#endif // KPAINTSHAPE_H
