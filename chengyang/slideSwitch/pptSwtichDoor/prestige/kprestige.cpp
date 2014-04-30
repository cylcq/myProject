#include "kprestige.h"
#include "prestige/kprestigemasspoint.h"
#include "gl\glut.h"
#include <QtOpenGL>

const float g_fHalfViewAngle = 30.0;
const float g_fPI = 3.14159;
const float g_fPreImageDepthZ = 20.0;
const float g_fClothMoveBeginTime = 0.1;

KPrestige::KPrestige()
	: m_fWidth(0)
	, m_fHeight(0)
	, m_fOldFrame(g_fClothMoveBeginTime)
{
	m_Gravity = QVector3D(0, -9.8, 0);
}

KPrestige::~KPrestige()
{

}

void KPrestige::initial(const float& width, const float& height, const QImage& preImage, const QImage& curImage)
{
	m_fHeight = 2 * tan(g_fHalfViewAngle * g_fPI / 180.0) * g_fPreImageDepthZ;	//布料高度
	m_fWidth = (width / height) * m_fHeight;			//布料宽度
	
	float massCountRate = 1.3;//质点的数量为m_fWidth*massCountRate,m_fHeight*massCountRate;

	m_Cloth = KCloth(m_fWidth, m_fHeight, massCountRate);
	m_Cloth.addMass(SWITCHSTYLE_PRESTIGE);
	m_Cloth.addSpring(0, 0, m_fWidth * massCountRate, m_fHeight * massCountRate);
	m_Cloth.setSpringNumBetween2Point(3);//两个质点见弹簧的数量


	setSpecialPosition();
	loadTexture(preImage,curImage);

	GLfloat lightPos[4] = {m_fWidth, m_fHeight /  2.0f, -100.0f, 1.0f};
	GLfloat lightAmbient[4] = {0.5f, 0.5f, 0.5f, 1.0f};
	GLfloat lightDiffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};

	glLightfv(GL_LIGHT1, GL_POSITION, (GLfloat*)&lightPos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, (GLfloat*) &lightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, (GLfloat*) &lightDiffuse);
	//glEnable(GL_LIGHTING); 
	//glEnable(GL_LIGHT1);
}

void KPrestige::loadTexture(const QImage& preImage, const QImage& curImage)
{
	glGenTextures(2, (GLuint*)&m_nTexture[0]);//构建纹理

	QImage texture = QGLWidget::convertToGLFormat(preImage);
	glBindTexture(GL_TEXTURE_2D, m_nTexture[0]);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB,texture.width(), texture.height(),
		GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	QImage curtexture = QGLWidget::convertToGLFormat(curImage);
	glBindTexture(GL_TEXTURE_2D, m_nTexture[1]);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB,curtexture.width(), curtexture.height(),
		GL_RGBA, GL_UNSIGNED_BYTE, curtexture.bits());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

}

void KPrestige::setSpecialPosition()
{
	//根据多点定位,模拟Prestige的效果
	int horizontalSpringCount = m_Cloth.getHorizontalSpringCount();
	int verticalSpringCount = m_Cloth.getVerticalSpringCount();
	for (int i = 0; i < verticalSpringCount * 0.5; ++i)
	{
		float offset = 0.45;
		for (int j = 0; j < 5; ++j)
		{
			m_Cloth.getMassPoint(i, horizontalSpringCount * offset).setPullPointXYZ(NOMOVE, POSITIVEMOVE, NOMOVE);
			offset += 0.01;
		}
	}
}

void KPrestige::switchSlide(QPainter& painter, const QImage& preImage, const QImage& curImage, float frame)
{
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawCurImage(curImage);
	drawPreImage(preImage,frame);
}

void KPrestige::drawCurImage(const QImage& curImage)
{
	glPushMatrix();
	glLoadIdentity();

	float backDepth = 50.0f;
	float height = 2 * tan(g_fHalfViewAngle * g_fPI / 180.0) * backDepth;	//布料高度
	float width = (m_fWidth / m_fHeight) * height;			//布料宽度

	glTranslatef(-width / 2.0f, -height /  2.0f, -backDepth);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_nTexture[1]);

	glBegin(GL_QUADS);
		glTexCoord2f(0, 1);glVertex3f(0.0f, height, 0.0f);
		glTexCoord2f(0, 0);glVertex3f(0.0f, 0.0f, 0.0f);
		glTexCoord2f(1, 0);glVertex3f(width, 0.0f, 0.0f);
		glTexCoord2f(1, 1);glVertex3f(width, height, 0.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void KPrestige::drawPreImage(const QImage& preImage, float frame)
{
	if (frame > g_fClothMoveBeginTime && frame < 0.7)
	{
		int horizontalSpringCount = m_Cloth.getHorizontalSpringCount();
		int verticalSpringCount = m_Cloth.getVerticalSpringCount();

 		for (int i = (int)(verticalSpringCount * 0.5); i < verticalSpringCount - 1; ++i)
 		{
			float offset = 0.3 *(1 - frame);
 			for (int j = 0; j < 5; ++j)
 			{
 				m_Cloth.getMassPoint(i, horizontalSpringCount * offset).setPullPointXYZ(NOMOVE,NOMOVE,POSITIVEMOVE);
				offset  += 0.01;
 			}
 			
 			m_Cloth.getMassPoint(i, horizontalSpringCount * offset + 1).setPullPointXYZ(NOMOVE, NOMOVE, NOMOVE);
 			
 		}
 		for (int i = (int)(verticalSpringCount * 0.6); i < verticalSpringCount - 1; ++i)
 		{
			float offset = 0.6;
 			for (int j = 0; j < 2; ++j)
 			{
 				m_Cloth.getMassPoint(i, horizontalSpringCount * offset).setPullPointXYZ(NOMOVE,NOMOVE,POSITIVEMOVE);
 				offset += 0.01;
 			}
 		}
	}

	
	float timestep = 0;
	m_Cloth.addPointGravity(m_Gravity);
	timestep = frame - m_fOldFrame;
	m_Cloth.timeStep(frame, timestep);
	m_fOldFrame = frame;

	glPushMatrix();
	glLoadIdentity();

	glTranslatef(-m_fWidth / 2.0f, -m_fHeight /  2.0f, -g_fPreImageDepthZ);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_nTexture[0]);
	m_Cloth.drawCloth(0, 0, m_Cloth.getHorizontalSpringCount(), m_Cloth.getVerticalSpringCount());
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}