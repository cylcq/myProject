///////////////////////////////////////////////////////////////
//
// FileName : k_paintwidget.cpp
// Creator : chengyang
// Date : 2014-03-06
// Comment :���������centralWidget��������ʾͼ��
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
	m_pOldShape = NULL;								//ָ������ͼ�ε�ԭͼ��
	m_nMousePointToStartPointW = 0;
	m_nMousePointToStartPointH = 0;
	m_enumMicRectAddr = KPaintWidget::None;			//С���ε�λ��

	initialVecBindShape();							//��ʼ��vecBindShape����ָ������

	setMouseTracking(true);							//����������

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
 * @brief : ����QPainter������
 * @param : painter: QPainter����
 * @return None
*/
void KPaintWidget::setShapePainter(QPainter &painter, KShape *sh)
{
	if (sh->getShapeRotateRate() >= g_cfNearZero || sh->getShapeRotateRate() <= -g_cfNearZero)
	{
		///�õ�K_Shape����ת�Ⱥ����ĵ������ת
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
 * @brief ���ڴ����ػ�
 * @param event:�����ػ��¼�
 * @return None
*/
void KPaintWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setPen(QPen(Qt::gray, 2, Qt::SolidLine, Qt::RoundCap));
	painter.setRenderHint(QPainter::Antialiasing,true);

	foreach(KShape *sh, m_vecShapeVector)				/// ��������������Ա�����ʷ�ۼ�
	{
		painter.save();
		sh->paint(painter);
		painter.restore();
	}

	KShape *sh = NULL;
	QMap<int, QVecRect>::const_iterator it = m_mapShapeFindMicroRect.constBegin();
	for (; it != m_mapShapeFindMicroRect.constEnd(); it++)
	{
		sh = m_vecShapeVector.at(it.key());				///�õ�С���ζ�Ӧ��KShape
		setShapePainter(painter, sh);					///����Shape��Ӧ��painter

		painter.save();
		painter.drawRects((*it));
		painter.restore();
	}
}
/**
 * @brief ���ڴ�����갴���¼�
 * @param event:����¼�
 * @return None
*/
void KPaintWidget::mousePressEvent(QMouseEvent *event)
{
	processMousePressEvent(m_enumCurShapeCode,event);		

	if(m_pShape && m_bDraw)
	{
		m_pShape->setTopLeftPoint(event->pos());
		m_pShape->setBottomRightPoint(event->pos());

		m_vecShapeVector.push_back(m_pShape);				///���m_pShape��m_vecShapeVector��
	}
	if (m_bCurInMicRect)
	{
		setCursor(Qt::CrossCursor);							///��������С������
	}
	update();
}

void KPaintWidget::mouseMoveEvent(QMouseEvent *event)
{
	if(m_pShape && m_bDraw)
	{
		///����ƶ���ʼ��ͼ
		m_pShape->setBottomRightPoint(event->pos());
		m_mapShapeFindMicroRect.clear();			//���С����
	}
	processCursorInMicroRect(event);				///��������ƶ���С�����еĲ���

	shapeMove(event);								///������갴�²����ƶ�ͼ��

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
		m_vecShapeVector.remove(index);					///�ƶ�������ɾ��ԭͼ��
		m_bShapeMove = false;
		m_mapShapeFindMicroRect.clear();
	}
	if(m_bCurInSharp && m_pShape)
	{
		createMicroRect(m_pShape);						///��С���δ������µ�ͼ������
	}
	if (m_bShapeScale || m_bShapeRotate)
	{
		//����ͼ�ε���ʼ�����ֹ��

		m_pShape->setOldShapeCenterPoint(m_pShape->getShapeCenterPoint());
		int index = m_vecShapeVector.indexOf(m_pOldShape);
		m_vecShapeVector.remove(index);					///������ɺ�ɾ��ԭͼ��
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
 * @brief �����ж�����Ƿ���һ��ͼ����
 * @param Pos:��굱ǰ��λ��
 * @param sh:ָ������������Ǹ�ͼ���ϣ�������û����ͼ������ָ��NULL
 * @return bool:��������ͼ�����򷵻�true�����򷵻�false
*/
bool KPaintWidget::judgeMouseInShape(const QPoint &Pos, KShape **sh)
{
	if (m_bShapeRotate || m_bShapeScale)
	{
		return false;					//�����ͼ�����ڲ������򲻴�����¼���ֱ�ӷ���false
	}
	if (!m_vecShapeVector.isEmpty())
	{
		QPoint curMousePoint;
		///�Ӻ���ѭ�������Ѵ��ڵ�ͼ���ж�����Ƿ���������
		for(int i = m_vecShapeVector.count() - 1; i >= 0; --i)
		{
			 curMousePoint = Pos;
			(*sh) = m_vecShapeVector.at(i);
			if ((*sh)->getShapeRotateRate() >= g_cfNearZero || (*sh)->getShapeRotateRate() <= -g_cfNearZero)
			{
				///���ͼ����ת����������������mapӳ�䣬�����������ת�������ϵͳ�ϵ�����ֵ��
				///�Դ�����ֵ�����ж�
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
 * @brief �����ж�����Ƿ���һ��ͼ����Χ��С������
 * @param Pos:��굱ǰ��λ��
 * @param sh:ָ��С���ζ�Ӧͼ��
 * @param m_enumMicRectAddr:��������С�����У����ֵ��ʾ�������һ��С������
 * @return bool:��������С�������򷵻�true�����򷵻�false
*/
bool KPaintWidget::judgeMouseInMicroRect(const QPoint &Pos, KShape **sh, 
	KPaintWidget::MICRORECTADDR &m_enumMicRectAddr)
{
	if (m_bShapeMove || m_bShapeRotate || m_bShapeScale)
	{
		//�����ͼ�����ڲ������򲻴�����¼���ֱ�ӷ���false
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
					/// ���ͼ����ת����������������mapӳ�䣬�����������ת�������ϵͳ�ϵ�����ֵ��
					///�Դ�����ֵ�����ж�
					int centerPointX = (*sh)->getShapeCenterPoint().x();
					int centerPointY = (*sh)->getShapeCenterPoint().y();

					QTransform transform;
					transform.translate(centerPointX, centerPointY );
					transform.rotate(-((*sh)->getShapeRotateRate()));
					transform.translate(-centerPointX, -centerPointY);
					curPos = transform.map(curPos);///mapӳ���Ժ��µ��������ֵ
				}
				if((*it).at(i).contains(curPos,true))
				{
					if (m_vecShapeVector.count() - 1 >= it.key())
					{
						m_pShape = m_vecShapeVector.at(it.key());		///�õ�������Ǹ�С������
						m_enumMicRectAddr = (MICRORECTADDR)(i + 1);///0��ʾ����С�����У������Ҫ��1
						return true;
					}
				}
			}///end of for (int i = (*it).count() - 1; i >= 0; --i)
		}
	}

	return false;
}
/**
 * @brief ������갴�µ��¼�
 * @param code:ö��ֵ����ͼ�α�־����ͬ��code����ͬ��ͼ��
 * @param event������¼�
 * @return None
*/
void KPaintWidget::processMousePressEvent(KShape::ShapeType &shapetype, QMouseEvent *event)
{
	KShape *tmpShape = NULL;
	if(shapetype == KShape::None)
	{
		///û��ѡ��������,����Ҫ�жϰ��µ����λ���Ƿ����Ѿ����ڵ�ͼ����
		if (judgeMouseInShape(event->pos(),&tmpShape))
		{
			m_bCurInSharp = true;
			///��ͼ������tmpShapeָ��ѡ�е�ͼ��������
			if (m_mapShapeFindMicroRect.isEmpty())
			{
				createMicroRect(tmpShape);
			}
			else if (QApplication::keyboardModifiers() == Qt::ControlModifier)
			{
				///����ctrl������ѡ�ж��ͼ��
				if (event->button() == Qt::LeftButton)
				{
					createMicroRect(tmpShape);
				}
			}
			else if (!(event->buttons() & Qt::RightButton))
			{
				m_mapShapeFindMicroRect.clear();		///���֮ǰ��ͼ����Χ�ϵ�С����
				createMicroRect(tmpShape);				///����ͼ����Χ����С����
			}
		}
		else
		{
			///�������ԭ������Χ��С����
			m_bCurInSharp = false;
			if (!m_bCurInMicRect)
			{
				m_mapShapeFindMicroRect.clear();
			}
		}
	}
	else
	{
		(this->*m_mapBindShape[shapetype])();				///��������ͼ��
	}
	if (tmpShape)
	{
		///���㵱ǰ���µ��������
		m_nMousePointToStartPointW = event->x() - tmpShape->getTopLeftPoint().x();
		m_nMousePointToStartPointH	= event->y() - tmpShape->getTopLeftPoint().y();
	}
}
/**
 * @brief ���������С�����еĲ�����ͼ�����죬��ת��
 * @param event������¼�
 * @return None
*/
void KPaintWidget::processCursorInMicroRect(QMouseEvent *event) 
{
	if (m_bShapeMove)
	{
		return;							///���ͼ�����ƶ�,�������������ת
	}
	QPoint curMousePoint = event->pos();
	KShape *tmpShape = NULL;
	if (judgeMouseInMicroRect(curMousePoint,&tmpShape,m_enumMicRectAddr) 
		|| m_bShapeScale || m_bShapeRotate)
	{
		if (!m_bShapeScale && (event->buttons() & Qt::LeftButton))
		{
			///����Ҫ���������ת��ͼ�Σ���֤�����������ת������������һ��
			(this->*m_mapBindShape[tmpShape->getShapeType()])();

			m_pOldShape = tmpShape;

			///��ʼ���½���m_pShape;
			m_pShape->setTopLeftPoint(m_pOldShape->getTopLeftPoint());
			m_pShape->setBottomRightPoint(m_pOldShape->getBottomRightPoint());
			m_pShape->setShapeType(m_pOldShape->getShapeType());
			m_pShape->setShapeRotateRate(m_pOldShape->getShapeRotateRate());
			m_bShapeScale = true;								///��������Ϊfalse;
			m_pShape->setShapeBeScale(true);					///��Ǹ�ͼ��Ϊ������
			m_pShape->setOldShapeCenterPoint(m_pOldShape->getShapeCenterPoint());
			
			m_vecShapeVector.push_back(m_pShape);				///���m_pShape��m_vecShapeVector��

		}
		
		if (m_pShape->getShapeRotateRate() >= g_cfNearZero || m_pShape->getShapeRotateRate() <= -g_cfNearZero)
		{
			///���ͼ����ת����������������mapӳ�䣬�����������ת�������ϵͳ�ϵ�����ֵ��
			QTransform transform;
			int centerPointX = m_pShape->getOldShapeCenterPoint().x();
			int centerPointY = m_pShape->getOldShapeCenterPoint().y();

			transform.translate(centerPointX, centerPointY );
			transform.rotate(-(m_pShape->getShapeRotateRate()));
			transform.translate(-centerPointX, -centerPointY);
			curMousePoint = transform.map(curMousePoint);///mapӳ���Ժ��µ��������ֵ
		}

		m_bCurInMicRect = true;	///�����Сͼ����
		QPoint  shapeStartPoint = m_pShape->getTopLeftPoint();
		QPoint  shapeEndPoint = m_pShape->getBottomRightPoint();

		switch(m_enumMicRectAddr)
		{
		case KPaintWidget::TopLeft:
			{
				///�������
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
		case KPaintWidget::RotateRect:					///����ͼ����ת
			{
				setCursor(Qt::CrossCursor);
				if (event->buttons() & Qt::LeftButton)
				{
					m_bShapeRotate = true;				///���ͼ��������ת

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
 * @brief ����ͼ���ƶ��Ĳ���
 * @param event������¼�
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
		///�����������������ϵ�Ŀ�͸ߡ�
		setCursor(Qt::SizeAllCursor);

		if(!m_bDraw)
		{
			if ((event->buttons() & Qt::LeftButton))
			{
				///�������������²�������ƶ�ʱ��ͼ���ƶ�
				///ͼ���ƶ� 
				if (!m_bShapeMove)
				{
					///����Ҫ�ƶ���ͼ�Σ���֤���ƶ�������������һ��
					(this->*m_mapBindShape[tmpShape->getShapeType()])();
					m_vecShapeVector.push_back(m_pShape);				///���m_pShape��m_vecShapeVector��
					///m_pOldShapeָ�����ڶ���ͼ��
					m_pOldShape = tmpShape;
				}
				m_bCurInSharp = true;
				m_bShapeMove = true;
				
			}
			if (m_bShapeMove)
			{
				///�����ƶ���ͼ�ε�λ��
				if(m_pShape)
				{
					///�����µľ��ε���ʼ�����ֹ��
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
	///��֪ͼ�ε����ĵ㣬��ת�㣬�Լ���ǰ����λ�ã�����ͼ����ת�ĽǶ�
}

/**
 * @brief �ۺ�����������϶��ͼ��
 * @return None
*/
void KPaintWidget::combineShape()
{
	if (m_mapShapeFindMicroRect.count() > 1)
	{
		//��ʾѡ���˶��ͼ�Σ����Խ������
		
	}
}

/**
 * @brief ���õ�ǰҪ������ͼ������
 * @param code��ö��ֵ����ͼ�α�־����ͬ��code����ͬ��ͼ�� 
 * @return None
*/
void KPaintWidget::setCurrentShape(KShape::ShapeType code)
{
	if(code != m_enumCurShapeCode)
	{
		m_enumCurShapeCode = code;  /// �趨��״
		m_bDraw = true;
	}
}

/**
 * @brief :����ͼ����Χ��С����
 * @param tmpShape:��ʾҪ��tmpShape��ָ���ͼ����Χ����С����
 * @return None
*/
void KPaintWidget::createMicroRect( KShape *tmpShape )
{
	QRect rect(tmpShape->getTopLeftPoint(),tmpShape->getBottomRightPoint());
	int	rectLeftX = 0 , rectLeftY = 0;
	int rectW = 0,rectH = 0;
	rect.getRect(&rectLeftX , &rectLeftY , &rectW , &rectH);

	QPoint centerPoint(tmpShape->getShapeCenterPoint());

	///��ԭ���ε����ܻ��Ÿ�С���Σ�
	///��С���εĶ�����뵽microRectVector��;
	QVecRect microRectVector;
	int rectWidth = 6;
	int halfWidth = rectWidth / 2;
	///��
	microRectVector.push_back(QRect(rectLeftX - rectWidth, rectLeftY - rectWidth, rectWidth, rectWidth));
	microRectVector.push_back(QRect(rectLeftX + rectW / 2 - halfWidth, rectLeftY - rectWidth, rectWidth, rectWidth));
	microRectVector.push_back(QRect(rectLeftX + rectW, rectLeftY - rectWidth, rectWidth, rectWidth));
	///��
	microRectVector.push_back(QRect(rectLeftX - rectWidth, rectLeftY + rectH / 2 - halfWidth, rectWidth, rectWidth));
	microRectVector.push_back(QRect(rectLeftX + rectW, rectLeftY + rectH / 2 - halfWidth, rectWidth, rectWidth));
	///��
	microRectVector.push_back(QRect(rectLeftX - rectWidth, rectLeftY + rectH, rectWidth, rectWidth));
	microRectVector.push_back(QRect(rectLeftX + rectW / 2 - halfWidth , rectLeftY + rectH, rectWidth , rectWidth));
	microRectVector.push_back(QRect(rectLeftX + rectW, rectLeftY + rectH, rectWidth, rectWidth));
	
	///��ת��
	///��ת��������ĵ�ĸ߶�Ϊ���ĵ�������� - ��/2 - 15;
	microRectVector.push_back(QRect(rectLeftX + rectW / 2 - halfWidth , rectLeftY - halfWidth - 15 , rectWidth , rectWidth));

	///��������С����ָ��tmpShape���ͼ�ζ���
	int key = m_vecShapeVector.indexOf(tmpShape);
	if (key < 0)
	{
		return;
	}
	m_mapShapeFindMicroRect.insert(key, microRectVector);
}

/**
 * @brief :�������������������һ����ļнǶ���
 * @param anglePointA:��һ���㣬Ҫ����õ����ڽǵĶ���
 * @param rotatePointB:�ڶ�����
 * @param movePointC:��������
 * @return None
*/
float  KPaintWidget::calRotateRate(const QPoint &anglePointA,
	const QPoint &rotatePointB, const QPoint &movePointC)
{
	if (movePointC == rotatePointB)
	{
		return 0;				///û����ת
	}
	if (movePointC.x() == anglePointA.x() && movePointC.y() > anglePointA.y())
	{
		return 180;				///������ƶ������ĵ�����·�������ת180
	}

	/*
	*	A��ʾanglePointA��,A����Եıߵı߳�Ϊa
	*	B��ʾrotatePointB�㣬B����Եıߵı߳�Ϊb
	*	C��ʾmovePointC��,C����Եıߵı߳�Ϊc
	*/

	//A�������ֵ
	int ax = anglePointA.x();
	int ay = anglePointA.y();
	//B�������ֵ
	int bx = rotatePointB.x();
	int by = rotatePointB.y();
	//C�������ֵ
	int cx = movePointC.x();
	int cy = movePointC.y();

	//����b = (bx-cx,by-cy);����b = ��cx - ax,cy - ay��
	QPoint vectorB = QPoint(bx - ax, by - ay);
	QPoint vectorC = QPoint(cx -ax, cy - ay);

	//���������нǹ�ʽ����нǵĻ���(x1x2+ y1y2)/(sqrt(pow(x1)+pow(y1)) * sqrt(pow(x2)+pow(y2)) )
	 double angle = qAcos((vectorB.x() * vectorC.x() + vectorB.y() * vectorC.y()) / 
		 (sqrt(qPow(vectorB.x(),2) + qPow(vectorB.y(),2)) * sqrt(qPow(vectorC.x(),2) + qPow(vectorC.y(),2))));

	if (movePointC.x() > anglePointA.x())
	{
		return (angle * 180) / g_cfPI;
		///����������ĵ�Ϊԭ�������ϵ�ĵ�һ�������ޣ�������ֵ
	}
	else
	{
		return -(angle * 180) / g_cfPI;
		///����������ĵ�Ϊԭ�������ϵ�ĵڶ��������ޣ����ظ�ֵ
	}
	return 0;
}
/**
 * @brief :��ʼ������ָ���map����
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
	///����һ����
	m_pShape = new KLine(this);
}
void KPaintWidget::createRect()
{
	///����һ������
	m_pShape = new KRect(this);
}
void KPaintWidget::createEllipse()
{
	m_pShape = new KEllipse(this);
}

