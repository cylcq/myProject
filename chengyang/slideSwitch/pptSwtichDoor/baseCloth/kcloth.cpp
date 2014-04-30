
#include "kcloth.h"
#include "prestige/kprestigemasspoint.h"
#include "curtain/kcurtainmasspoint.h"
#include <QVector2D>
#include <QVector3D>
#include <QtOpenGL>
#include <QtDebug>

KCloth::KCloth()
	: m_fWidth(0)
	, m_fHeight(0)
	, m_nVerticalSpringCount(0)
	, m_nHorizontalSpringCount(0)
{
	
}

KCloth::~KCloth()
{
	
}

KCloth::KCloth(float w, float h, float massCountRate)
	: m_fWidth(w)
	, m_fHeight(h)
	, m_nVerticalSpringCount((int)(h * massCountRate))
	, m_nHorizontalSpringCount((int)(w * massCountRate))
	, m_nSpringNumBetween2Point(1)
{
	initMapCreateMassPointFun();
}

void KCloth::addMass(SWITCHSTYLE style)
{
	for (int i = m_nVerticalSpringCount - 1; i > -1; --i)
	{
		for (int j = 0; j < m_nHorizontalSpringCount; ++j)
		{
			QVector3D pos(m_fWidth * (j / (float)(m_nHorizontalSpringCount - 1)),
				m_fHeight * (i / (float)(m_nVerticalSpringCount - 1)), 0);

			QSharedPointer<KMassPoint> point = (this->*m_mapCreateMassPointFun[style])(pos);

			m_vecMassPoints.push_back(point);
		}
	}
}

void KCloth::addSpring(int x, int y, int offsetToX, int offsetToY)
{
	// add spring
	for (int i = y; i < offsetToY; ++i)
	{
		for (int j = x; j < offsetToX; ++j)
		{
			//add structural spring
			if (j < offsetToX - 1)
			{
				m_vecSpring.push_back(KSpringModel(&getMassPoint(i, j), &getMassPoint(i, j +1)));
			}
			if (i < offsetToY - 1)
			{
				m_vecSpring.push_back(KSpringModel(&getMassPoint(i,j), &getMassPoint(i + 1, j)));
			}

			//add bend spring
			if ((i < offsetToY - 1) && (j < offsetToX - 1))
			{
				m_vecSpring.push_back(KSpringModel(&getMassPoint(i, j),&getMassPoint(i + 1, j + 1)));
				m_vecSpring.push_back(KSpringModel(&getMassPoint(i + 1, j), &getMassPoint(i, j + 1)));
			}

			//add shear spring
			if (j < offsetToX - 2)
			{
				m_vecSpring.push_back(KSpringModel(&getMassPoint(i, j), &getMassPoint(i,j + 2)));
			}
			if (i < offsetToY - 2)
			{
				m_vecSpring.push_back(KSpringModel(&getMassPoint(i, j), &getMassPoint(i + 2, j)));
			}
			if ((i < offsetToY - 2) && (j < offsetToX - 2))
			{
				m_vecSpring.push_back(KSpringModel(&getMassPoint(i, j),&getMassPoint(i + 2, j + 2)));
				m_vecSpring.push_back(KSpringModel(&getMassPoint(i + 2, j), &getMassPoint(i, j + 2)));
			}
		}
	}//end of add spring
}

KMassPoint& KCloth::getMassPoint(int i, int j)
{
	return *m_vecMassPoints[i * (m_nHorizontalSpringCount) + j];
}

QVector3D KCloth::getTriNormal(const KMassPoint &p1, const KMassPoint &p2, const KMassPoint &p3)
{
	QVector3D pos1 = p1.getPos();
	QVector3D pos2 = p2.getPos();
	QVector3D pos3 = p3.getPos();

	QVector3D v1 = pos2 - pos1;
	QVector3D v2 = pos3 - pos1;

	//v1,v2叉乘,其结果等于v1,v2所定义平面的法线向量
	return QVector3D::crossProduct(v1, v2);
}

void KCloth::drawCloth(int xb, int yb, int offsetToX, int offsetToY)
{
	for (int x = yb; x < offsetToY - 1; ++x)
	{
		for (int y =xb; y < offsetToX - 1; ++y)
		{
			QVector3D normal = getTriNormal(getMassPoint(x + 1, y), getMassPoint(x, y),
				getMassPoint(x, y + 1));
			getMassPoint(x + 1, y).setNormal(normal);
			getMassPoint(x, y).setNormal(normal);
			getMassPoint(x, y + 1).setNormal(normal);

			normal = getTriNormal(getMassPoint(x + 1, y + 1), getMassPoint(x + 1, y), getMassPoint(x, y + 1));
			getMassPoint(x + 1, y + 1).setNormal(normal);
		}
	}
	//draw Triangle
	glBegin(GL_TRIANGLES);
		for (int x = yb; x < offsetToY - 1; ++x)
		{
			for (int y = xb; y < offsetToX - 1; ++y)
			{
				float texture_x = float(y) / (m_nHorizontalSpringCount);
				float texture_y = float(m_nVerticalSpringCount- x) / (m_nVerticalSpringCount);

				float texture_xb = float(y + 1) / (m_nHorizontalSpringCount);
				float texture_yb = float(m_nVerticalSpringCount - x - 1) / (m_nVerticalSpringCount);

				//取纹理坐标
				QVector2D leftTop(texture_x, texture_y);
				QVector2D leftBottom(texture_x, texture_yb);
				QVector2D rightTop(texture_xb, texture_y);
				QVector2D rightBottom(texture_xb, texture_yb);

				QVector<QVector2D> vec;
				
				vec.push_back(leftBottom);
				vec.push_back(rightTop);
				vec.push_back(leftTop);
				drawTriangle(getMassPoint(x + 1, y),getMassPoint(x, y + 1),getMassPoint(x, y),vec);

				vec.clear();
				vec.push_back(leftBottom);
				vec.push_back(rightBottom);
				vec.push_back(rightTop);
				drawTriangle(getMassPoint(x + 1, y), getMassPoint(x + 1, y + 1), getMassPoint(x, y + 1),vec);
			}
		}
	glEnd();

	//resetMassPointPosition();
}

void KCloth::drawTriangle(const KMassPoint &p1, const KMassPoint &p2, 
	const KMassPoint &p3,const QVector<QVector2D>& texCoordinate)
{
	glNormal3fv((GLfloat*)&(p1.getNormal().normalized()));
	glTexCoord2fv((GLfloat*)&texCoordinate[0]);
	glVertex3fv((GLfloat*)&(p1.getPos()));

	glNormal3fv((GLfloat *)&(p2.getNormal().normalized()));
	glTexCoord2fv((GLfloat*)&texCoordinate[1]);
	glVertex3fv((GLfloat *)&(p2.getPos()));

	glNormal3fv((GLfloat *)&(p3.getNormal().normalized()));
	glTexCoord2fv((GLfloat*)&texCoordinate[2]);
	glVertex3fv((GLfloat *)&(p3.getPos()));
}

void KCloth::resetMassPointPosition()
{
	for (int i = m_nVerticalSpringCount - 1; i > -1; --i)
	{
		for (int j = 0; j < m_nHorizontalSpringCount; ++j)
		{
			QVector3D oldPos = getMassPoint(i, j).getOldPos();
			getMassPoint(i, j).setPos(oldPos);
		}
	}
}

void KCloth::timeStep(float frame, float timestep)
{
	int springNumBetween2Point = m_nSpringNumBetween2Point;//在两个质点之间弹簧数量
	while (springNumBetween2Point)
	{
		for (int i = 0; i < m_vecSpring.size(); ++i)
		{
			m_vecSpring[i].calculateSpringForce();
		}
		--springNumBetween2Point;
	}

	for (int i = 0; i < m_vecMassPoints.size(); ++i)
	{
		m_vecMassPoints[i]->timeStep(m_fWidth, m_fHeight, frame, timestep);
	}
}
void KCloth::timeStepConsiderVelocity(float frame, float timestep)
{
	int springNumBetween2Point = m_nSpringNumBetween2Point;//在两个质点之间弹簧数量
	while (springNumBetween2Point)
	{
		for (int i = 0; i < m_vecSpring.size(); ++i)
		{
			m_vecSpring[i].calculateSpringForceConsiderVelocity();
		}
		--springNumBetween2Point;
	}

	for (int i = 0; i < m_vecMassPoints.size(); ++i)
	{
		m_vecMassPoints[i]->timeStep(m_fWidth, m_fHeight, frame, timestep);
	}

	for (int i = 0; i < m_vecSpring.size(); ++i)
	{
	 	m_vecSpring[i].velocityConstraint();//速度约束
	}
}

void KCloth::setSpringKoc_Damping(float koc, float damping)
{
	for (int i = 0; i < m_vecSpring.count(); ++i)
	{
		m_vecSpring[i].setKoc(koc);
		m_vecSpring[i].setDamping(damping);
	}
}

void KCloth::addPointGravity(const QVector3D &gravity)
{
	for(int i = 0; i<m_vecMassPoints.size(); i++)
	{
		m_vecMassPoints[i]->setPointGravityOffset(gravity);
	}
}

int KCloth::getHorizontalSpringCount()
{
	return m_nHorizontalSpringCount;
}

int KCloth::getVerticalSpringCount()
{
	return m_nVerticalSpringCount;
}

void KCloth::setSpringNumBetween2Point(int num)
{
	m_nSpringNumBetween2Point = num;
}

void KCloth::initMapCreateMassPointFun()
{
	m_mapCreateMassPointFun[SWITCHSTYLE_PRESTIGE] = &KCloth::createPrestigeMass;
	m_mapCreateMassPointFun[SWITCHSTYLE_CURTAIN] = &KCloth::createCurtainMass;
}
QSharedPointer<KMassPoint> KCloth::createPrestigeMass(QVector3D pos)
{
	QSharedPointer<KMassPoint> masspoint =
		QSharedPointer<KMassPoint>(new KPrestigeMassPoint(pos));
	return masspoint;
}
QSharedPointer<KMassPoint> KCloth::createCurtainMass(QVector3D pos)
{
	QSharedPointer<KMassPoint> masspoint =
		QSharedPointer<KMassPoint>(new KCurtainMassPoint(pos));
	return masspoint;
}