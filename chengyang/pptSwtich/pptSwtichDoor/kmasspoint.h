#ifndef __PPTSWITCHDOOR_KMASSPOINT_H__
#define __PPTSWITCHDOOR_KMASSPOINT_H__

#include <QVector3D>

class KMassPoint
{
public:
	KMassPoint();
	KMassPoint(QVector3D& pos);
	~KMassPoint();

public:
	void setPos(const QVector3D& pos);
	QVector3D getPos()const;
	QVector3D getOldPos()const;
	void setPointGravityOffset(const QVector3D& gravity);
	void setPointSpringOffset(const QVector3D& springforce);
	void timeStep(float height, float timesetp);
	void setNormal(const QVector3D& normal);
	QVector3D getNormal()const;
	void setPullPointXYZ(bool enableX = false, bool enableY = false, bool enableZ = false);
private:
	QVector3D m_point3Pos;
	QVector3D m_point3OldPos;
	QVector3D m_point3Normal;
	QVector3D m_point3OffsetByGravity;
	QVector3D m_point3OffsetBySpring;
	bool m_bPullX;
	bool m_bPullY;
	bool m_bPullZ;
	int m_nMass;
	float m_fTimeStep;
	float m_pointY;
	float m_pointZ;
};

#endif // __PPTSWITCHDOOR_KMASSPOINT_H__
