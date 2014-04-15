#include "kprestige.h"
#include "gl\glut.h"
#include <QtOpenGL>

static const float g_fHalfViewAngle = 30.0;
static const float g_fPI = 3.14159;
static const float g_fPreimageDepthZ = 10.0;
KPrestige::KPrestige()
	: m_fWidth(0)
	, m_fHeight(0)
{
	m_Gravity = QVector3D(0, -20, 0);
}

KPrestige::~KPrestige()
{

}

void KPrestige::initial(const float& width, const float& height, const QImage& preImage, const QImage& curImage)
{
	m_fHeight = 2 * tan(g_fHalfViewAngle * g_fPI / 180.0) * g_fPreimageDepthZ;	//布料高度
	m_fWidth = (width / height) * m_fHeight;			//布料宽度
	
// 	m_fHeight = 8;
// 	m_fWidth = (width / height) * m_fHeight;
	
	m_Cloth = KCloth(m_fWidth, m_fHeight);
	setFixPosition();
	loadTexture(preImage,curImage);
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
void KPrestige::setFixPosition()
{
	//根据四点定位,模拟Prestige的效果
	int horizontalSpringCount = m_Cloth.getHorizontalSpringCount();
	int verticalSpringCount = m_Cloth.getVerticalSpringCount();
	for (int i = 0; i < verticalSpringCount - 1; ++i)
	{
		float offset = 0.42;
		for (int j = 0; j < 5; ++j)
		{
			m_Cloth.getMassPoint(i, horizontalSpringCount * offset).setPullPointXYZ(false,true);
			offset += 0.01;
		}
	}
	
	//m_Cloth.getMassPoint(verticalSpringCount * 0.65, horizontalSpringCount * 0.3).setPullPointXYZ(true);
	//m_Cloth.getMassPoint(verticalSpringCount * 0.65, horizontalSpringCount * 0.6).setPullPointXYZ(true);
	//m_Cloth.getMassPoint(verticalSpringCount - 1, horizontalSpringCount * 0.5).setPullPointZ();
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
	if (frame > 0.1 && frame < 0.7)
	{
		int horizontalSpringCount = m_Cloth.getHorizontalSpringCount();
		int verticalSpringCount = m_Cloth.getVerticalSpringCount();
		m_Cloth.getMassPoint(verticalSpringCount * 0.45, horizontalSpringCount * 0.5 *(1 - frame)).setPullPointXYZ(false, true, true);
		m_Cloth.getMassPoint(verticalSpringCount * 0.45, horizontalSpringCount * 0.45).setPullPointXYZ(false, true, true);
		if (1 - frame > 0)
		{
			m_Cloth.getMassPoint(verticalSpringCount * 0.5, horizontalSpringCount * 0.5 * (1 - frame) - 1).setPullPointXYZ(false, false, false);
		}
	}
	if (frame < 0.1)
	{
		m_Cloth.addPointGravity(m_Gravity * frame / 0.1);
	}
	else
	{
		m_Cloth.addPointGravity(m_Gravity );
	}
	
	m_Cloth.timeStep(frame);

	glPushMatrix();
	glLoadIdentity();

	glTranslatef(-m_fWidth / 2.0f, -m_fHeight /  2.0f, -g_fPreimageDepthZ);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_nTexture[0]);
	m_Cloth.drawCloth();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}