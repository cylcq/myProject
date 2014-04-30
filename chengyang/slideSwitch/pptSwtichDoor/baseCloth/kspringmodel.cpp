#include "kspringmodel.h"
#include <QtDebug>

KSpringModel::KSpringModel()
{

}
KSpringModel::KSpringModel(KMassPoint *p1,KMassPoint *p2)
	: m_fOriginLength(0.0)
	, m_fKoc(0.95f)
	, m_fDamping(-0.75f)
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
void KSpringModel::calculateSpringForceConsiderVelocity()
{
	QVector3D deltaP = m_massPointSecond->getPos() - m_massPointFirst->getPos();
 	QVector3D deltaV = m_massPointSecond->getVelocity() - m_massPointFirst->getVelocity();
 
 	float currentLength = deltaP.length();
 
 	float leftTerm = -m_fKoc * (currentLength - m_fOriginLength);
 	float rightTerm = m_fDamping * (QVector3D::dotProduct(deltaV, deltaP) / currentLength);
 
 	QVector3D springForce = (leftTerm + rightTerm) * deltaP.normalized();
 
 	m_massPointFirst->addForce(springForce);
 	m_massPointSecond->addForce(-springForce);
}

void KSpringModel::velocityConstraint()
{
	QVector3D deltaP = m_massPointSecond->getPos() - m_massPointFirst->getPos();
	float currentLength = deltaP.length();
	if (currentLength > m_fOriginLength)
	{
		currentLength -= m_fOriginLength;
		currentLength /= 2.0f;
		
		deltaP = deltaP.normalized();
		deltaP *= currentLength;
		m_massPointFirst->addVelocity(deltaP);
		m_massPointSecond->addVelocity(-deltaP);
	}
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

void KSpringModel::setKoc(const float& k)
{
	m_fKoc = k;
}

float KSpringModel::getKoc()
{
	return m_fKoc;
}

void KSpringModel::setDamping(const float& damping)
{
	m_fDamping = damping;
}

float KSpringModel::getDamping()
{
	return m_fDamping;
}