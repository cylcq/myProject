#ifndef __TRANSFORMSHAPE_KSHAPEVIEW_H__
#define __TRANSFORMSHAPE_KSHAPEVIEW_H__


#include "global.h"
#include "kcontroller.h"
#include "kshapeview.h"
#include "ktransparentwidget.h"
#include "kpaintshape.h"
#include <QMap>
#include <QWidget>
#include <QPainterPath>
#include <QVector>
#include <QRect>

class QMouseEvent;
class KShapeView:public QWidget
{
	Q_OBJECT

public:
	KShapeView(QWidget *parent = 0);
	virtual ~KShapeView();
	//void notify();

public:
	void			setController(KController *controller);
	void			setWidgetUpdate();
	void			unsetWidgetUpdate();
protected:
	void			paintEvent(QPaintEvent *event);
	//void			mousePressEvent(QMouseEvent *event);
	//void			mouseMoveEvent(QMouseEvent *event);
	//void			mouseReleaseEvent(QMouseEvent *event);

private:
	KController				*m_controllerControl;
	KPaintShape				 m_paintShape;
};

#endif // __TRANSFORMSHAPE_KSHAPEVIEW_H__
