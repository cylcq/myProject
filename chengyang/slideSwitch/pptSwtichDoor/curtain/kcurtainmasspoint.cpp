#include "kcurtainmasspoint.h"

const float G_FDEFAULT_DAMPING = -2.5f;
KCurtainMassPoint::KCurtainMassPoint(QVector3D pos)
	:KMassPoint(pos)
{
}


KCurtainMassPoint::~KCurtainMassPoint(void)
{
}
void KCurtainMassPoint::addForce(const QVector3D& deltaForce)
{
	if (!m_enumPullX)
	{
		m_point3Force += deltaForce;
	}
}
void KCurtainMassPoint::addVelocity(const QVector3D &deltaV)
{
	if (!m_enumPullX)
	{
		m_point3Velocity += deltaV;
	}
}
void KCurtainMassPoint::setPointGravityOffset(const QVector3D& gravity)
{
	if (!m_enumPullX)
	{
		m_point3Force = QVector3D(0, 0, 0);
		m_point3Force += gravity;

		m_point3Force += (G_FDEFAULT_DAMPING * m_point3Velocity);
	}
}

void KCurtainMassPoint::setPointSpringOffset(const QVector3D& springforce)
{
	if (!m_enumPullX)
	{
		m_point3Pos += springforce;
	}
}

void KCurtainMassPoint::timeStep(float width, float height, float frame, float deltaTime)
{
	//deltaTime = 0.02;
	float damping = 0.2;	//阻尼力,可以模拟布料的材质
	QVector3D tmp = m_point3Pos;
	//m_point3Velocity += (m_point3Pos - m_point3OldPos) / deltaTime;
	m_point3Velocity += (m_point3Force * deltaTime / m_fMass);
	m_point3Pos += ((m_point3Velocity * (1 - damping)) * deltaTime);
	
	if (frame < 0.5)
	{
		if (m_enumPullX == NEGATIVEMOVE)
		{
			//向左
			m_point3Pos.setX(m_point3Begin.x() - width * frame);
			m_point3Pos.setY(m_point3Begin.y());
			m_point3Pos.setZ(m_point3Begin.z());
		}
		else if (m_enumPullX == POSITIVEMOVE)
		{
			//向右移动
			m_point3Pos.setX(m_point3Begin.x() + width * frame);
			m_point3Pos.setY(m_point3Begin.y());
			m_point3Pos.setZ(m_point3Begin.z());
		}
	}
	else if (frame > 0.5 && frame < 0.6)
	{
 		if (m_enumPullX == NEGATIVEMOVE)
 		{
 			//向左
 			m_point3Pos = m_point3OldPos;
 		}
 		else if (m_enumPullX == POSITIVEMOVE)
 		{
 			//向右移动
 			m_point3Pos = m_point3OldPos;
 		}
	}
	m_point3OldPos = tmp;
}