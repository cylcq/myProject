#ifndef __PPTSWITCHDOOR_KCLOTH_H__
#define __PPTSWITCHDOOR_KCLOTH_H__

#include "kspringmodel.h"
#include <QVector>

class KCloth
{
public:
	KCloth();
	KCloth(float width, float height);
	virtual ~KCloth();

public:
	KMassPoint& getMassPoint(int i, int j);
	QVector3D getTriNormal(const KMassPoint &p1, const KMassPoint &p2, const KMassPoint &p3);
	void drawTriangle(const KMassPoint &p1, const KMassPoint &p2, const KMassPoint &p3,const QVector<QVector2D>& texCoordinate);
	void drawCloth();
	void timeStep(float frame = 1.0);
	void addPointGravity(const QVector3D &direction);
	int getVerticalSpringCount();
	int getHorizontalSpringCount();
	void resetMassPointPosition();
private:
	float	m_fWidth;
	float   m_fHeight;
	int		m_nVerticalSpringCount;
	int		m_nHorizontalSpringCount;
	QVector<KMassPoint> m_vecMassPoints;
	QVector<KSpringModel> m_vecSpring;
};

#endif;	//__PPTSWITCHDOOR_KCLOTH_H__