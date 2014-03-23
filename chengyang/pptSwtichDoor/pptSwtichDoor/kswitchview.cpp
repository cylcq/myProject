#include "kswitchview.h"
#include "klistwidgetitem.h"
#include "ksliderpptlist.h"
#include "klanternslide.h"
#include <QListWidgetItem>
#include <QPainter>
#include <QTimer>
#include <QtDebug>

KSwitchView::KSwitchView(QWidget *parent)
	: QWidget(parent)
	, m_pSliderListWidget(0)
	, m_preImageColor(Qt::black)
	, m_curImageColor(Qt::white)
	, m_fSwitchImageDuration(0)
	, m_fIncreasedWidth(0)
	, m_fIncreasedHeight(0)
	, m_fIncreasedAngle(0)
	, m_fAlphaDecrease(0)
	, m_fCurImageWidth(0)
	, m_fCurImageHeight(0)
	, m_fRotateAngle(0)
	, m_fTimeInterval(20)
	, m_fAlpha(255)
	, m_fTransformTime(0)
	, m_fImageMeetScale(0.666)
	, m_fPreImage_Total_ScaleValue(0)
	, m_fPreImage_One_ScaleValue(0)
{
	QPalette palette;
	palette.setBrush(this->backgroundRole(),QBrush(QColor(176, 196, 222)));
	this->setPalette(palette);
	this->setAutoFillBackground(true);
	m_imageCurPPT = QImage(1, 1, QImage::Format_ARGB32);
	m_imagePrePPT = QImage(2, 1, QImage::Format_ARGB32);

	m_timeDuration = new QTimer(this);
	connect(m_timeDuration, SIGNAL(timeout()), this, SLOT(update()));
}

KSwitchView::~KSwitchView()
{

}
void KSwitchView::setSliderListWidget(KSliderpptList *&sliderList)
{
	m_pSliderListWidget = sliderList;
}

void KSwitchView::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
	painter.fillRect(m_rectImageArea, Qt::SolidPattern);

	if (m_timeDuration->isActive())
	{
		
		/*float showCurImageTime = m_fSwitchImageDuration * 1000 * 0.03;
		if (m_fTransformTime - showCurImageTime > 0 && m_fTransformTime - showCurImageTime < 200)
		{
		m_fAlpha -= 5;
		}*/
		
		if (m_fTransformTime < (m_fSwitchImageDuration * 1000))
		{
			m_fTransformTime += m_fTimeInterval;
			drawCurImageEffect(painter, m_imageCurPPT);

			if (m_fTransformTime < (m_fSwitchImageDuration * 1000 * m_fImageMeetScale))
			{
				drawPreImageEffect(painter, m_imagePrePPT);
			}
			else
			{
				float duration = m_fSwitchImageDuration - m_fTransformTime / 1000;
				calTimeOutOffset(1, duration, m_rectImageArea.width(), m_rectImageArea.height());
			}
		}
		else
		{
			stopSwitch();
		}
	}
	else
	{
		painter.drawImage(m_rectImageArea, m_imageCurPPT);
	}
}

void KSwitchView::drawPreImageEffect(QPainter &painter, QImage &preImage)
{
	QPointF widgetCenter = m_rectImageArea.center();
	float halfRectWidth = m_rectImageArea.width() / 2;
	float halfRectHeight = m_rectImageArea.height() / 2;
	m_fAlpha -= m_fAlphaDecrease;
	m_fRotateAngle += m_fIncreasedAngle;
	m_fPreImage_Total_ScaleValue += m_fPreImage_One_ScaleValue;

	//左边的门
	QImage leftImage = preImage;
	QRect leftRect(leftImage.rect().x() + (m_fPreImage_Total_ScaleValue), leftImage.rect().y() + m_fPreImage_Total_ScaleValue / 2,
		halfRectWidth - m_fPreImage_Total_ScaleValue, m_rectImageArea.height() - m_fPreImage_Total_ScaleValue);

	leftImage.scaledToHeight( m_rectImageArea.height() + m_fPreImage_Total_ScaleValue);

	leftImage =  leftImage.copy(leftRect);
	setTransparent(leftImage, m_fAlpha);
	QTransform leftTransform;
	leftTransform.rotate(-m_fRotateAngle, Qt::YAxis);
	leftImage.transformed(leftTransform);

	leftRect = QRect(m_rectImageArea.x(), m_rectImageArea.y(), 
		halfRectWidth - m_fPreImage_Total_ScaleValue, m_rectImageArea.height());
	painter.drawImage(leftRect,leftImage);

	//右边的门
	QImage rightImage = preImage;
	QRect rightRect(halfRectWidth, rightImage.rect().y() + m_fPreImage_Total_ScaleValue / 2,
		halfRectWidth - m_fPreImage_Total_ScaleValue, m_rectImageArea.height() - m_fPreImage_Total_ScaleValue);

	rightImage.scaledToHeight( m_rectImageArea.height() + m_fPreImage_Total_ScaleValue);

	rightImage =  rightImage.copy(rightRect);
	setTransparent(rightImage, m_fAlpha);
	QTransform rightTransform;
	rightTransform.rotate(m_fRotateAngle, Qt::YAxis);
	rightImage.transformed(rightTransform);

	rightRect = QRect(m_rectImageArea.x() + halfRectWidth + m_fPreImage_Total_ScaleValue, 
		m_rectImageArea.y(), halfRectWidth - m_fPreImage_Total_ScaleValue, m_rectImageArea.height());
	painter.drawImage(rightRect,rightImage);
}

void KSwitchView::drawCurImageEffect(QPainter &painter, QImage &curImage)
{
	m_fCurImageWidth += (m_fIncreasedWidth * 2);
	m_fCurImageHeight += (m_fIncreasedHeight * 2);

	curImage = curImage.scaled(m_fCurImageWidth, m_fCurImageHeight);

	QPointF widgetCenter = m_rectImageArea.center();

	painter.save();
	QRectF rectF(widgetCenter.x() - m_fCurImageWidth / 2,
		widgetCenter.y() - m_fCurImageHeight / 2, m_fCurImageWidth, m_fCurImageHeight);
	painter.drawImage(rectF,curImage);
	painter.restore();
}

void KSwitchView::resizeEvent(QResizeEvent *event)
{
	m_rectImageArea = QRectF(this->width() *0.15, this->height() * 0.1,
		this->width() * 0.7, this->height() * 0.8);

	m_fCurImageWidth = m_rectImageArea.width() * 0.25;
	m_fCurImageHeight = m_rectImageArea.height() * 0.4;
}

void KSwitchView::switchPPTColor(QListWidgetItem* current, QListWidgetItem* previous)
{
	if (m_timeDuration->isActive())
	{
		stopSwitch();
	}
	KListWidgetItem* curItem = (KListWidgetItem*)current;
	m_curImageColor = curItem->getLanterSlideData()->getColor();
	m_imageCurPPT.fill(m_curImageColor);

	m_preImageColor = Qt::black;
	
	emit amendDuration(curItem->getLanterSlideData()->getDuration());
	update(this->rect());
}

void KSwitchView::switchPPT(SWITCHSTYLE style)
{
	if (m_timeDuration->isActive())
	{
		stopSwitch();
		return;
	}
	KListWidgetItem* curItem = (KListWidgetItem*)m_pSliderListWidget->currentItem();
	m_imageCurPPT = curItem->getLanterSlideData()->getImage();
	
	int curRow = m_pSliderListWidget->currentRow();
	if (curRow > 0)
	{
		KListWidgetItem* preItem = 0;
		preItem = (KListWidgetItem*)m_pSliderListWidget->item(curRow - 1);
		m_preImageColor = preItem->getLanterSlideData()->getColor();
		m_imagePrePPT = preItem->getLanterSlideData()->getImage();
	}
	//bool i =m_imagePrePPT.load("E:\\gitProject\\wps_dev_wpp\\homework\\chengyang\\pptSwtichDoor\\pptSwtichDoor\\test.png");

	m_imageCurPPT = m_imageCurPPT.scaled(m_rectImageArea.width() * 0.25,
		m_rectImageArea.height() * 0.4);

	m_imagePrePPT = m_imagePrePPT.scaled(m_rectImageArea.width(), m_rectImageArea.height());
	QRect rect = m_imagePrePPT.rect();
	m_imagePrePPT.fill(m_preImageColor);
	m_imageCurPPT.fill(m_curImageColor);

	

	m_fSwitchImageDuration = curItem->getLanterSlideData()->getDuration() * m_fImageMeetScale;

	float width = m_rectImageArea.width() * m_fImageMeetScale;
	float height = m_rectImageArea.height() * m_fImageMeetScale;
	calTimeOutOffset(m_fImageMeetScale, m_fSwitchImageDuration, width, height);

	m_timeDuration->start(m_fTimeInterval);
}

void KSwitchView::stopSwitch()
{
	m_fCurImageWidth = m_rectImageArea.width() * 0.3;
	m_fCurImageHeight = m_rectImageArea.height() * 0.3;
	m_fRotateAngle = 0;
	m_fAlpha = 255;
	m_fTransformTime =0;
	m_fPreImage_Total_ScaleValue = 0;
	m_timeDuration->stop();
	update(this->rect());
}

void KSwitchView::calTimeOutOffset(const float &imageMeetScale, const float &duration,
	const float &width, const float &height)
{
	float offsetWide = (width  - m_imageCurPPT.width()) / 2;
	float offsetHeight = (height - m_imageCurPPT.height()) / 2;

	m_fIncreasedWidth = (offsetWide / (duration * 1000 * 
		imageMeetScale)) * m_fTimeInterval;
	m_fIncreasedHeight = (offsetHeight / (duration * 1000 * 
		imageMeetScale)) * m_fTimeInterval;

	m_fIncreasedAngle = (90 / (duration * 1000 * 
		imageMeetScale)) * m_fTimeInterval;

	m_fAlphaDecrease = 255 / (duration * 1000 * 
		imageMeetScale) * m_fTimeInterval;

	m_fPreImage_One_ScaleValue = ((m_rectImageArea.width() / 2) /
		(duration * 1000 * imageMeetScale)) * m_fTimeInterval;
}

void KSwitchView::setTransparent(QImage &image, int alpha)
{
	QColor color = m_preImageColor;
	color.setAlpha(alpha);
	image.fill(color);

	/*QImage mage = image;
	QRgb rgb;
	for(int i = 0;i < mage.rect().width(); i++)
	{
	for (int j = 0;j < mage.rect().height(); j++)
	{
	QRgb rgb1 = mage.pixel(i,j);
	rgb = qRgba(qRed(rgb1),qGreen(rgb1),qBlue(rgb1),alpha);
	immageage.setPixel(i,j,rgb);
	}
	}
	image = mage;*/
}
