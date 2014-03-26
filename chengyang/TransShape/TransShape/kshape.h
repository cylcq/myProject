///////////////////////////////////////////////////////////////
//
// FileName : k_shape.h
// Creator : chengyang
// Date : 2014-03-06
// Comment :所有图形的基类
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
		None = -1,							//无
		Line,								//线条
		Rect,								//矩形
		Ellipse,							//椭圆
		Triangle							//三角形
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
	QPoint		m_pointTopLeftPoint;				//图形左上点
	QPoint		m_pointTopRightPoint;				//图形的右上点
	QPoint		m_pointBottomRightPoint;			//图形右下点
	QPoint		m_pointBottomLeftPoint;				//图形的左下点
	int			m_nShapeWidth;						//图形的宽
	int			m_nShapeHeight;						//图形的高
	ShapeType	m_nShapeType;						//该图形的类型
	float		m_fShapeRotateRate;					//图形旋转度
	QPoint		m_pointShapeCenterPoint;			//图形的中心点坐标
	QPoint		m_pointShapeRotatePoint;			//旋转点坐标
	bool		m_bShapeBeScaled;					//判断图形是否被拉伸
	QPoint		m_pointOldShapeCenterPoint;			//原图形的中心点
};

#endif // __TRANSSHAPE_SHAPE_H__