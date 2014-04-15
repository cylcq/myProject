#ifndef __PPTSWITCHDOOR_KSPRINGMODEL_H__
#define __PPTSWITCHDOOR_KSPRINGMODEL_H__

#include "kmasspoint.h"

class KSpringModel
{
public:
	KSpringModel();
	KSpringModel(KMassPoint *p1,KMassPoint *p2);
	virtual ~KSpringModel();

public:
	void setMassPointFirst(KMassPoint *masspoint);
	KMassPoint* getMassPointFirst();
	void setMassPointSecond(KMassPoint *masspoint);
	KMassPoint* getMassPointSecond();
	void setLength(const float& length);
	float getLength();
	void setModelus(const float& k);
	float getModulus();
	void setDamping(const float& damping);
	float getDamping();
	void calculateSpringForce();

private:
	KMassPoint *m_massPointFirst;
	KMassPoint *m_massPointSecond;
	float m_fOriginLength;
	float m_fModulus;//µ¯ÐÔÏµÊý
};
#endif;	//__PPTSWITCHDOOR_KSPRINGMODEL_H__

