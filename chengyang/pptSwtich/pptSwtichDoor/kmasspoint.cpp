#include "kmasspoint.h"
#include <QtDebug>

KMassPoint::KMassPoint()
{

}
KMassPoint::KMassPoint(QVector3D& pos)
	: m_bPullX(false)
	, m_bPullY(false)
	, m_bPullZ(false)
	, m_nMass(1)
	, m_fTimeStep(0.00001)
	, m_point3Pos(pos)
	, m_point3OldPos(pos)
	, m_pointY(pos.y())
	, m_pointZ(pos.z())
{
	
}

KMassPoint::~KMassPoint()
{

}

void KMassPoint::setPos(const QVector3D& pos)
{
	m_point3Pos = pos;
}
QVector3D KMassPoint::getPos()const
{
	return m_point3Pos;
}
QVector3D KMassPoint::getOldPos()const
{
	return m_point3OldPos;
}
void KMassPoint::setPointGravityOffset(const QVector3D& gravity)
{ 
	m_point3OffsetByGravity = gravity * m_nMass;
}
void KMassPoint::setPointSpringOffset(const QVector3D& springforce)
{
	if (!m_bPullY)
	{
		m_point3Pos += springforce;
	}
}
void KMassPoint::timeStep(float height, float frame)
{
	QVector3D pullForce/*(0, 0.6, 0)*/;

	QVector3D tmp = m_point3Pos;
	float damping = 0.6;	//阻尼力,可以模拟布料的材质
	QVector3D velocityVector = m_point3Pos - m_point3OldPos;

	if (!m_bPullY)
	{
		m_point3Pos = m_point3Pos + (m_point3OffsetByGravity * m_fTimeStep +
			(1.0 - damping) * velocityVector);
	}
	else
	{
		m_point3Pos =  m_point3Pos + (pullForce * m_fTimeStep +
			m_point3OffsetByGravity * m_fTimeStep +
			(1.0 - damping) * velocityVector);

		m_point3Pos.setY(m_pointY + height* 1.6 * frame);
		if (m_bPullZ && frame > 0.1)
		{
			m_point3Pos.setZ(m_pointZ + 10 * (frame - 0.1));
		}
	
	}
	m_point3OldPos = tmp;
}
void KMassPoint::setNormal(const QVector3D& normal)
{
	m_point3Normal = normal;
}
QVector3D KMassPoint::getNormal()const
{
	return m_point3Normal;
}
void KMassPoint::setPullPointXYZ(bool enableX, bool enableY, bool enableZ)
{
	m_bPullX = enableX;
	m_bPullY = enableY;
	m_bPullZ = enableZ;
}
