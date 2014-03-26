///////////////////////////////////////////////////////////////
//
// FileName : k_shape.h
// Creator : chengyang
// Date : 2014-03-06
// Comment :����ͼ�εĻ���
//
///////////////////////////////////////////////////////////////
#ifndef __TRANSSHAPE_SHAPE_H__
#define __TRANSSHAPE_SHAPE_H__

#include "global.h"
#include <QWidget>
#include <QPainter>

class QPainter;
class KShape:public QWidget
{
	Q_OBJECT

public:

	enum ShapeType
	{
		None = -1,							//��
		Line,								//����
		Rect,								//����
		Ellipse,							//��Բ
		Triangle							//������
	};

	KShape(QWidget *parent =0);
	virtual ~KShape();

	QPoint		getTopLeftPoint(){return m_pointTopLeftPoint;}
	QPoint		getBottomRightPoint(){return m_pointBottomRightPoint;}
	QPoint		getTopRightPoint(){return m_pointTopRightPoint;}
	QPoint		getBottomLeftPoint(){return m_pointBottomLeftPoint;}
	void		setShapeWidth(int width){m_nShapeWidth = width;}
	int			getShapeWidth(){return m_nShapeWidth;}
	void		setShapeHeight(int height){m_nShapeHeight = height;}
	int			getShapeHeight(){return m_nShapeHeight;}
	void		setShapeType(KShape::ShapeType type){m_nShapeType = type;}
	ShapeType	getShapeType(){return m_nShapeType;}
	void		setShapeRotateRate(int rate){m_fShapeRotateRate = rate;}
	float		getShapeRotateRate(){return m_fShapeRotateRate;}
	void		setShapeCenterPoint(QPoint point){m_pointShapeCenterPoint = point;}
	QPoint		getShapeCenterPoint(){return m_pointShapeCenterPoint;}
	void		setShapeRotatePoint(QPoint point){m_pointShapeRotatePoint = point;}
	QPoint		getShapeRotatePoint(){return m_pointShapeRotatePoint;}
	void		setShapeBeScale(bool isScale){m_bShapeBeScaled = isScale;}
	bool		getShapeBeScale(){return m_bShapeBeScaled;}
	void		setOldShapeCenterPoint(QPoint point){m_pointOldShapeCenterPoint = point;}
	QPoint		getOldShapeCenterPoint(){return m_pointOldShapeCenterPoint;}

public:
	void		setShapePainter(QPainter &painter);
	void		setTopLeftPoint(const QPoint &point);
	void		setTopRightPoint(const QPoint &point);
	void		setBottomLeftPoint(const QPoint &point);
	void		setBottomRightPoint(const QPoint &point);
	float		calLineLength(const QPoint &pa,const QPoint &pb);

public:
	void virtual paint(QPainter & painter);
	bool virtual judgeMouseInShape(const QPoint &Pos);

protected:
	QPoint		m_pointTopLeftPoint;				//ͼ�����ϵ�
	QPoint		m_pointTopRightPoint;				//ͼ�ε����ϵ�
	QPoint		m_pointBottomRightPoint;			//ͼ�����µ�
	QPoint		m_pointBottomLeftPoint;				//ͼ�ε����µ�
	int			m_nShapeWidth;						//ͼ�εĿ�
	int			m_nShapeHeight;						//ͼ�εĸ�
	ShapeType	m_nShapeType;						//��ͼ�ε�����
	float		m_fShapeRotateRate;					//ͼ����ת��
	QPoint		m_pointShapeCenterPoint;			//ͼ�ε����ĵ�����
	QPoint		m_pointShapeRotatePoint;			//��ת������
	bool		m_bShapeBeScaled;					//�ж�ͼ���Ƿ�����
	QPoint		m_pointOldShapeCenterPoint;			//ԭͼ�ε����ĵ�
};

#endif // __TRANSSHAPE_SHAPE_H__