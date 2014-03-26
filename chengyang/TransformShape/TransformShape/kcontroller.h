#ifndef __TRANSFORMSHAPE_KCONTROLLER_H__
#define __TRANSFORMSHAPE_KCONTROLLER_H__
#include "global.h"
#include "kjudgemouseinshape.h"
#include <QVector>

class KTransparentWidget;
class KShapeView;
class KModel;
class QMouseEvent;
class QPoint;
class QRectF;

typedef QVector<QRectF> QVecRectF;
class KController
{
public:
	KController();
	~KController();

	void	setTransparentView(KTransparentWidget *view);
	void	setShapeView(KShapeView *view);
	void	setModel(KModel *model);
	void	processMousePressEvent(/*const SHAPETYPE &shapetype, */QMouseEvent *event);
	void	processMouseMoveEvent(QMouseEvent *event);
	void	processMouseRelease(QMouseEvent *event);
	bool	judgeMouseInShape(const QPointF &Pos, ShapeTreeNode *&treeNode);
	void	insertShape(const SHAPETYPE &type);
	void    getCacheShapeContainsShape();
	void	createControlPoint(ShapeTreeNode *tree);
	void	shapeMove(QMouseEvent *event);
	void	combineShape();
	void	findMinPoint(QPointF &topLeft, QPointF &bottomRight, const std::vector<ShapeTreeNode*> &vecNode);

public:
	QVector<ShapeTreeNode*>					m_vecCacheShape;
	QVector<ShapeTreeNode*>					m_vecOldCacheShape;
	std::vector<ShapeTreeNode*>				m_vecAllShapes;
	std::map<ShapeTreeNode*, QVecRectF>		m_mapShapeFindControlPoint;

private:
	KModel*					m_pModel;
	KShapeView*				m_pShapeview;
	KTransparentWidget*		m_pTransparentWidget;
	ShapeTreeNode*			m_pShapeTree;
	SHAPETYPE				m_enumCurShapeType;
	bool					m_bDraw;
	bool					m_bCurInSharp;
	bool					m_bCurInMicRect;
	bool					m_bShapeMove;
	bool					m_bSelectArea;
	QPointF					m_pointMousePressPoint;
	MICRORECTADDR			m_enumMicRectAddr;
	KJudgeMouseInShape		m_judgeMouseInShape;
	
};
#endif // __TRANSFORMSHAPE_KCONTROLLER_H__
