#ifndef KTRANSPARENTWIDGET_H
#define KTRANSPARENTWIDGET_H

#include "global.h"
#include <QWidget>
#include "kpaintshape.h"
class KController;
class KTransparentWidget : public QWidget
{
	Q_OBJECT

public:
	KTransparentWidget(QWidget *parent = 0);
	~KTransparentWidget();
	void			setController(KController *controller);
	void			paintShape(bool bScale = false);
	void			setCursorShape(const QCursor &cursor);
	void			unsetCursorShape();

	public slots:
		void		insertShape(const SHAPETYPE &type);
		void		combineShape();

protected:
	void			paintEvent(QPaintEvent *event);
	void			mousePressEvent(QMouseEvent *event);
	void			mouseMoveEvent(QMouseEvent *event);
	void			mouseReleaseEvent(QMouseEvent *event);

private:
	KController				*m_controllerControl;
	KPaintShape				m_paintShape;
	bool					m_bScale;
};

#endif // KTRANSPARENTWIDGET_H
