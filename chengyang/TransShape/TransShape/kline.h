///////////////////////////////////////////////////////////////
//
// FileName : k_line.h
// Creator : chengyang
// Date : 2014-03-06
// Comment :»­Ò»ÌõÏß
//
///////////////////////////////////////////////////////////////
#ifndef __TRANSSHAPE_LINE_H__
#define __TRANSSHAPE_LINE_H__

#include "kshape.h"

class KLine : public KShape
{
public:
	KLine(QWidget *parent = 0);
	~KLine();

protected:
	void	paint(QPainter &painter);
	bool	judgeMouseInShape(const QPoint &Pos);
};

#endif // __TRANSSHAPE_LINE_H__