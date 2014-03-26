
///////////////////////////////////////////////////////////////
//
// FileName : k_rect.h
// Creator : chengyang
// Date : 2014-03-06
// Comment :»­Ò»¸ö¾ØÐÎ
//
///////////////////////////////////////////////////////////////

#ifndef __TRANSSHAPE_RECT_H__
#define __TRANSSHAPE_RECT_H__

#include "kshape.h"

class KRect : public KShape
{
	Q_OBJECT

public:
	KRect(QWidget *parent = 0);
	~KRect();

protected:
	void paint(QPainter &painter);
	bool judgeMouseInShape(const QPoint &Pos);
};

#endif // __TRANSSHAPE_RECT_H__
