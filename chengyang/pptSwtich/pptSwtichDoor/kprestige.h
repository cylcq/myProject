#ifndef __PPTSWITCHDOOR_KPRESTIGE_H__
#define __PPTSWITCHDOOR_KPRESTIGE_H__

#include "kcloth.h"
#include "kbaseswitch.h"

class KPrestige 
	: public KCloth
	, public KBaseSwitch
{
public:
	KPrestige();
	virtual ~KPrestige();

public:
	void loadTexture(const QImage& image, const QImage& curImage);
	void switchSlide(QPainter& painter, const QImage& preImage, const QImage& curImage, float frame);
	void initial(const float& width, const float& height, const QImage& preImage, const QImage& curImage);
	void drawCurImage(const QImage& curImage);
	void drawPreImage(const QImage& preImage, float frame);
	void setFixPosition();
private:
	KCloth m_Cloth;
	float m_fWidth;
	float m_fHeight;
	int		m_nTexture[2];
	QVector3D m_Gravity;
};

#endif; //__PPTSWITCHDOOR_KPRESTIGE_H__