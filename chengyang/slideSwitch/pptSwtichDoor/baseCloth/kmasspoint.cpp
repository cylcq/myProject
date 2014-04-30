#include "kmasspoint.h"
#include <QtDebug>


KMassPoint::KMassPoint()
{

}
KMassPoint::KMassPoint(QVector3D& pos)
	: m_fMass(1)
	, m_point3Pos(pos)
	, m_point3OldPos(pos)
	, m_point3Begin(pos)
	, m_enumPullX(NOMOVE)
	, m_enumPullY(NOMOVE)
	, m_enumPullZ(NOMOVE)
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
void KMassPoint::addForce(const QVector3D& deltaForce)
{
	m_point3Force += deltaForce;
}
QVector3D& KMassPoint::getForce()
{
	return m_point3Force;
}
void KMassPoint::addVelocity(const QVector3D &deltaV)
{
	m_point3Velocity += deltaV;
}
QVector3D& KMassPoint::getVelocity()
{
	return m_point3Velocity;
}
void KMassPoint::setPointGravityOffset(const QVector3D& gravity)
{

}
void KMassPoint::setPointSpringOffset(const QVector3D& springforce)
{

}
void KMassPoint::timeStep(float width, float height, float frame, float deltaTime)
{
	
}

void KMassPoint::setNormal(const QVector3D& normal)
{
	m_point3Normal = normal;
}
QVector3D KMassPoint::getNormal()const
{
	return m_point3Normal;
}
void KMassPoint::setPullPointXYZ(PULLPOINTDIRECTION x, PULLPOINTDIRECTION y, PULLPOINTDIRECTION z)
{
	m_enumPullX = x;
	m_enumPullY = y;
	m_enumPullZ = z;
}
PULLPOINTDIRECTION KMassPoint::getPullY()
{
	return m_enumPullY;
}
