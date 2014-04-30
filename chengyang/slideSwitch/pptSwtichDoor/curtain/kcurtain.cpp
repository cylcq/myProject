#include "kcurtain.h"
#include "curtain\kcurtainmasspoint.h"
#include "gl\glut.h"
#include <QtOpenGL>
#include <QtDebug>

const float g_fHalfViewAngle = 30.0;
const float g_fPI = 3.14159;
const float g_fPreImageDepthZ = 10.0;
const float g_fClothMoveBeginTime = 0.1;

KCurtain::KCurtain()
	: m_fWidth(0)
	, m_fHeight(0)
	, m_fOldFrame(g_fClothMoveBeginTime)
{
	m_Gravity = QVector3D(0, -0.98, 0);
}

KCurtain::~KCurtain()
{
}

void KCurtain::initial(const float& width, const float& height, const QImage& preImage, const QImage& curImage)
{ 
	m_fHeight = 2 * tan(g_fHalfViewAngle * g_fPI / 180.0) * g_fPreImageDepthZ;	//布料高度
	m_fWidth = (width / height) * m_fHeight;			//布料宽度

	float massCountRate = 2;//质点的数量为m_fWidth*massCountRate,m_fHeight*massCountRate;
	float koc = 0.25;//弹性系数
	float damping = 0.0;//阻尼系数

	m_leftCloth = KCloth(m_fWidth / 2.0, m_fHeight, massCountRate);
	m_leftCloth.addMass(SWITCHSTYLE_CURTAIN);
	m_leftCloth.addSpring(0, 0, m_leftCloth.getHorizontalSpringCount(),m_leftCloth.getVerticalSpringCount());
	m_leftCloth.setSpringNumBetween2Point(3);//两个质点之间弹簧的数量
	m_leftCloth.setSpringKoc_Damping(koc,damping);

	m_rightCloth = KCloth(m_fWidth / 2.0, m_fHeight, massCountRate);
	m_rightCloth.addMass(SWITCHSTYLE_CURTAIN);
	m_rightCloth.addSpring(0, 0, m_rightCloth.getHorizontalSpringCount(),m_rightCloth.getVerticalSpringCount());
	m_rightCloth.setSpringNumBetween2Point(1);//两个质点之间弹簧的数量
	m_rightCloth.setSpringKoc_Damping(koc,damping);

	setSpecialPosition();//设置特殊点
	loadTexture(preImage,curImage);

	//光照,未启用光照
	GLfloat lightPos[4] = {m_fWidth / 2, m_fHeight / 2, 10.0f, 0.0f};
	GLfloat lightAmbient[4] = {0.5f, 0.5f, 0.5f, 1.0f};
	GLfloat lightDiffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};

	glLightfv(GL_LIGHT1, GL_POSITION, (GLfloat*)&lightPos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, (GLfloat*) &lightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, (GLfloat*) &lightDiffuse);
	//glEnable(GL_LIGHTING); 
	glEnable(GL_LIGHT1);

	//glPolygonMode(GL_FRONT,GL_LINE);
}

void KCurtain::loadTexture(const QImage& preImage, const QImage& curImage)
{
	glGenTextures(2, (GLuint*)&m_nTexture[0]);//构建纹理

	//当前image的纹理
	QImage curtexture = QGLWidget::convertToGLFormat(curImage);
	glBindTexture(GL_TEXTURE_2D, m_nTexture[0]);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB,curtexture.width(), curtexture.height(),
		GL_RGBA, GL_UNSIGNED_BYTE, curtexture.bits());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//左边的纹理
	QImage leftImage  = preImage.copy(0, 0, preImage.width() / 2, preImage.height());
	QImage leftTexture = QGLWidget::convertToGLFormat(leftImage);
	glBindTexture(GL_TEXTURE_2D, m_nTexture[1]);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB,leftTexture.width(), leftTexture.height(),
		GL_RGBA, GL_UNSIGNED_BYTE, leftTexture.bits());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//右边的纹理
	QImage rightImage  = preImage.copy(preImage.width() / 2, 0, preImage.width() / 2,preImage.height());
	QImage rightTexture = QGLWidget::convertToGLFormat(rightImage);
	glBindTexture(GL_TEXTURE_2D, m_nTexture[2]);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB,rightTexture.width(), rightTexture.height(),
		GL_RGBA, GL_UNSIGNED_BYTE, rightTexture.bits());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void KCurtain::setSpecialPosition()
{
	for (int i = 0; i < m_leftCloth.getHorizontalSpringCount();)
	{
		m_leftCloth.getMassPoint(0, i).setPullPointXYZ(NEGATIVEMOVE, NOMOVE, NOMOVE);
		i += 1;
	}
	for (int i = 0; i < m_rightCloth.getHorizontalSpringCount();)
	{
		m_rightCloth.getMassPoint(0, i).setPullPointXYZ(POSITIVEMOVE, NOMOVE, NOMOVE);
		i += 1;
	}
	
}

void KCurtain::switchSlide(QPainter& painter, const QImage& preImage, const QImage& curImage, float frame)
{
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawCurImage(curImage);
	drawPreImage(preImage,frame);
}

void KCurtain::drawCurImage(const QImage& curImage)
{
	glPushMatrix();
	glLoadIdentity();

	float backDepth = 50.0f;
	float height = 2 * tan(g_fHalfViewAngle * g_fPI / 180.0) * backDepth;	//布料高度
	float width = (m_fWidth / m_fHeight) * height;			//布料宽度

	glTranslatef(-width / 2.0f, -height /  2.0f, -backDepth);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_nTexture[0]);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);glVertex3f(0.0f, height, 0.0f);
	glTexCoord2f(0, 0);glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1, 0);glVertex3f(width, 0.0f, 0.0f);
	glTexCoord2f(1, 1);glVertex3f(width, height, 0.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void KCurtain::drawPreImage(const QImage& preImage, float frame)
{
	qDebug()<<frame;
	//绘制左边
	drawCloth(m_leftCloth, 1, frame, -m_fWidth / 2.0f, -m_fHeight /  2.0f);
	//右边
	drawCloth(m_rightCloth, 2, frame, 0.0f, -m_fHeight /  2.0f);
	m_fOldFrame = frame;
}

void KCurtain::drawCloth(KCloth& cloth, int textureAddr, float frame, float coordX, float coordY)
{
	float timestep = 0;

	cloth.addPointGravity(m_Gravity);
	timestep = frame - m_fOldFrame;

	//根据步长计算质点位置的变化
	cloth.timeStepConsiderVelocity(frame, timestep);
	//cloth.timeStep(frame,timestep);
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(coordX, coordY, -g_fPreImageDepthZ);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_nTexture[textureAddr]);

	cloth.drawCloth(0, 0, cloth.getHorizontalSpringCount(), cloth.getVerticalSpringCount());
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
