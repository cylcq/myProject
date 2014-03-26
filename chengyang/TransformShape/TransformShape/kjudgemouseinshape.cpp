#include "kjudgemouseinshape.h"
#include <QRectF>
#include <QPainterPath>
KJudgeMouseInShape::KJudgeMouseInShape(QObject *parent)
	: QObject(parent)
{
	initialJudgeMouseInShapeFunc();
}

KJudgeMouseInShape::~KJudgeMouseInShape()
{

}

bool KJudgeMouseInShape::judgeMouseInLine(ShapeTreeNode *tree, const QPointF &pos)
{
	return false;
}
bool KJudgeMouseInShape::judgeMouseInRect(ShapeTreeNode *tree, const QPointF &pos)
{
	float tx = tree->m_pShape->getTopLeftPointX();
	float ty = tree->m_pShape->getTopLeftPointY();
	float bx = tree->m_pShape->getBottomRightPointX();
	float by = tree->m_pShape->getBottomRightPointY();

	QRectF rectF(QPointF(tx, ty),QPointF(bx, by));
	return (rectF.contains(pos));
}
bool KJudgeMouseInShape::judgeMouseInEllipse(ShapeTreeNode *tree, const QPointF &pos)
{
	float tx = tree->m_pShape->getTopLeftPointX();
	float ty = tree->m_pShape->getTopLeftPointY();
	float bx = tree->m_pShape->getBottomRightPointX();
	float by = tree->m_pShape->getBottomRightPointY();

	QRectF rectF(QPointF(tx, ty),QPointF(bx, by));
	QPainterPath path; 
	path.addEllipse(rectF);     //向path中添加图形
	return path.contains(pos);
}
void KJudgeMouseInShape::initialJudgeMouseInShapeFunc()
{
	m_mapJudgeMouseInShape[SHAPETYPE::Line] = &KJudgeMouseInShape::judgeMouseInLine;
	m_mapJudgeMouseInShape[SHAPETYPE::Rect] = &KJudgeMouseInShape::judgeMouseInRect;
	m_mapJudgeMouseInShape[SHAPETYPE::Ellipse] = &KJudgeMouseInShape::judgeMouseInEllipse;
}
