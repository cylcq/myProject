#ifndef CLOTH_H
#define CLOTH_H

#include <QVector>
#include "massPoint.h"
#include "springConstraint.h"

class cloth
{
public:
	cloth(float width, float height);

     QVector<massPoint> massPoints; //array of every mass point
     QVector<springConstraint> springConstraints; //array of every springConstraint

     massPoint* getmassPoint(int x, int y);

     QVector3D getTriNormals(massPoint *p1,massPoint *p2,massPoint *p3);
     void triWindForce(massPoint *p1,massPoint *p2,massPoint *p3, const QVector3D direction);
     void drawTriangle(massPoint *p1, massPoint *p2, massPoint *p3, const QVector<QVector2D>&);
     void drawCloth();
     void timeStep();
     void addPointGravity(const QVector3D direction);
     void windForce(const QVector3D direction);
     void doFrame();
     void ballCollision(const QVector3D center,const float radius );
     
};
#endif // CLOTH_H
