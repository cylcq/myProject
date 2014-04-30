#ifndef __PPTSWITCH_KCURTAIN_H__
#define __PPTSWITCH_KCURTAIN_H__

#include "kbaseswitch.h"
#include "baseCloth/kcloth.h"
#include "kbaseswitch.h"

class KCurtain :public KBaseSwitch
{
public:
	KCurtain();
	virtual ~KCurtain();

public:
	void loadTexture(const QImage& image, const QImage& curImage);
	void switchSlide(QPainter& painter, const QImage& preImage, const QImage& curImage, float frame);
	void initial(const float& width, const float& height, const QImage& preImage, const QImage& curImage);
	void drawCurImage(const QImage& curImage);
	void drawPreImage(const QImage& preImage, float frame);
	void setSpecialPosition();
	void drawCloth(KCloth& cloth, int textureAddr, float frame, float coordX, float coordY);

private:
	float m_fWidth;
	float m_fHeight;
	KCloth m_leftCloth;
	KCloth m_rightCloth;
	int		m_nTexture[3];	//Èý¸öÎÆÀí
	QVector3D m_Gravity;
	float m_fOldFrame;
};

#endif; //__PPTSWITCH_KCURTAIN_H__