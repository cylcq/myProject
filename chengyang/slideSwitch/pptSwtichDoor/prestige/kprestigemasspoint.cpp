#include "prestige/kprestigemasspoint.h"
#include <QtDebug>

const float G_FDEFAULT_DAMPING = -2.5f;
KPrestigeMassPoint::KPrestigeMassPoint()
{

}
KPrestigeMassPoint::KPrestigeMassPoint(QVector3D pos)
	:KMassPoint(pos)
{
	
}

KPrestigeMassPoint::~KPrestigeMassPoint()
{

}

void KPrestigeMassPoint::setPointGravityOffset(const QVector3D& gravity)
{
	if (!m_enumPullY)
	{
		m_point3Force = QVector3D(0, 0, 0);
		m_point3Force += gravity;

		m_point3Force += (G_FDEFAULT_DAMPING * m_point3Velocity);
	}
}
void KPrestigeMassPoint::setPointSpringOffset(const QVector3D& springforce)
{
	if (!m_enumPullY)
	{
		m_point3Pos += springforce;
	}
}
void KPrestigeMassPoint::timeStep(float width, float height, float frame, float deltaTime)
{
	float damping = 0;	//阻尼力,可以模拟布料的材质
	QVector3D tmp = m_point3Pos;
	//m_point3Velocity = (m_point3Pos - m_point3OldPos) / deltaTime;
	m_point3Velocity = (m_point3Force * deltaTime / m_fMass);
	m_point3Pos += ((m_point3Velocity * (1 - damping)) * deltaTime);
	if (!m_enumPullY)
	{
		if (frame > 0.1 && m_enumPullZ)
		{
			if (m_point3Pos.z() < m_point3OldPos.z() + 0.1)
			{
				m_point3Pos.setZ(m_point3OldPos.z() + 0.1);
  				if (m_point3Pos.z() > 8)
  				{
  					m_point3Pos.setZ(8);
  				}
			}
		}
	}
	else
	{
		if (frame > 0.1)
		{
			m_point3Pos.setY(m_point3Begin.y() + (height * 1.7) * (frame - 0.1));
			m_point3Pos.setX(m_point3Begin.x());
			m_point3Pos.setZ(0);
		}
	}
	m_point3OldPos = tmp;
}
