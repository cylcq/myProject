
#include "kcloth.h"
#include <QVector2D>
#include <QVector3D>
#include <QtOpenGL>

KCloth::KCloth()
{
}

KCloth::~KCloth()
{
}

KCloth::KCloth(float w, float h)
	: m_fWidth(w)
	, m_fHeight(h)
	, m_nVerticalSpringCount((int)(h))
	, m_nHorizontalSpringCount((int)(w))
	//, m_fOldframe(0)
{
	for (int i = m_nVerticalSpringCount - 1; i > -1; --i)
	{
		for (int j = 0; j < m_nHorizontalSpringCount; ++j)
		{
			QVector3D pos(m_fWidth * (j / (float)(m_nHorizontalSpringCount - 1)),
				m_fHeight * (i / (float)(m_nVerticalSpringCount - 1)), 0);
			m_vecMassPoints.push_back(KMassPoint(pos));
		}
	}
	// add spring
	for (int i = 0; i < m_nVerticalSpringCount; ++i)
	{
		for (int j = 0; j < m_nHorizontalSpringCount; ++j)
		{
			//add structural spring
			if (j < m_nHorizontalSpringCount - 1)
			{
				m_vecSpring.push_back(KSpringModel(&getMassPoint(i, j), &getMassPoint(i, j +1)));
			}
			if (i < m_nVerticalSpringCount - 1)
			{
				m_vecSpring.push_back(KSpringModel(&getMassPoint(i,j), &getMassPoint(i + 1, j)));
			}

			//add bend spring
			if ((i < m_nVerticalSpringCount - 1) && (j < m_nHorizontalSpringCount - 1))
			{
				m_vecSpring.push_back(KSpringModel(&getMassPoint(i, j),&getMassPoint(i + 1, j + 1)));
				m_vecSpring.push_back(KSpringModel(&getMassPoint(i, j + 1),&getMassPoint(i + 1, j)));
			}

			//add shear spring
			if (j < m_nHorizontalSpringCount - 2)
			{
				m_vecSpring.push_back(KSpringModel(&getMassPoint(i, j), &getMassPoint(i,j + 2)));
			}
			if (i < m_nVerticalSpringCount - 2)
			{
				m_vecSpring.push_back(KSpringModel(&getMassPoint(i, j), &getMassPoint(i + 2, j)));
			}
			if ((i < m_nVerticalSpringCount - 2) && (j < m_nHorizontalSpringCount - 2))
			{
				m_vecSpring.push_back(KSpringModel(&getMassPoint(i, j),&getMassPoint(i + 2, j + 2)));
				m_vecSpring.push_back(KSpringModel(&getMassPoint(i, j + 2),&getMassPoint(i + 2, j)));
			}
		}
	}//end of add spring
}

KMassPoint& KCloth::getMassPoint(int i, int j)
{
	return m_vecMassPoints[i * m_nHorizontalSpringCount + j];
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

void KCloth::drawCloth()
{
	for (int x = 0; x < m_nVerticalSpringCount - 1; ++x)
	{
		for (int y =0; y < m_nHorizontalSpringCount - 1; ++y)
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
		for (int x = 0; x < m_nVerticalSpringCount - 1; ++x)
		{
			for (int y = 0; y < m_nHorizontalSpringCount - 1; ++y)
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
void KCloth::timeStep(float frame)
{
	for (int i = 0; i < m_vecSpring.size(); ++i)
	{
		m_vecSpring[i].calculateSpringForce();
	}
	for (int i = 0; i < m_vecMassPoints.size(); ++i)
	{
		m_vecMassPoints[i].timeStep(m_fHeight, frame);
	}
}
void KCloth::addPointGravity(const QVector3D &gravity)
{
	for(int i = 0; i<m_vecMassPoints.size(); i++)
	{
		m_vecMassPoints[i].setPointGravityOffset(gravity);
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