///////////////////////////////////////////////////////////////
//
// FileName : k_ellipse.h
// Creator : chengyang
// Date : 2014-03-10
// Comment :ª≠“ª∏ˆÕ÷‘≤
//
///////////////////////////////////////////////////////////////

#ifndef __TRANSSHAPE_ELLIPSE_H__
#define __TRANSSHAPE_ELLIPSE_H__

#include "kshape.h"

class KEllipse : public KShape
{
	Q_OBJECT

public:
	KEllipse(QWidget *parent = 0);
	~KEllipse();

protected:
	void paint(QPainter& painter);
	bool judgeMouseInShape(const QPoint &Pos);
};

#endif // __TRANSSHAPE_ELLIPSE_H__
