#ifndef __PPTSWITCHDOOR_KPRESTIGEMASSPOINT_H__
#define __PPTSWITCHDOOR_KPRESTIGEMASSPOINT_H__

#include "baseCloth/kmasspoint.h"

class KPrestigeMassPoint:public KMassPoint
{
public:
	KPrestigeMassPoint();
	KPrestigeMassPoint(QVector3D pos);
	~KPrestigeMassPoint();

public:
	void setPointGravityOffset(const QVector3D& gravity);
	void setPointSpringOffset(const QVector3D& springforce);
	void timeStep(float width, float height, float frame, float timestep);
};

#endif // __PPTSWITCHDOOR_KPRESTIGEMASSPOINT_H__
