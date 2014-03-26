#include "ktransparentwidget.h"
#include "kcontroller.h"
#include <QPainter>
#include <QRectF>
#include <map>
KTransparentWidget::KTransparentWidget(QWidget *parent)
	: QWidget(parent)
{
	setMouseTracking(true);
	setMaximumSize(800,600);
	setMinimumSize(800,600);

	setAttribute(Qt::WA_TranslucentBackground, true);
	setWindowOpacity(0);
}

KTransparentWidget::~KTransparentWidget()
{

}
void KTransparentWidget::insertShape(const SHAPETYPE &type)
{
	setCursor(Qt::CrossCursor);
	m_controllerControl->insertShape(type);
}
void KTransparentWidget::setController(KController *controller)
{
	m_controllerControl = controller;
}
void KTransparentWidget::setCursorShape(const QCursor &cursor)
{
	setCursor(cursor);
}
void KTransparentWidget::unsetCursorShape()
{
	unsetCursor();
}

void KTransparentWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap));
	painter.setRenderHint(QPainter::Antialiasing,true);
	
	int vecCounts = m_controllerControl->m_vecCacheShape.count();
	for (int i = 0; i < vecCounts; ++i)
	{
		painter.save();

		painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap));
		painter.setBrush(QBrush(QColor(238, 233, 233, 120)));

		float tx = m_controllerControl->m_vecCacheShape[i]->m_pFirstChild->m_pShape->getTopLeftPointX();
		float ty = m_controllerControl->m_vecCacheShape[i]->m_pFirstChild->m_pShape->getTopLeftPointY();
		float bx = m_controllerControl->m_vecCacheShape[i]->m_pFirstChild->m_pShape->getBottomRightPointX();
		float by = m_controllerControl->m_vecCacheShape[i]->m_pFirstChild->m_pShape->getBottomRightPointY();

		float wide = abs(bx - tx);
		float height = abs(by - ty);
		float rotateAngle =m_controllerControl->m_vecCacheShape[i]->m_pFirstChild->m_pShape->getShapeRotateRate();
		SHAPETYPE shapeType = m_controllerControl->m_vecCacheShape[i]->m_pFirstChild->m_pShape->getShapeType();

		(m_paintShape.*(m_paintShape.m_mapPaintShape)[shapeType])(painter,QPointF(tx, ty),
			QPointF(bx, by),rotateAngle,m_bScale);
		painter.restore();
	}

	ShapeTreeNode *tree = NULL;
	std::map<ShapeTreeNode*, QVecRectF>::const_iterator iter = m_controllerControl->m_mapShapeFindControlPoint.begin();
	for (; iter != m_controllerControl->m_mapShapeFindControlPoint.end(); iter++)
	{
		painter.save();
		float tx = (iter->first)->m_pShape->getTopLeftPointX();
		float ty = (iter->first)->m_pShape->getTopLeftPointY();
		float bx = (iter->first)->m_pShape->getBottomRightPointX();
		float by = (iter->first)->m_pShape->getBottomRightPointY();

		float rotateAngle =(iter->first)->m_pShape->getShapeRotateRate();
		QPointF central = QRectF(QPointF(tx, ty),QPointF(bx, by)).center();
		m_paintShape.setShapePainter(painter, central, rotateAngle, false);
		painter.drawRects(iter->second); 
		painter.restore();
	}

}

void KTransparentWidget::paintShape(bool bScale)
{
	m_bScale = bScale;
	this->update();
}

void KTransparentWidget::mousePressEvent(QMouseEvent *event)
{
	m_controllerControl->processMousePressEvent(event);
}
void KTransparentWidget::mouseMoveEvent(QMouseEvent *event)
{
	m_controllerControl->processMouseMoveEvent(event);
}
void KTransparentWidget::mouseReleaseEvent(QMouseEvent *event)
{
	m_controllerControl->processMouseRelease(event);
}
void KTransparentWidget::combineShape()
{
	m_controllerControl->combineShape();
}

