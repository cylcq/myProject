///////////////////////////////////////////////////////////////
//
// FileName : k_paintwidget.cpp
// Creator : chengyang
// Date : 2014-03-06
// Comment :窗体的中心centralWidget，用于显示图形
//
///////////////////////////////////////////////////////////////

#include "kpaintwidget.h"
#include "global.h"
#include <QtDebug>

KPaintWidget::KPaintWidget(QWidget *parent)
	: QWidget(parent)
	, m_enumCurShapeCode(KShape::None)
	, m_bDraw(false)
	, m_bShapeMove(false)
	, m_bCurInSharp(false)
	, m_bCurInMicRect(false)
	, m_bShapeScale(false)
	, m_bShapeRotate(false)
{
	
	m_pShape = NULL;
	m_pOldShape = NULL;								//指向正在图形的原图形
	m_nMousePointToStartPointW = 0;
	m_nMousePointToStartPointH = 0;
	m_enumMicRectAddr = KPaintWidget::None;			//小矩形的位置

	initialVecBindShape();							//初始化vecBindShape函数指针数组

	setMouseTracking(true);							//设置鼠标跟踪

	connect(this,SIGNAL(sendPainter(QPainter&)),this,SLOT(receivePainter(QPainter&)));

}
KPaintWidget::~KPaintWidget()
{
	if (!m_vecShapeVector.isEmpty())
	{
		foreach(KShape *sh, m_vecShapeVector)
		{
			delete sh;
			sh = NULL;
		}
	}
	m_vecShapeVector.clear();
}
/**
 * @brief : 设置QPainter的属性
 * @param : painter: QPainter对象
 * @return None
*/
void KPaintWidget::setShapePainter(QPainter &painter, KShape *sh)
{
	if (sh->getShapeRotateRate() >= g_cfNearZero || sh->getShapeRotateRate() <= -g_cfNearZero)
	{
		///得到K_Shape的旋转度和中心点进行旋转
		if (sh->getShapeBeScale())
		{
			painter.translate(sh->getOldShapeCenterPoint());
		}
		else
		{
			painter.translate(sh->getShapeCenterPoint());
		}
		painter.rotate(sh->getShapeRotateRate());

		if (sh->getShapeBeScale())
		{
			painter.translate(-(sh->getOldShapeCenterPoint()));
		}
		else
		{
			painter.translate(-(sh->getShapeCenterPoint()));
		}
	}
}
/**
 * @brief 用于窗体重绘
 * @param event:窗体重绘事件
 * @return None
*/
void KPaintWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setPen(QPen(Qt::gray, 2, Qt::SolidLine, Qt::RoundCap));
	painter.setRenderHint(QPainter::Antialiasing,true);

	foreach(KShape *sh, m_vecShapeVector)				/// 这里遍历容器可以保留历史痕迹
	{
		painter.save();
		sh->paint(painter);
		painter.restore();
	}

	KShape *sh = NULL;
	QMap<int, QVecRect>::const_iterator it = m_mapShapeFindMicroRect.constBegin();
	for (; it != m_mapShapeFindMicroRect.constEnd(); it++)
	{
		sh = m_vecShapeVector.at(it.key());				///得到小矩形对应的KShape
		setShapePainter(painter, sh);					///设置Shape对应的painter

		painter.save();
		painter.drawRects((*it));
		painter.restore();
	}
}
/**
 * @brief 用于处理鼠标按下事件
 * @param event:鼠标事件
 * @return None
*/
void KPaintWidget::mousePressEvent(QMouseEvent *event)
{
	processMousePressEvent(m_enumCurShapeCode,event);		

	if(m_pShape && m_bDraw)
	{
		m_pShape->setTopLeftPoint(event->pos());
		m_pShape->setBottomRightPoint(event->pos());

		m_vecShapeVector.push_back(m_pShape);				///添加m_pShape到m_vecShapeVector中
	}
	if (m_bCurInMicRect)
	{
		setCursor(Qt::CrossCursor);							///如果鼠标在小矩形区
	}
	update();
}

void KPaintWidget::mouseMoveEvent(QMouseEvent *event)
{
	if(m_pShape && m_bDraw)
	{
		///鼠标移动开始画图
		m_pShape->setBottomRightPoint(event->pos());
		m_mapShapeFindMicroRect.clear();			//清空小矩形
	}
	processCursorInMicroRect(event);				///处理鼠标移动到小矩形中的操作

	shapeMove(event);								///处理鼠标按下并且移动图形

	update();
}

void KPaintWidget::mouseReleaseEvent(QMouseEvent *event)
{
	m_enumCurShapeCode = KShape::None;
	m_bDraw = false;
	if (m_bShapeMove)
	{
		m_pShape->setOldShapeCenterPoint(m_pShape->getShapeCenterPoint());
		int index = m_vecShapeVector.indexOf(m_pOldShape);
		m_vecShapeVector.remove(index);					///移动结束，删除原图形
		m_bShapeMove = false;
		m_mapShapeFindMicroRect.clear();
	}
	if(m_bCurInSharp && m_pShape)
	{
		createMicroRect(m_pShape);						///将小矩形创建到新的图形上面
	}
	if (m_bShapeScale || m_bShapeRotate)
	{
		//重置图形的起始点和终止点

		m_pShape->setOldShapeCenterPoint(m_pShape->getShapeCenterPoint());
		int index = m_vecShapeVector.indexOf(m_pOldShape);
		m_vecShapeVector.remove(index);					///拉伸完成后删除原图形
		m_bShapeScale = false;
		m_bShapeRotate = false;
		m_mapShapeFindMicroRect.clear();
		createMicroRect(m_pShape);
	}
	m_pShape = NULL;
	if (m_pOldShape)
	{
		delete m_pOldShape;
		m_pOldShape = NULL;
	}
	update();
}
/**
 * @brief 用于判断鼠标是否在一个图形中
 * @param Pos:鼠标当前的位置
 * @param sh:指向鼠标落在了那个图形上，如果鼠标没有在图形上则指向NULL
 * @return bool:如果鼠标在图形区则返回true，否则返回false
*/
bool KPaintWidget::judgeMouseInShape(const QPoint &Pos, KShape **sh)
{
	if (m_bShapeRotate || m_bShapeScale)
	{
		return false;					//如果有图形正在操作，则不处理该事件，直接返回false
	}
	if (!m_vecShapeVector.isEmpty())
	{
		QPoint curMousePoint;
		///从后面循环遍历已存在的图形判断鼠标是否在其里面
		for(int i = m_vecShapeVector.count() - 1; i >= 0; --i)
		{
			 curMousePoint = Pos;
			(*sh) = m_vecShapeVector.at(i);
			if ((*sh)->getShapeRotateRate() >= g_cfNearZero || (*sh)->getShapeRotateRate() <= -g_cfNearZero)
			{
				///如果图形旋转过，则进行鼠标坐标map映射，返回鼠标在旋转后的坐标系统上的坐标值，
				///以此坐标值进行判断
				QTransform transform;
				int centerPointX = (*sh)->getShapeCenterPoint().x();
				int centerPointY = (*sh)->getShapeCenterPoint().y();
				transform.translate(centerPointX, centerPointY );
				transform.rotate(-((*sh)->getShapeRotateRate()));
				transform.translate(-centerPointX, -centerPointY);
				curMousePoint = transform.map(curMousePoint);
			}
			 if((*sh)->judgeMouseInShape(curMousePoint))
			 {
				 return true;
			 }
		}
	}
	return false;
}
/**l
 * @brief 用于判断鼠标是否在一个图形周围的小矩形中
 * @param Pos:鼠标当前的位置
 * @param sh:指向小矩形对应图形
 * @param m_enumMicRectAddr:如果鼠标在小矩形中，则该值表示鼠标在哪一个小矩形中
 * @return bool:如果鼠标在小矩形中则返回true，否则返回false
*/
bool KPaintWidget::judgeMouseInMicroRect(const QPoint &Pos, KShape **sh, 
	KPaintWidget::MICRORECTADDR &m_enumMicRectAddr)
{
	if (m_bShapeMove || m_bShapeRotate || m_bShapeScale)
	{
		//如果有图形正在操作，则不处理该事件，直接返回false
		return false;
	}
	if (!m_mapShapeFindMicroRect.isEmpty())
	{
		QMap<int, QVecRect>::const_iterator it;
		for (it = m_mapShapeFindMicroRect.begin(); it != m_mapShapeFindMicroRect.end(); it++)
		{
			for (int i = 0; i <= (*it).count() - 1; ++i)
			{
				QPoint curPos(Pos);
				*sh = m_vecShapeVector.at(it.key());
				if ((*sh)->getShapeRotateRate() >= g_cfNearZero || (*sh)->getShapeRotateRate() <= -g_cfNearZero)
				{
					/// 如果图形旋转过，则进行鼠标坐标map映射，返回鼠标在旋转后的坐标系统上的坐标值，
					///以此坐标值进行判断
					int centerPointX = (*sh)->getShapeCenterPoint().x();
					int centerPointY = (*sh)->getShapeCenterPoint().y();

					QTransform transform;
					transform.translate(centerPointX, centerPointY );
					transform.rotate(-((*sh)->getShapeRotateRate()));
					transform.translate(-centerPointX, -centerPointY);
					curPos = transform.map(curPos);///map映射以后新的鼠标坐标值
				}
				if((*it).at(i).contains(curPos,true))
				{
					if (m_vecShapeVector.count() - 1 >= it.key())
					{
						m_pShape = m_vecShapeVector.at(it.key());		///得到鼠标在那个小矩形上
						m_enumMicRectAddr = (MICRORECTADDR)(i + 1);///0表示不再小矩形中，因此需要加1
						return true;
					}
				}
			}///end of for (int i = (*it).count() - 1; i >= 0; --i)
		}
	}

	return false;
}
/**
 * @brief 处理鼠标按下的事件
 * @param code:枚举值，是图形标志，不同的code代表不同的图形
 * @param event：鼠标事件
 * @return None
*/
void KPaintWidget::processMousePressEvent(KShape::ShapeType &shapetype, QMouseEvent *event)
{
	KShape *tmpShape = NULL;
	if(shapetype == KShape::None)
	{
		///没有选择插入操作,则需要判断按下的鼠标位置是否在已经存在的图形上
		if (judgeMouseInShape(event->pos(),&tmpShape))
		{
			m_bCurInSharp = true;
			///在图形区，tmpShape指向被选中的图形区对象
			if (m_mapShapeFindMicroRect.isEmpty())
			{
				createMicroRect(tmpShape);
			}
			else if (QApplication::keyboardModifiers() == Qt::ControlModifier)
			{
				///按下ctrl键可以选中多个图形
				if (event->button() == Qt::LeftButton)
				{
					createMicroRect(tmpShape);
				}
			}
			else if (!(event->buttons() & Qt::RightButton))
			{
				m_mapShapeFindMicroRect.clear();		///清空之前的图形周围上的小矩形
				createMicroRect(tmpShape);				///在新图形周围创建小矩形
			}
		}
		else
		{
			///清空所有原矩形周围的小矩形
			m_bCurInSharp = false;
			if (!m_bCurInMicRect)
			{
				m_mapShapeFindMicroRect.clear();
			}
		}
	}
	else
	{
		(this->*m_mapBindShape[shapetype])();				///创建其它图形
	}
	if (tmpShape)
	{
		///计算当前按下的鼠标点距离
		m_nMousePointToStartPointW = event->x() - tmpShape->getTopLeftPoint().x();
		m_nMousePointToStartPointH	= event->y() - tmpShape->getTopLeftPoint().y();
	}
}
/**
 * @brief 处理鼠标在小矩形中的操作（图形拉伸，旋转）
 * @param event：鼠标事件
 * @return None
*/
void KPaintWidget::processCursorInMicroRect(QMouseEvent *event) 
{
	if (m_bShapeMove)
	{
		return;							///如果图形在移动,不处理拉伸和旋转
	}
	QPoint curMousePoint = event->pos();
	KShape *tmpShape = NULL;
	if (judgeMouseInMicroRect(curMousePoint,&tmpShape,m_enumMicRectAddr) 
		|| m_bShapeScale || m_bShapeRotate)
	{
		if (!m_bShapeScale && (event->buttons() & Qt::LeftButton))
		{
			///创建要拉伸或者旋转的图形，保证在拉伸或者旋转过程中至创建一次
			(this->*m_mapBindShape[tmpShape->getShapeType()])();

			m_pOldShape = tmpShape;

			///初始化新建的m_pShape;
			m_pShape->setTopLeftPoint(m_pOldShape->getTopLeftPoint());
			m_pShape->setBottomRightPoint(m_pOldShape->getBottomRightPoint());
			m_pShape->setShapeType(m_pOldShape->getShapeType());
			m_pShape->setShapeRotateRate(m_pOldShape->getShapeRotateRate());
			m_bShapeScale = true;								///设置拉伸为false;
			m_pShape->setShapeBeScale(true);					///标记该图形为被拉伸
			m_pShape->setOldShapeCenterPoint(m_pOldShape->getShapeCenterPoint());
			
			m_vecShapeVector.push_back(m_pShape);				///添加m_pShape到m_vecShapeVector中

		}
		
		if (m_pShape->getShapeRotateRate() >= g_cfNearZero || m_pShape->getShapeRotateRate() <= -g_cfNearZero)
		{
			///如果图形旋转过，则进行鼠标坐标map映射，返回鼠标在旋转后的坐标系统上的坐标值，
			QTransform transform;
			int centerPointX = m_pShape->getOldShapeCenterPoint().x();
			int centerPointY = m_pShape->getOldShapeCenterPoint().y();

			transform.translate(centerPointX, centerPointY );
			transform.rotate(-(m_pShape->getShapeRotateRate()));
			transform.translate(-centerPointX, -centerPointY);
			curMousePoint = transform.map(curMousePoint);///map映射以后新的鼠标坐标值
		}

		m_bCurInMicRect = true;	///鼠标在小图形中
		QPoint  shapeStartPoint = m_pShape->getTopLeftPoint();
		QPoint  shapeEndPoint = m_pShape->getBottomRightPoint();

		switch(m_enumMicRectAddr)
		{
		case KPaintWidget::TopLeft:
			{
				///拉伸操作
				if (!m_bShapeRotate)
				{
					setCursor(QCursor(Qt::SizeFDiagCursor));
					if ((event->buttons() & Qt::LeftButton))
					{
						m_pShape->setTopLeftPoint(curMousePoint);
					}
				}
				break;
			}
		case KPaintWidget::BottomRight:
			{
				if (!m_bShapeRotate)
				{
					if ((event->buttons() & Qt::LeftButton))
					{
						m_pShape->setBottomRightPoint(curMousePoint);
					}
					setCursor(Qt::SizeFDiagCursor);
				}
				break;
			}
		case KPaintWidget::LeftCentral:
			{
				if (!m_bShapeRotate)
				{
					if ((event->buttons() & Qt::LeftButton))
					{
						m_pShape->setTopLeftPoint(QPoint(curMousePoint.x(), shapeStartPoint.y()));
					}
					setCursor(Qt::SizeHorCursor);
				}
				break;
			}
		case KPaintWidget::RightCentral:
			{
				if (!m_bShapeRotate)
				{
					if ((event->buttons() & Qt::LeftButton))
					{
						m_pShape->setBottomRightPoint(QPoint(curMousePoint.x(), shapeEndPoint.y()));
					}
					setCursor(Qt::SizeHorCursor);
				}
				break;
			}
		case KPaintWidget::TopCentral:
			{
				if (!m_bShapeRotate)
				{
					if ((event->buttons() & Qt::LeftButton))
					{
						m_pShape->setTopLeftPoint(QPoint(shapeStartPoint.x(), curMousePoint.y()));
					}
					setCursor(Qt::SizeVerCursor);
				}
				break;
			}
		case KPaintWidget::BottomCentral:
			{
				if (!m_bShapeRotate)
				{
					if ((event->buttons() & Qt::LeftButton))
					{
						m_pShape->setBottomRightPoint(QPoint(shapeEndPoint.x(), curMousePoint.y()));
					}
					setCursor(Qt::SizeVerCursor);
				}
				break;
			}
		case KPaintWidget::BottomLeft:
			{
				if (!m_bShapeRotate)
				{
					if ((event->buttons() & Qt::LeftButton))
					{
						m_pShape->setBottomLeftPoint(curMousePoint);
					}
					setCursor(Qt::SizeBDiagCursor);
				}
				break;
			}
		case KPaintWidget::TopRight:
			{
				if (!m_bShapeRotate)
				{
					if ((event->buttons() & Qt::LeftButton))
					{
						m_pShape->setTopRightPoint(curMousePoint);
					}
					setCursor(Qt::SizeBDiagCursor);
				}
				break;
			}
		case KPaintWidget::RotateRect:					///处理图形旋转
			{
				setCursor(Qt::CrossCursor);
				if (event->buttons() & Qt::LeftButton)
				{
					m_bShapeRotate = true;				///标记图像正在旋转

					float angle = calRotateRate(m_pShape->getShapeCenterPoint(),
						m_pShape->getShapeRotatePoint(),event->pos());

					m_pShape->setShapeRotateRate(angle);
				}
				break;
			}
		}
	}
	else
	{
		unsetCursor();
		m_bCurInMicRect = false;
	}
	
}
/**
 * @brief 处理图形移动的操作
 * @param event：鼠标事件
 * @return None
*/
void KPaintWidget::shapeMove(QMouseEvent *event)
{
	if(m_bShapeScale || m_bShapeRotate)
	{
		return;
	}
	KShape *tmpShape = NULL;
	if (judgeMouseInShape(event->pos(), &tmpShape))
	{
		///计算鼠标点距离矩形左上点的宽和高。
		setCursor(Qt::SizeAllCursor);

		if(!m_bDraw)
		{
			if ((event->buttons() & Qt::LeftButton))
			{
				///处理鼠标左键按下并且鼠标移动时的图形移动
				///图形移动 
				if (!m_bShapeMove)
				{
					///创建要移动的图形，保证在移动过程中至创建一次
					(this->*m_mapBindShape[tmpShape->getShapeType()])();
					m_vecShapeVector.push_back(m_pShape);				///添加m_pShape到m_vecShapeVector中
					///m_pOldShape指向倒数第二个图形
					m_pOldShape = tmpShape;
				}
				m_bCurInSharp = true;
				m_bShapeMove = true;
				
			}
			if (m_bShapeMove)
			{
				///设置移动后图形的位置
				if(m_pShape)
				{
					///设置新的矩形的起始点和终止点
					m_pShape->setTopLeftPoint(QPoint(event->x() - m_nMousePointToStartPointW,
						event->y() - m_nMousePointToStartPointH));
					m_pShape->setBottomRightPoint(QPoint(event->x() - m_nMousePointToStartPointW + m_pOldShape->getShapeWidth(),
						event->y() - m_nMousePointToStartPointH + m_pOldShape->getShapeHeight()));

					m_pShape->setShapeWidth(m_pOldShape->getShapeWidth());
					m_pShape->setShapeHeight(m_pOldShape->getShapeHeight());
					m_pShape->setShapeRotateRate(m_pOldShape->getShapeRotateRate());
				}
			}///end of if (m_bShapeMove)
		}///end of if(!m_bDraw)
	}///end of if (judgeMouseInShape(event->pos(), &tmpShape))
}
void KPaintWidget::shapeScale()
{

}

void KPaintWidget::shapeRotate(QMouseEvent *event,KShape *sh)
{
	///已知图形的中心点，旋转点，以及当前鼠标的位置，计算图形旋转的角度
}

/**
 * @brief 槽函数，用来组合多个图形
 * @return None
*/
void KPaintWidget::combineShape()
{
	if (m_mapShapeFindMicroRect.count() > 1)
	{
		//表示选中了多个图形，可以进行组合
		
	}
}

/**
 * @brief 设置当前要创建的图形类型
 * @param code：枚举值，是图形标志，不同的code代表不同的图形 
 * @return None
*/
void KPaintWidget::setCurrentShape(KShape::ShapeType code)
{
	if(code != m_enumCurShapeCode)
	{
		m_enumCurShapeCode = code;  /// 设定形状
		m_bDraw = true;
	}
}

/**
 * @brief :创建图形周围的小矩形
 * @param tmpShape:表示要在tmpShape所指向的图形周围创建小矩形
 * @return None
*/
void KPaintWidget::createMicroRect( KShape *tmpShape )
{
	QRect rect(tmpShape->getTopLeftPoint(),tmpShape->getBottomRightPoint());
	int	rectLeftX = 0 , rectLeftY = 0;
	int rectW = 0,rectH = 0;
	rect.getRect(&rectLeftX , &rectLeftY , &rectW , &rectH);

	QPoint centerPoint(tmpShape->getShapeCenterPoint());

	///在原矩形的四周画九个小矩形，
	///将小矩形的对象加入到microRectVector中;
	QVecRect microRectVector;
	int rectWidth = 6;
	int halfWidth = rectWidth / 2;
	///上
	microRectVector.push_back(QRect(rectLeftX - rectWidth, rectLeftY - rectWidth, rectWidth, rectWidth));
	microRectVector.push_back(QRect(rectLeftX + rectW / 2 - halfWidth, rectLeftY - rectWidth, rectWidth, rectWidth));
	microRectVector.push_back(QRect(rectLeftX + rectW, rectLeftY - rectWidth, rectWidth, rectWidth));
	///中
	microRectVector.push_back(QRect(rectLeftX - rectWidth, rectLeftY + rectH / 2 - halfWidth, rectWidth, rectWidth));
	microRectVector.push_back(QRect(rectLeftX + rectW, rectLeftY + rectH / 2 - halfWidth, rectWidth, rectWidth));
	///下
	microRectVector.push_back(QRect(rectLeftX - rectWidth, rectLeftY + rectH, rectWidth, rectWidth));
	microRectVector.push_back(QRect(rectLeftX + rectW / 2 - halfWidth , rectLeftY + rectH, rectWidth , rectWidth));
	microRectVector.push_back(QRect(rectLeftX + rectW, rectLeftY + rectH, rectWidth, rectWidth));
	
	///旋转点
	///旋转点距离中心点的高度为中心点的纵坐标 - 高/2 - 15;
	microRectVector.push_back(QRect(rectLeftX + rectW / 2 - halfWidth , rectLeftY - halfWidth - 15 , rectWidth , rectWidth));

	///标记上面的小矩形指向tmpShape这个图形对象
	int key = m_vecShapeVector.indexOf(tmpShape);
	if (key < 0)
	{
		return;
	}
	m_mapShapeFindMicroRect.insert(key, microRectVector);
}

/**
 * @brief :根据三个点的坐标计算第一个点的夹角度数
 * @param anglePointA:第一个点，要计算该点所在角的度数
 * @param rotatePointB:第二个点
 * @param movePointC:第三个点
 * @return None
*/
float  KPaintWidget::calRotateRate(const QPoint &anglePointA,
	const QPoint &rotatePointB, const QPoint &movePointC)
{
	if (movePointC == rotatePointB)
	{
		return 0;				///没有旋转
	}
	if (movePointC.x() == anglePointA.x() && movePointC.y() > anglePointA.y())
	{
		return 180;				///当鼠标移动到中心点的正下方，则旋转180
	}

	/*
	*	A表示anglePointA点,A点相对的边的边长为a
	*	B表示rotatePointB点，B点相对的边的边长为b
	*	C表示movePointC点,C点相对的边的边长为c
	*/

	//A点的坐标值
	int ax = anglePointA.x();
	int ay = anglePointA.y();
	//B点的坐标值
	int bx = rotatePointB.x();
	int by = rotatePointB.y();
	//C点的坐标值
	int cx = movePointC.x();
	int cy = movePointC.y();

	//向量b = (bx-cx,by-cy);向量b = （cx - ax,cy - ay）
	QPoint vectorB = QPoint(bx - ax, by - ay);
	QPoint vectorC = QPoint(cx -ax, cy - ay);

	//利用向量夹角公式计算夹角的弧长(x1x2+ y1y2)/(sqrt(pow(x1)+pow(y1)) * sqrt(pow(x2)+pow(y2)) )
	 double angle = qAcos((vectorB.x() * vectorC.x() + vectorB.y() * vectorC.y()) / 
		 (sqrt(qPow(vectorB.x(),2) + qPow(vectorB.y(),2)) * sqrt(qPow(vectorC.x(),2) + qPow(vectorC.y(),2))));

	if (movePointC.x() > anglePointA.x())
	{
		return (angle * 180) / g_cfPI;
		///鼠标在以中心点为原点的坐标系的第一第四象限，返回正值
	}
	else
	{
		return -(angle * 180) / g_cfPI;
		///鼠标在以中心点为原点的坐标系的第二第三象限，返回负值
	}
	return 0;
}
/**
 * @brief :初始化函数指针的map容器
 * @return None
*/
void KPaintWidget::initialVecBindShape()
{
	m_mapBindShape[KShape::Line] = &KPaintWidget::createLine;
	m_mapBindShape[KShape::Rect] = &KPaintWidget::createRect;
	m_mapBindShape[KShape::Ellipse] = &KPaintWidget::createEllipse;
}
void KPaintWidget::createLine()
{
	///创建一条线
	m_pShape = new KLine(this);
}
void KPaintWidget::createRect()
{
	///创建一个矩形
	m_pShape = new KRect(this);
}
void KPaintWidget::createEllipse()
{
	m_pShape = new KEllipse(this);
}

