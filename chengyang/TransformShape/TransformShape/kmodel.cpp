#include "kmodel.h"

#include <QSharedPointer>

KModel *KModel::m_modelInstance = NULL;

KModel::KModel()
{
	m_pShapeTree = NULL;
}

KModel::~KModel()
{
	if (!m_vectorShapeTree.empty())
	{
		//ÊÍ·ÅÄÚ´æ
		foreach (ShapeTreeNode *tree, m_vectorShapeTree)
		{
			deleteNode(tree);
		}
		m_vectorShapeTree.clear();
	}
}

KModel *KModel::getInstance()
{
	if (!m_modelInstance)
	{
		m_modelInstance = new KModel();
	}
	return m_modelInstance;
}

void KModel::addModelListener(IModelListener *viewObject)
{
	m_vectorModelListener.push_back(viewObject);
}
void KModel::addShape(ShapeTreeNode *&shapeTree)
{
	m_vectorShapeTree.push_back(shapeTree);
}
void KModel::combineNode(std::vector<ShapeTreeNode*> &vecNode)
{
	for (int i = vecNode.size() - 1; i >= 0; --i)
	{
		removeNode(vecNode[i]);
	}

	ShapeTreeNode *parent = createShapeTreeParent();
	for (int i = 0; i < vecNode.size(); ++i)
	{
		vecNode[i]->m_pShapeParent = parent;
		if (i > vecNode.size() - 2)
		{
			break;
		}
		vecNode[i]->m_pNextSibling = vecNode[i + 1];
	}
	vecNode[0]->m_pShapeParent = parent;
	parent->m_pFirstChild = vecNode[0];

	m_vectorShapeTree.push_back(parent);
}
void KModel::CancelCombineNode(ShapeTreeNode *shapeTree)
{
	if (!shapeTree)
	{
		return;
	}
	
	ShapeTreeNode *child = shapeTree->m_pFirstChild;
	ShapeTreeNode *tmpChild = shapeTree;
	child->m_pShapeParent = NULL;
	m_vectorShapeTree.push_back(shapeTree->m_pFirstChild);
	child = child->m_pNextSibling;
	tmpChild->m_pNextSibling = NULL;

	while (!child)
	{
		tmpChild = child;
		child->m_pShapeParent = NULL;
		m_vectorShapeTree.push_back(child);
		child = child->m_pNextSibling;
		tmpChild->m_pNextSibling = NULL;
	}
	removeNode(shapeTree);
	//delete shapeTree;
}
void KModel::removeNode(ShapeTreeNode *&shapeTree)
{
	if (!shapeTree)
	{
		return;
	}
	ShapeTreeNode *parent =  shapeTree->m_pShapeParent;
	while (parent)
	{
		shapeTree = parent;
		parent = parent->m_pShapeParent;
	}
	for (std::vector<ShapeTreeNode*>::iterator iter= m_vectorShapeTree.begin();
		iter != m_vectorShapeTree.end();)
	{
		
		if ((*iter) == shapeTree)
		{
			m_vectorShapeTree.erase(iter);
			break;
		}
		++iter;
	}
}
void KModel::removeTree(ShapeTreeNode *shapeTree)
{
	if (shapeTree)
	{
		removeTree(shapeTree->m_pFirstChild);
		removeTree(shapeTree->m_pNextSibling);

		if (!shapeTree->m_pFirstChild)
		{
			delete shapeTree;
			shapeTree = NULL;
		}
	}
}
void KModel::deleteNode(ShapeTreeNode *shapeTree)
{
	removeNode(shapeTree);
	removeTree(shapeTree);
}
void KModel::getOneTreeLeaf(ShapeTreeNode *treeNode, std::vector<ShapeTreeNode*> &vecNode)
{
	if (treeNode)
	{
		if (!treeNode->m_pFirstChild)
		{
			vecNode.push_back(treeNode);
		}
		getOneTreeLeaf(treeNode->m_pFirstChild, vecNode);
		getOneTreeLeaf(treeNode->m_pNextSibling, vecNode);
	}
}
void KModel::getVecShapeLeaf(std::vector<ShapeTreeNode*> &vecNode)
{
	foreach(ShapeTreeNode *treeNode, m_vectorShapeTree)
	{
		getOneTreeLeaf(treeNode, vecNode);
	}
}
void KModel::getShapeParent(ShapeTreeNode *&treeNode)
{
	while (treeNode)
	{
		if (!treeNode->m_pShapeParent)
		{
			return;
		}
		treeNode = treeNode->m_pShapeParent;
	}
}
ShapeTreeNode* KModel::createShapeTreeParent()
{
	ShapeTreeNode* parent = new ShapeTreeNode();
	parent->m_pShapeParent = NULL;
	parent->m_pShape = new KShape();
	
	return parent;
}
ShapeTreeNode* KModel::createShapeTreeLeaf(SHAPETYPE shapetype)
{
	ShapeTreeNode* parent = createShapeTreeParent();
	
	m_pShapeTree = new ShapeTreeNode;
	m_pShapeTree->m_pShapeParent = parent;
	m_pShapeTree->m_pShape = new KShape();
	m_pShapeTree->m_pShape->setShapeType(shapetype);
	parent->m_pFirstChild = m_pShapeTree;
	return parent;
}
void KModel::copyTree(ShapeTreeNode *original, ShapeTreeNode *&copy)
{
	 
}