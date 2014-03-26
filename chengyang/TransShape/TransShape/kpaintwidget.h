///////////////////////////////////////////////////////////////
//
// FileName : k_paintwidget.h
// Creator : chengyang
// Date : 2014-03-06
// Comment :窗体的中心centralWidget，用于显示图形
//
///////////////////////////////////////////////////////////////
#ifndef __TRANSSHAPE_PAINTWIDGET_H_
#define __TRANSSHAPE_PAINTWIDGET_H_

#include <QtGui>

#include "kshape.h"
#include "kline.h"
#include "krect.h"
#include "kellipse.h"

class KPaintWidget : public QWidget
{
	Q_OBJECT

public:
	KPaintWidget(QWidget *parent = 0);
	~KPaintWidget();

	typedef void (KPaintWidget::*fBindShape)();		//函数指针，将需要插入的图形对象和处理该图形的函数绑定
	typedef QVector<QRect> QVecRect;				//QVector<QRect>

	enum MICRORECTADDR								//该枚举值代表小矩形的位置
	{
		None,
		TopLeft, 
		TopCentral, 
		TopRight,
		LeftCentral,
		RightCentral,
		BottomLeft, 
		BottomCentral,
		BottomRight,
		RotateRect
	};

	public slots:
		void	setCurrentShape(KShape::ShapeType s);
		void	combineShape();

public:
	bool	judgeMouseInShape(const QPoint &Pos, KShape **sh);							///判断鼠标在图形中
	void	processMousePressEvent(KShape::ShapeType &shapetype, QMouseEvent *event);		///处理鼠标按下事件
	void	createMicroRect(KShape *sh);													///在图形周围生成小的矩形
	bool	judgeMouseInMicroRect(const QPoint &Pos, KShape **sh, 
				KPaintWidget::MICRORECTADDR &m_enumMicRectAddr);						///判断鼠标是否在图形周围的小矩形中
	void	initialVecBindShape();														///初始化函数指针的Vector
	void	processCursorInMicroRect(QMouseEvent *event);
	void	shapeMove(QMouseEvent *event);
	void	shapeScale();
	void	shapeRotate(QMouseEvent *event, KShape *sh);
	float	calRotateRate(const QPoint &anglePointA, const QPoint &rotatePointB, 
				const QPoint &movePointC);												//通过三点坐标计算第一个点夹角
	void	setShapePainter(QPainter &painter, KShape *sh);
public:
	///创建要插入的图形对象
	void	createLine();
	void	createRect();
	void	createEllipse();

protected:
	void	paintEvent(QPaintEvent *event);
	void	mousePressEvent(QMouseEvent *event);
	void	mouseMoveEvent(QMouseEvent *event);
	void	mouseReleaseEvent(QMouseEvent *event);

private:
	KShape::ShapeType	m_enumCurShapeCode;
	KShape				*m_pShape;								///当前的图形
	KShape				*m_pOldShape;							///在移动旋转拉伸时指向旧的图形
	bool				m_bDraw;								///用于判断是否允许绘制图形
	bool				m_bShapeMove;							///判断是否正在移动图形
	bool				m_bCurInSharp;							///判断鼠标是否在图形区
	bool				m_bCurInMicRect;						///判断鼠标在小矩形中
	bool				m_bShapeScale;							///判断是否在拉伸图形
	bool				m_bShapeRotate;							///判断鼠标是否在旋转
	int					m_nMousePointToStartPointW;				///鼠标点距离被选中的图形区左上点的宽
	int					m_nMousePointToStartPointH;				///鼠标点距离被选中的图形区左上点的高
	KPaintWidget::MICRORECTADDR				m_enumMicRectAddr;		///记录鼠标在那个小矩形上面
	QVector<KShape*>						m_vecShapeVector;		///存放窗体上的图形
	QMap<KShape::ShapeType, fBindShape>		m_mapBindShape;			///图形类型为键，函数指针位置进行动态绑定
	QMap<int, QVecRect>						m_mapShapeFindMicroRect;///通过图形的索引查到图形周围的矩形 
};

#endif // __TRANSSHAPE_PAINTWIDGET_H_