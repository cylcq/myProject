#include "kcontroller.h"
#include "kshapeview.h"
#include "ktransparentwidget.h"
#include "kmodel.h"
#include <QMouseEvent>
#include <QPoint>
#include <QRect>
#include <QVector>
#include <QApplication>
#include <QtDebug>
KController::KController()
	: m_enumCurShapeType(SHAPETYPE::Rect)
	, m_bDraw(false)
	, m_bCurInSharp(false)
	, m_bCurInMicRect(false)
	, m_bShapeMove(false)
	, m_bSelectArea(false)
	, m_enumMicRectAddr(MICRORECTADDR::NONE)
{
	m_pModel = NULL;
	m_pShapeview = NULL;
}

KController::~KController()
{

}
void KController::setModel(KModel *model)
{
	m_pModel = KModel::getInstance();
}
void KController::setTransparentView(KTransparentWidget *view)
{ 
	m_pTransparentWidget = view;
}
void KController::setShapeView(KShapeView *view)
{
	m_pShapeview = view;
}
void KController::insertShape(const SHAPETYPE &type)
{
	m_bDraw = true;
	m_enumCurShapeType = type;
	m_pTransparentWidget->setHidden(false);
}
void KController::processMousePressEvent(/*const SHAPETYPE &shapetype, */QMouseEvent *event)
{
	//m_pShapeview->unsetWidgetUpdate();
	ShapeTreeNode *tmpShapeTree = NULL;
	if (judgeMouseInShape(event->pos(),tmpShapeTree))
	{
		m_bCurInSharp = true;
		if (m_mapShapeFindControlPoint.empty())
		{
			createControlPoint(tmpShapeTree);
		}
		else if (QApplication::keyboardModifiers() == Qt::ControlModifier)
		{
			///按下ctrl键可以选中多个图形
			if (event->button() == Qt::LeftButton)
			{
				createControlPoint(tmpShapeTree);
			}
		}
		else
		{
			while(tmpShapeTree->m_pShapeParent)
			{
				tmpShapeTree = tmpShapeTree->m_pShapeParent;
			}
			std::map<ShapeTreeNode*, QVecRectF>::iterator iter = m_mapShapeFindControlPoint.find(tmpShapeTree);
			if (iter == m_mapShapeFindControlPoint.end())
			{
				m_mapShapeFindControlPoint.clear();
				createControlPoint(tmpShapeTree);
			}
		}
		m_pointMousePressPoint = event->posF();
	}
	else
	{
		///清空所有原矩形周围的小矩形
		m_bSelectArea = true;
		m_bCurInSharp = false;
		m_mapShapeFindControlPoint.clear();

		m_pShapeTree = m_pModel->createShapeTreeLeaf(m_enumCurShapeType);

		m_pShapeTree->m_pFirstChild->m_pShape->setTopLeftPointX(event->posF().x());
		m_pShapeTree->m_pFirstChild->m_pShape->setTopLeftPointY(event->posF().y());
		m_pShapeTree->m_pFirstChild->m_pShape->setBottomRightPointX(event->posF().x());
		m_pShapeTree->m_pFirstChild->m_pShape->setBottomRightPointY(event->posF().y());
		m_vecCacheShape.push_back(m_pShapeTree);
	}
}
void KController::processMouseMoveEvent(QMouseEvent *event)
{
	if ((event->buttons() & Qt::LeftButton) && !m_bCurInSharp)
	{
		m_pShapeTree->m_pFirstChild->m_pShape->setBottomRightPointX(event->posF().x());
		m_pShapeTree->m_pFirstChild->m_pShape->setBottomRightPointY(event->posF().y());
	}

	if (!m_bSelectArea)
	{
		shapeMove(event);
	}
	m_pTransparentWidget->paintShape(false);
}
void KController::processMouseRelease(QMouseEvent *event)
{
	m_vecAllShapes.clear();
	m_pModel->getVecShapeLeaf(m_vecAllShapes);

	m_enumCurShapeType = SHAPETYPE::Rect;
	if (m_bDraw)
	{
		m_vecAllShapes.clear();
		m_pModel->addShape(m_pShapeTree);	
		m_pModel->getVecShapeLeaf(m_vecAllShapes);
	}
	else
	{
		if (m_bShapeMove)
		{
			std::map<ShapeTreeNode*, QVecRectF>::const_iterator iter = m_mapShapeFindControlPoint.begin();
			for (; iter != m_mapShapeFindControlPoint.end(); iter++)
			{
				ShapeTreeNode *tmp = iter->first;
				m_pModel->deleteNode(tmp);
			}
			m_mapShapeFindControlPoint.clear();
			foreach (ShapeTreeNode* tree,m_vecCacheShape)
			{
				m_pModel->addShape(tree);
				m_vecAllShapes.clear();
				m_pModel->getVecShapeLeaf(m_vecAllShapes);
				createControlPoint(tree->m_pFirstChild);
			}
		}
		else
		{
			getCacheShapeContainsShape();
			foreach(ShapeTreeNode* treeNode,m_vecCacheShape)
			{
				delete treeNode;
				treeNode = NULL;
			}
			m_bSelectArea = false;
		}
	}
	m_bCurInSharp = false;
	m_bShapeMove = false;
	m_vecCacheShape.clear();
	m_bDraw = false;
	m_pShapeTree = NULL;
	m_vecOldCacheShape.clear();
	m_pTransparentWidget->paintShape(false);
	m_pTransparentWidget->unsetCursorShape();
	//m_pShapeview->setWidgetUpdate();
}
bool KController::judgeMouseInShape(const QPointF &Pos, ShapeTreeNode *&treeNode)
{
	treeNode = NULL;
;	if (!m_vecAllShapes.empty())
	{
		QPointF curMousePoint;
		///从后面循环遍历已存在的图形判断鼠标是否在其里面
		for(int i = m_vecAllShapes.size() - 1; i >= 0; --i)
		{
			curMousePoint = Pos;
			treeNode = m_vecAllShapes.at(i);
			if (treeNode->m_pShape->getShapeRotateRate() >= g_cfNearZero ||
				treeNode->m_pShape->getShapeRotateRate() <= -g_cfNearZero)
			{
				///如果图形旋转过，则进行鼠标坐标map映射，返回鼠标在旋转后的坐标系统上的坐标值，
				///以此坐标值进行判断
				QTransform transform;
				int centerPointX = treeNode->m_pShape->getTopLeftPointX() + 
					(treeNode->m_pShape->getShapeWidth()) / 2;
				int centerPointY = treeNode->m_pShape->getTopLeftPointY() + 
					(treeNode->m_pShape->getShapeHeight()) / 2;
				transform.translate(centerPointX, centerPointY );
				transform.rotate(-(treeNode->m_pShape->getShapeRotateRate()));
				transform.translate(-centerPointX, -centerPointY);
				curMousePoint = transform.map(curMousePoint);
			}
			 if((m_judgeMouseInShape.*(m_judgeMouseInShape.m_mapJudgeMouseInShape)
				 [treeNode->m_pShape->getShapeType()])(treeNode, curMousePoint))
			 {
				 //函数指针，指向不同图形的不同判断方法
				 return true;
			 }
		}
	}
	return false;
}
void KController::getCacheShapeContainsShape()
{
	if (!m_pShapeTree)
	{
		return;
	}
	float tx = m_pShapeTree->m_pFirstChild->m_pShape->getTopLeftPointX();
	float ty = m_pShapeTree->m_pFirstChild->m_pShape->getTopLeftPointY();
	float bx = m_pShapeTree->m_pFirstChild->m_pShape->getBottomRightPointX();
	float by = m_pShapeTree->m_pFirstChild->m_pShape->getBottomRightPointY();
	
	QRectF rectF(QPointF(tx, ty),QPointF(bx, by));

	foreach (ShapeTreeNode *tree, m_vecAllShapes)
	{
		float lx = tree->m_pShape->getTopLeftPointX();
		float ly = tree->m_pShape->getTopLeftPointY();
		float rx = tree->m_pShape->getBottomRightPointX();
		float ry = tree->m_pShape->getBottomRightPointY();

		QRectF rect(QPointF(lx, ly),QPointF(rx, ry));

		if (rectF.contains(rect))
		{
			createControlPoint(tree);
		}
	}
}
void KController::createControlPoint(ShapeTreeNode *tree)
{
	while(tree->m_pShapeParent)
	{
		tree = tree->m_pShapeParent;
	}

	std::map<ShapeTreeNode*, QVecRectF>::iterator iter = m_mapShapeFindControlPoint.find(tree);
	if (iter != m_mapShapeFindControlPoint.end())
	{
		return;
	}

	std::vector<ShapeTreeNode*> vecNode;
	m_pModel->getOneTreeLeaf(tree, vecNode);
	QPointF topLeft, bottomRright;
	findMinPoint(topLeft, bottomRright,vecNode);
	tree->m_pShape->setTopLeftPointX(topLeft.x());
	tree->m_pShape->setTopLeftPointY(topLeft.y());
	tree->m_pShape->setBottomRightPointX(bottomRright.x());
	tree->m_pShape->setBottomRightPointY(bottomRright.y());

	float tx = tree->m_pShape->getTopLeftPointX();
	float ty = tree->m_pShape->getTopLeftPointY();
	float bx = tree->m_pShape->getBottomRightPointX();
	float by = tree->m_pShape->getBottomRightPointY();

	float wide = abs(bx - tx);
	float height = abs(by - ty);
	QRectF rectF(QPointF(tx, ty),QPointF(bx, by));

	QPointF centerPoint(rectF.center());
	QVecRectF controlPointVector;
	int rectWidth = 6;
	int halfWidth = rectWidth / 2;
	///上
	controlPointVector.push_back(QRectF(tx - rectWidth, ty - rectWidth, rectWidth, rectWidth));
	controlPointVector.push_back(QRectF(tx + wide / 2 - halfWidth, ty - rectWidth, rectWidth, rectWidth));
	controlPointVector.push_back(QRectF(tx + wide, ty - rectWidth, rectWidth, rectWidth));
	///中
	controlPointVector.push_back(QRectF(tx - rectWidth, ty + height / 2 - halfWidth, rectWidth, rectWidth));
	controlPointVector.push_back(QRectF(tx + wide, ty + height / 2 - halfWidth, rectWidth, rectWidth));
	///下
	controlPointVector.push_back(QRectF(tx - rectWidth, ty + height, rectWidth, rectWidth));
	controlPointVector.push_back(QRectF(tx + wide / 2 - halfWidth , ty + height, rectWidth , rectWidth));
	controlPointVector.push_back(QRectF(tx + wide, ty + height, rectWidth, rectWidth));

	///旋转点
	///旋转点距离中心点的高度为中心点的纵坐标 - 高/2 - 15;
	controlPointVector.push_back(QRectF(tx + wide / 2 - halfWidth , ty - halfWidth - 15 , rectWidth , rectWidth));

	m_mapShapeFindControlPoint[tree] = controlPointVector;
}
void KController::findMinPoint(QPointF &topLeft, QPointF &bottomRight, const std::vector<ShapeTreeNode*> &vecNode)
{
	std::vector<float> pointX, pointY;
	foreach(ShapeTreeNode* tree, vecNode)
	{
		pointX.push_back(tree->m_pShape->getTopLeftPointX());
		pointX.push_back(tree->m_pShape->getBottomRightPointX());
		pointY.push_back(tree->m_pShape->getTopLeftPointY());
		pointY.push_back(tree->m_pShape->getBottomRightPointY());
	}
	std::sort(pointX.begin(),pointX.end());
	std::sort(pointY.begin(),pointY.end());

	topLeft = QPointF(pointX[0], pointY[0]);
	bottomRight = QPointF(pointX[pointX.size() - 1],pointY[pointY.size() - 1]);
}
void KController::shapeMove(QMouseEvent *event)
{
	ShapeTreeNode *shapeTree = NULL;
	if (judgeMouseInShape(event->pos(), shapeTree) || m_bShapeMove)
	{
		m_pTransparentWidget->setCursorShape(Qt::SizeAllCursor);

		if ((event->buttons() & Qt::LeftButton))
		{
			if (!m_bShapeMove)
			{
				std::map<ShapeTreeNode*, QVecRectF>::const_iterator iter = m_mapShapeFindControlPoint.begin();
				for (; iter != m_mapShapeFindControlPoint.end(); iter++)
				{
					std::vector<ShapeTreeNode*> vecNode;
					m_pModel->getOneTreeLeaf(iter->first, vecNode);

					foreach(ShapeTreeNode *treeNode,vecNode)
					{
						ShapeTreeNode *tree = m_pModel->createShapeTreeLeaf(
							treeNode->m_pShape->getShapeType());

						tree->m_pFirstChild->m_pShape->setBottomRightPointX(
							treeNode->m_pShape->getBottomRightPointX());

						tree->m_pFirstChild->m_pShape->setBottomRightPointY(
							treeNode->m_pShape->getBottomRightPointY());

						tree->m_pFirstChild->m_pShape->setTopLeftPointX(
							treeNode->m_pShape->getTopLeftPointX());

						tree->m_pFirstChild->m_pShape->setTopLeftPointY(
				 			treeNode->m_pShape->getTopLeftPointY());

						float m_fShapeWidth = abs(tree->m_pFirstChild->m_pShape->getTopLeftPointX() - 
							tree->m_pFirstChild->m_pShape->getBottomRightPointX());

						float m_fShapeHeight = abs(tree->m_pFirstChild->m_pShape->getTopLeftPointY() -
							tree->m_pFirstChild->m_pShape->getBottomRightPointY());

						tree->m_pFirstChild->m_pShape->setShapeHeight(
							m_fShapeHeight);

						tree->m_pFirstChild->m_pShape->setShapeWidth(
							m_fShapeWidth);

						tree->m_pFirstChild->m_pShape->setShapeRotateRate(
							treeNode->m_pShape->getShapeRotateRate());
						m_vecOldCacheShape.push_back(treeNode);
						m_vecCacheShape.push_back(tree);
					}
					
				}
			}
			m_bCurInSharp = true;
			m_bShapeMove = true;

		}
		if (m_bShapeMove)
		{
			float offsetX = event->posF().x() - m_pointMousePressPoint.x();
			float offsetY = event->posF().y() - m_pointMousePressPoint.y();
			for(int i = 0; i < m_vecCacheShape.count(); ++i)
			{
				ShapeTreeNode *tree = m_vecCacheShape.at(i);

				tree->m_pFirstChild->m_pShape->setTopLeftPointX(
					m_vecOldCacheShape[i]->m_pShape->getTopLeftPointX() + offsetX);

				tree->m_pFirstChild->m_pShape->setTopLeftPointY(
					m_vecOldCacheShape[i]->m_pShape->getTopLeftPointY() + offsetY);

				tree->m_pFirstChild->m_pShape->setBottomRightPointX(
					tree->m_pFirstChild->m_pShape->getTopLeftPointX() + 
					m_vecOldCacheShape[i]->m_pShape->getShapeWidth());

				tree->m_pFirstChild->m_pShape->setBottomRightPointY(
					tree->m_pFirstChild->m_pShape->getTopLeftPointY() + 
					m_vecOldCacheShape[i]->m_pShape->getShapeHeight());
			}
		}///end of if (m_bShapeMove)
	}///end of if (judgeMouseInShape(event->pos(), &tmpShape))
	else
	{
		m_pTransparentWidget->unsetCursorShape();
	}
}
void KController::combineShape()
{
	std::vector<ShapeTreeNode*> vecNode;
	std::map<ShapeTreeNode*, QVecRectF>::iterator iter = m_mapShapeFindControlPoint.begin();
	for (; iter != m_mapShapeFindControlPoint.end();)
	{
		/*ShapeTreeNode *shapeTree = NULL;
		ShapeTreeNode *parent =  iter->first->m_pShapeParent;
		while (parent)
		{
			shapeTree = parent;
			parent = parent->m_pShapeParent;
		}
*/
		vecNode.push_back(iter->first);
		++iter;
	}
	if (!vecNode.empty())
	{
		m_pModel->combineNode(vecNode);
	}
	m_mapShapeFindControlPoint.clear();
}