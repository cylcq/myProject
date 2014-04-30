#include "kdoor.h"
#include <QtDebug>

KDoor::KDoor(void)
	: m_fImageMeetScale(0.666)
{
}


KDoor::~KDoor(void)
{
}

void KDoor::switchSlide(QPainter& painter, const QImage& preImage, const QImage& curImage, float frame)
{
	painter.save();
	qDebug()<<frame;
	drawCurImageEffect(painter, curImage, frame);
	if (frame < m_fImageMeetScale)
	{
		drawPreImageEffect(painter, preImage, frame);
	}

	painter.restore();
}

void KDoor::drawPreImageEffect(QPainter &painter,const QImage &preImage, const float& frame)
{
	
	float switchAreaWidth = preImage.width();
	float switchAreaHeight = preImage.height();

	float halfRectWidth = switchAreaWidth / 2;
	float halfRectHeight = switchAreaHeight / 2;

	float m_fIncreasedAngle = 10 * frame / m_fImageMeetScale;
	float m_fAlpha = (-frame / m_fImageMeetScale) + 1;
	float m_fPreImageScaleWidth = halfRectWidth * frame / m_fImageMeetScale;
	float m_fPreImageScaleHeight = halfRectHeight * frame / m_fImageMeetScale;

	painter.save();
	painter.setOpacity(m_fAlpha);

	//左边的门

 	QImage leftImage = preImage.copy(0, 0, halfRectWidth, switchAreaHeight);
 	QTransform leftTransform;
 	leftTransform.rotate(-m_fIncreasedAngle, Qt::YAxis);
 	leftImage = leftImage.transformed(leftTransform);
 
 	QRect leftRect(m_fPreImageScaleWidth, leftImage.rect().y() + m_fPreImageScaleHeight / 2,
 		halfRectWidth - m_fPreImageScaleWidth, leftImage.rect().height() - m_fPreImageScaleHeight);
	
 	QRect tLeftRect = QRect(preImage.rect().x(), preImage.rect().y(), 
 		halfRectWidth - m_fPreImageScaleWidth, preImage.height());
 	painter.drawImage(tLeftRect, leftImage, leftRect);


	//右边的门
 	QImage rightImage = preImage.copy(halfRectWidth, 0, halfRectWidth, switchAreaHeight);

 	QTransform rightTransform;
 	rightTransform.rotate(m_fIncreasedAngle, Qt::YAxis);
 	rightImage = rightImage.transformed(rightTransform);

 	QRect rightRect(0, m_fPreImageScaleHeight / 2, halfRectWidth - m_fPreImageScaleWidth, 
		rightImage.height() - m_fPreImageScaleHeight);
 
 	QRect tRightRect = QRect(preImage.rect().x() + halfRectWidth + m_fPreImageScaleWidth, preImage.rect().y(), 
 		halfRectWidth - m_fPreImageScaleWidth, preImage.rect().height());
	painter.drawImage(tRightRect,rightImage,rightRect);

	painter.restore();
}

void KDoor::drawCurImageEffect(QPainter &painter,const QImage &curImage, const float& frame)
{
	float switchAreaWidth = curImage.width();
	float switchAreaHeight = curImage.height();

	float offsetWide = (switchAreaWidth * 0.6) / 2;
	float offsetHeight = (switchAreaHeight * 0.6) / 2;

	//计算当前image增长的大小
	float m_fIncreasedWidth = offsetWide * frame;
	float m_fIncreasedHeight = offsetHeight * frame;
	QImage image = curImage.scaled(switchAreaWidth * 0.4 + m_fIncreasedWidth * 2,
		switchAreaHeight * 0.4 + m_fIncreasedHeight * 2);

	QPointF widgetCenter = curImage.rect().center();

	painter.save();

	QRectF rectF(widgetCenter.x() - image.width() / 2,
		widgetCenter.y() - image.height() / 2, image.width(), image.height());
	painter.drawImage(rectF,image);

	painter.restore();
}
