#ifndef KJUDGEMOUSEINSHAPE_H
#define KJUDGEMOUSEINSHAPE_H

#include "kmodel.h"
#include <QMap>
#include <QObject>


class QPointF;
class KJudgeMouseInShape : public QObject
{
	Q_OBJECT

public:
	KJudgeMouseInShape(QObject *parent = 0);
	~KJudgeMouseInShape();

	typedef bool (KJudgeMouseInShape::*fJudgeMouseInShape)(ShapeTreeNode *tree, const QPointF &pos);
public:
	void initialJudgeMouseInShapeFunc();
	bool judgeMouseInLine(ShapeTreeNode *tree, const QPointF &pos);
	bool judgeMouseInRect(ShapeTreeNode *tree, const QPointF &pos);
	bool judgeMouseInEllipse(ShapeTreeNode *tree, const QPointF &pos);

public:
	QMap<SHAPETYPE, fJudgeMouseInShape>		m_mapJudgeMouseInShape;
};

#endif // KJUDGEMOUSEINSHAPE_H
