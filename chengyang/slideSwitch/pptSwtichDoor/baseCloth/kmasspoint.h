#ifndef __PPTSWITCHDOOR_KMASSPOINT_H__
#define __PPTSWITCHDOOR_KMASSPOINT_H__

#include <QVector3D>

const enum PULLPOINTDIRECTION
{
	NEGATIVEMOVE = -1,	//向左,向下,向里为负
	NOMOVE,				//不进行特殊修改
	POSITIVEMOVE		//向右,向上,向外为正
};

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
	void setNormal(const QVector3D& normal);
	QVector3D getNormal()const;
	QVector3D& getVelocity();
	QVector3D& getForce();
	PULLPOINTDIRECTION getPullY();
	void setPullPointXYZ(PULLPOINTDIRECTION x, PULLPOINTDIRECTION y, PULLPOINTDIRECTION z);
	
	virtual void addForce(const QVector3D& deltaForce);
	virtual void addVelocity(const QVector3D &deltaV);
	virtual void setPointGravityOffset(const QVector3D& gravity);
	virtual void setPointSpringOffset(const QVector3D& springforce);
	virtual void timeStep(float width, float height, float frame, float timestep);

protected:
	QVector3D m_point3Pos;
	QVector3D m_point3OldPos;
	float m_fMass;
	QVector3D m_point3Velocity;
	QVector3D m_point3Force;
	QVector3D m_point3Begin;
	PULLPOINTDIRECTION m_enumPullX;
	PULLPOINTDIRECTION m_enumPullY;
	PULLPOINTDIRECTION m_enumPullZ;

private:
	QVector3D m_point3Normal;
	QVector3D m_point3OffsetByGravity;
	QVector3D m_point3OffsetBySpring;
};

#endif // __PPTSWITCHDOOR_KMASSPOINT_H__
