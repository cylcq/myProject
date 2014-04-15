#include "kspringmodel.h"
#include <QtDebug>

KSpringModel::KSpringModel()
{

}
KSpringModel::KSpringModel(KMassPoint *p1,KMassPoint *p2)
	: m_fOriginLength(0.0)
	, m_fModulus(1.0)
{
	m_massPointFirst = p1;
	m_massPointSecond = p2;

	QVector3D offset = m_massPointFirst->getPos() - m_massPointSecond->getPos();
	m_fOriginLength = offset.length();
}
KSpringModel::~KSpringModel()
{
}

void KSpringModel::calculateSpringForce()
{
	QVector3D offset = m_massPointSecond->getPos() - m_massPointFirst->getPos();
	float currentLength = offset.length();
	//计算弹簧一边的弹力.
	QVector3D constrainVector = offset * (1 - m_fOriginLength / currentLength) / 2;
	m_massPointFirst->setPointSpringOffset(constrainVector);
	m_massPointSecond->setPointSpringOffset(-constrainVector);
}

void KSpringModel::setMassPointFirst(KMassPoint *masspoint)
{
	m_massPointFirst = masspoint;
}
KMassPoint* KSpringModel::getMassPointFirst()
{
	return m_massPointFirst;
}
void KSpringModel::setMassPointSecond(KMassPoint *masspoint)
{
	m_massPointSecond = masspoint;
}
KMassPoint* KSpringModel::getMassPointSecond()
{
	return m_massPointSecond;
}
void KSpringModel::setLength(const float& length)
{
	m_fOriginLength = length;
}
float KSpringModel::getLength()
{
	return m_fOriginLength;
}
void KSpringModel::setModelus(const float& k)
{
	m_fModulus = k;
}
float KSpringModel::getModulus()
{
	return m_fModulus;
}