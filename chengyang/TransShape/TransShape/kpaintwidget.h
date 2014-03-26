///////////////////////////////////////////////////////////////
//
// FileName : k_paintwidget.h
// Creator : chengyang
// Date : 2014-03-06
// Comment :���������centralWidget��������ʾͼ��
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

	typedef void (KPaintWidget::*fBindShape)();		//����ָ�룬����Ҫ�����ͼ�ζ���ʹ����ͼ�εĺ�����
	typedef QVector<QRect> QVecRect;				//QVector<QRect>

	enum MICRORECTADDR								//��ö��ֵ����С���ε�λ��
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
	bool	judgeMouseInShape(const QPoint &Pos, KShape **sh);							///�ж������ͼ����
	void	processMousePressEvent(KShape::ShapeType &shapetype, QMouseEvent *event);		///������갴���¼�
	void	createMicroRect(KShape *sh);													///��ͼ����Χ����С�ľ���
	bool	judgeMouseInMicroRect(const QPoint &Pos, KShape **sh, 
				KPaintWidget::MICRORECTADDR &m_enumMicRectAddr);						///�ж�����Ƿ���ͼ����Χ��С������
	void	initialVecBindShape();														///��ʼ������ָ���Vector
	void	processCursorInMicroRect(QMouseEvent *event);
	void	shapeMove(QMouseEvent *event);
	void	shapeScale();
	void	shapeRotate(QMouseEvent *event, KShape *sh);
	float	calRotateRate(const QPoint &anglePointA, const QPoint &rotatePointB, 
				const QPoint &movePointC);												//ͨ��������������һ����н�
	void	setShapePainter(QPainter &painter, KShape *sh);
public:
	///����Ҫ�����ͼ�ζ���
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
	KShape				*m_pShape;								///��ǰ��ͼ��
	KShape				*m_pOldShape;							///���ƶ���ת����ʱָ��ɵ�ͼ��
	bool				m_bDraw;								///�����ж��Ƿ��������ͼ��
	bool				m_bShapeMove;							///�ж��Ƿ������ƶ�ͼ��
	bool				m_bCurInSharp;							///�ж�����Ƿ���ͼ����
	bool				m_bCurInMicRect;						///�ж������С������
	bool				m_bShapeScale;							///�ж��Ƿ�������ͼ��
	bool				m_bShapeRotate;							///�ж�����Ƿ�����ת
	int					m_nMousePointToStartPointW;				///������뱻ѡ�е�ͼ�������ϵ�Ŀ�
	int					m_nMousePointToStartPointH;				///������뱻ѡ�е�ͼ�������ϵ�ĸ�
	KPaintWidget::MICRORECTADDR				m_enumMicRectAddr;		///��¼������Ǹ�С��������
	QVector<KShape*>						m_vecShapeVector;		///��Ŵ����ϵ�ͼ��
	QMap<KShape::ShapeType, fBindShape>		m_mapBindShape;			///ͼ������Ϊ��������ָ��λ�ý��ж�̬��
	QMap<int, QVecRect>						m_mapShapeFindMicroRect;///ͨ��ͼ�ε������鵽ͼ����Χ�ľ��� 
};

#endif // __TRANSSHAPE_PAINTWIDGET_H_