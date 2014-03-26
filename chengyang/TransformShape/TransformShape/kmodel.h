///////////////////////////////////////////////////////////////
//
// FileName : kmodel.h
// Creator : chengyang
// Date : 2014-03-13 22:17
// Comment :用于访问数据，将数据返回给控制层
//
///////////////////////////////////////////////////////////////
#ifndef __TRANSFORMSHAPE_KMODEL_H__
#define __TRANSFORMSHAPE_KMODEL_H__

#include "kshape.h"
#include "imodellistener.h"
#include <iostream>
#include <vector>

typedef struct SHAPE_TREE_NODE 
{
	KShape				*m_pShape;
	SHAPE_TREE_NODE		*m_pShapeParent;
	SHAPE_TREE_NODE		*m_pFirstChild;
	SHAPE_TREE_NODE		*m_pNextSibling;
	SHAPE_TREE_NODE		*m_pPreSibling;

	SHAPE_TREE_NODE()
	{
		m_pShape = NULL;
		m_pShapeParent = NULL;
		m_pFirstChild = NULL;
		m_pNextSibling = NULL;
		m_pPreSibling = NULL;
	}
	~SHAPE_TREE_NODE()
	{
		delete m_pShape;
		m_pShape = NULL;
	}
}ShapeTreeNode;

class KModel
{
public:
	virtual ~KModel();

public:
	static	KModel*				getInstance();
	void						addModelListener(IModelListener*);
	void						combineNode(std::vector<ShapeTreeNode*> &vecNode);
	void						CancelCombineNode(ShapeTreeNode *shapeTree);
	void						removeNode(ShapeTreeNode *&shapeTree);
	void						deleteNode(ShapeTreeNode *shapeTree);
	void						removeTree(ShapeTreeNode *shapeTree);
	ShapeTreeNode*				createShapeTreeParent();
	ShapeTreeNode*				createShapeTreeLeaf(SHAPETYPE shapetype);
	void						addShape(ShapeTreeNode *&shapeTree);
	void						notifyView();

	ShapeTreeNode*				getShapeTree(){return m_pShapeTree;}
	void						getVecShapeLeaf(std::vector<ShapeTreeNode*> &vecNode);
	void						getOneTreeLeaf(ShapeTreeNode *treeNode, std::vector<ShapeTreeNode*> &vecNode);
	void						getShapeParent(ShapeTreeNode *&treeNode);
	void						copyTree(ShapeTreeNode *original, ShapeTreeNode *&copy);
	void						copyNode(ShapeTreeNode);
private:
	KModel();

private:
	static	KModel*					m_modelInstance;
	ShapeTreeNode*					m_pShapeTree;
	std::vector<ShapeTreeNode*>		m_vectorShapeTree;
	std::vector<IModelListener*>	m_vectorModelListener;
};

#endif // __TRANSFORMSHAPE_KMODEL_H__
