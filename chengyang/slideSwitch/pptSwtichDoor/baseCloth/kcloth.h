#ifndef __PPTSWITCHDOOR_KCLOTH_H__
#define __PPTSWITCHDOOR_KCLOTH_H__

#include "kswitchstyle.h"
#include "kspringmodel.h"
#include "kbaseswitch.h"
#include <QVector>
#include <QSharedPointer>
#include <map>


class KCloth
{
public:
	KCloth();
	KCloth(float width, float height,float massCountRate);
	virtual ~KCloth();
	typedef QSharedPointer<KMassPoint> (KCloth::*CreateMassFun)(QVector3D pos);

public:
	void addMass(SWITCHSTYLE style);
	void addSpring(int x, int y, int offsetToX, int offsetToY);
	KMassPoint& getMassPoint(int i, int j);
	QVector3D getTriNormal(const KMassPoint &p1, const KMassPoint &p2, const KMassPoint &p3);
	void drawTriangle(const KMassPoint &p1, const KMassPoint &p2, const KMassPoint &p3,const QVector<QVector2D>& texCoordinate);
	void drawCloth(int x, int y, int offsetToX, int offsetToY);
	void timeStep(float frame, float timestep);
	void timeStepConsiderVelocity(float frame, float timestep);
	void addPointGravity(const QVector3D &direction);
	int getVerticalSpringCount();
	int getHorizontalSpringCount();
	void resetMassPointPosition();
	void setSpringNumBetween2Point(int num);
	void setSpringKoc_Damping(float koc, float damping);
	void initMapCreateMassPointFun();
	QSharedPointer<KMassPoint> createPrestigeMass(QVector3D pos);
	QSharedPointer<KMassPoint> createCurtainMass(QVector3D pos);

private:
	float	m_fWidth;
	float   m_fHeight;
	int		m_nVerticalSpringCount;
	int		m_nHorizontalSpringCount;
	int		m_nSpringNumBetween2Point;
	QVector<QSharedPointer<KMassPoint>> m_vecMassPoints;
	QVector<KSpringModel> m_vecSpring;
	std::map<SWITCHSTYLE, CreateMassFun> m_mapCreateMassPointFun;
};

#endif;	//__PPTSWITCHDOOR_KCLOTH_H__