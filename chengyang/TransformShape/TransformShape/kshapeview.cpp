#include "kshapeview.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtDebug>

KShapeView::KShapeView(QWidget *parent)
	: QWidget(parent)
{
	m_controllerControl = NULL;
	
}

KShapeView::~KShapeView()
{
	
}


void KShapeView::setController(KController *controller)
{
	m_controllerControl = controller;
}

void  KShapeView::setWidgetUpdate()
{
	setUpdatesEnabled(true);
	this->update();
}

void KShapeView::unsetWidgetUpdate()
{
	setUpdatesEnabled(false);
}
void KShapeView::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap));
	painter.setBrush(Qt::SolidPattern);
	painter.setRenderHint(QPainter::Antialiasing,true);
	painter.fillRect(QRect(0,0,800,600),QBrush(Qt::white));
	int vecCounts = m_controllerControl->m_vecAllShapes.size();
	for (int i = 0; i < vecCounts; ++i)
	{
		painter.save();
		painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap));
		painter.setBrush(QBrush(QColor(188, 210, 238)));

		float tx = m_controllerControl->m_vecAllShapes[i]->m_pShape->getTopLeftPointX();
		float ty = m_controllerControl->m_vecAllShapes[i]->m_pShape->getTopLeftPointY();
		float bx = m_controllerControl->m_vecAllShapes[i]->m_pShape->getBottomRightPointX();
		float by = m_controllerControl->m_vecAllShapes[i]->m_pShape->getBottomRightPointY();

		float wide = abs(bx - tx);
		float height = abs(by - ty);
		float rotateAngle =m_controllerControl->m_vecAllShapes[i]->m_pShape->getShapeRotateRate();
		SHAPETYPE shapeType = m_controllerControl->m_vecAllShapes[i]->m_pShape->getShapeType();

		(m_paintShape.*(m_paintShape.m_mapPaintShape)[shapeType])(painter,QPointF(tx, ty),
			QPointF(bx, by),rotateAngle,false);
		painter.restore();
	}
}
//void KShapeView::mousePressEvent(QMouseEvent *event)
//{
//	//m_controllerControl->processMousePressEvent(event);
//}
//void KShapeView::mouseMoveEvent(QMouseEvent *event)
//{
//
//}
//void KShapeView::mouseReleaseEvent(QMouseEvent *event)
//{
//	
//}
//void KShapeView::combineShape()
//{
//
//}
