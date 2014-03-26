///////////////////////////////////////////////////////////////
//
// FileName : k_rect.cpp
// Creator : chengyang
// Date : 2014-03-06
// Comment :��һ������
//
///////////////////////////////////////////////////////////////
#include "krect.h"

KRect::KRect(QWidget *parent)
	:KShape(parent)
{
	m_nShapeType = KShape::Rect;
}
KRect::~KRect()
{

}
/**
 * @brief :��һ������
 * @param painter:ͨ������painter������ͬ����ͼ��
 * @return None
*/
void KRect::paint(QPainter &painter)
{
	setShapePainter(painter);///����painter����
	painter.drawRect(m_pointTopLeftPoint.x(), m_pointTopLeftPoint.y(),
		m_pointBottomRightPoint.x() - m_pointTopLeftPoint.x(), m_pointBottomRightPoint.y() - m_pointTopLeftPoint.y());
}

/**
 * @brief �����ж�����Ƿ���һ��ͼ����
 * @param Pos:��굱ǰ��λ��
 * @return bool:��������ͼ�����򷵻�true�����򷵻�false
*/
bool KRect::judgeMouseInShape(const QPoint &Pos)
{
	QRect rect(this->getTopLeftPoint(), this->getBottomRightPoint());
	return (rect.contains(Pos, false));
}