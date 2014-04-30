#ifndef __PPTSWITCH_KCURTAINMASSPOINT_H
#define __PPTSWITCH_KCURTAINMASSPOINT_H

#include "basecloth\kmasspoint.h"

class KCurtainMassPoint :public KMassPoint
{
public:
	KCurtainMassPoint(QVector3D pos);
	virtual ~KCurtainMassPoint();

public:
	void addForce(const QVector3D& deltaForce);
	void addVelocity(const QVector3D &deltaV);
	void setPointGravityOffset(const QVector3D& gravity);
	void setPointSpringOffset(const QVector3D& springforce);
	void timeStep(float width, float height, float frame, float deltaTime);
};

#endif; //__PPTSWITCH_KCURTAINMASSPOINT_H
