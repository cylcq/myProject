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
	, m_fTimeInterval(30)
	, m_fTransformTime(0)
	, m_fImageMeetScale(0.666)
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

void KSwitchView::updateSlide()
{

}

void KSwitchView::switchSlideDoor(QPainter& painter, const QImage& preImage, const QImage& curImage, float frame)
{
	painter.save();

	drawCurImageEffect(painter, curImage, frame);
	if (frame < m_fImageMeetScale)
	{
		drawPreImageEffect(painter, preImage, frame);
	}

	painter.restore();
	
}

void KSwitchView::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
	painter.fillRect(m_rectImageArea, Qt::SolidPattern);

	if (m_timeDuration->isActive())
	{
		if (m_fTransformTime < (m_fSwitchImageDuration * 1000))
		{
			m_fTransformTime += m_fTimeInterval;

			float frame = m_fTransformTime / ((m_fSwitchImageDuration * 1000) * 1.0);
			switchSlideDoor(painter, m_imagePrePPT, m_imageCurPPT, frame);

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

void KSwitchView::drawPreImageEffect(QPainter &painter,const QImage &preImage, const float& frame)
{
	float switchAreaWidth = preImage.width();
	float switchAreaHeight = preImage.height();

	float halfRectWidth = switchAreaWidth / 2;
	float halfRectHeight = switchAreaHeight / 2;

	float m_fIncreasedAngle = 90 * frame / m_fImageMeetScale;
	float m_fAlpha = (m_fImageMeetScale + 0.2 - frame);
	float m_fPreImageScaleWidth = halfRectWidth * frame / m_fImageMeetScale;
	float m_fPreImageScaleHeight = halfRectHeight * frame / m_fImageMeetScale;//�������switchAreaHeight��0.2��

	painter.save();
	painter.setOpacity(m_fAlpha);

	//��ߵ���
	QImage leftImage = preImage;
	QRect leftRect( m_fPreImageScaleWidth, leftImage.rect().y() + m_fPreImageScaleHeight / 2,
		halfRectWidth - m_fPreImageScaleWidth, preImage.rect().height() - m_fPreImageScaleHeight);
	qDebug()<<preImage.rect();
	leftImage = leftImage.scaled(leftImage.width(), leftImage.height() + m_fPreImageScaleHeight);
	qDebug()<<"left"<<leftImage.rect();
	leftImage = leftImage.copy(leftRect);

	QTransform leftTransform;
	leftTransform.rotate(-m_fIncreasedAngle, Qt::YAxis);
	leftImage.transformed(leftTransform);

	leftRect = QRect(preImage.rect().x(), preImage.rect().y(), 
		halfRectWidth - m_fPreImageScaleWidth, preImage.height());
	
	painter.drawImage(leftRect, leftImage);

	/*QRect leftRect(preImage.rect().x() + m_fPreImageScaleWidth, preImage.rect().y() + m_fPreImageScaleHeight / 2,
		halfRectWidth - m_fPreImageScaleWidth, preImage.height());
	painter.drawImage(QPoint(preImage.rect().x(), preImage.rect().y()), leftImage, leftRect);*/
	
	QImage rightImage = preImage;
	
	QRect rightRect(halfRectWidth, rightImage.rect().y() + m_fPreImageScaleHeight / 2,
		halfRectWidth - m_fPreImageScaleWidth, preImage.height() - m_fPreImageScaleHeight);

	rightImage = rightImage.scaled(rightImage.width(), rightImage.height() + m_fPreImageScaleHeight);
	rightImage = rightImage.copy(rightRect);
	QTransform rightTransform;
	rightTransform.rotate(m_fIncreasedAngle, Qt::YAxis);
	rightImage.transformed(rightTransform);

	rightRect = QRect(halfRectWidth + m_fPreImageScaleWidth, preImage.rect().y(), 
		halfRectWidth - m_fPreImageScaleWidth, preImage.rect().height());
	painter.drawImage(rightRect, rightImage);

	//�ұߵ���
	//QImage rightImage = preImage.scaled( preImage.width() + m_fPreImageScaleWidth, preImage.height() + m_fPreImageScaleHeight);
	//QRect rightRect(rightImage.rect().center().x(),rightImage.rect().y() - m_fPreImageScaleHeight / 2,
	//	halfRectWidth - m_fPreImageScaleWidth, preImage.height() - m_fPreImageScaleHeight);

	//rightImage = rightImage.copy(rightRect);
	//QTransform rightTransform;
	//rightTransform.rotate(m_fIncreasedAngle, Qt::YAxis);
	//rightImage.transformed(rightTransform);

	//rightRect = QRect(halfRectWidth + m_fPreImageScaleWidth, preImage.rect().y(), 
	//	halfRectWidth - m_fPreImageScaleWidth, preImage.rect().height());
	//painter.drawImage(rightRect, leftImage);


	/*QRect rightRect(preImage.rect().center().x(),preImage.rect().y() + m_fPreImageScaleHeight / 2,
		halfRectWidth - m_fPreImageScaleWidth, preImage.height());
	QPoint point(preImage.rect().x() + halfRectWidth + m_fPreImageScaleWidth, preImage.rect().y());

	painter.drawImage(point, rightImage, rightRect);
	*/

	painter.restore();
	
}

void KSwitchView::drawCurImageEffect(QPainter &painter,const QImage &curImage, const float& frame)
{
	float switchAreaWidth = curImage.width();
	float switchAreaHeight = curImage.height();
	
	float offsetWide = (switchAreaWidth  - curImage.width() * 0.4) / 2;
	float offsetHeight = (switchAreaHeight - curImage.height() * 0.4) / 2;

	//���㵱ǰimage�����Ĵ�С
	float m_fIncreasedWidth = offsetWide * frame;
	float m_fIncreasedHeight = offsetHeight * frame;
	QImage image = curImage.scaled(curImage.width() * 0.4 + m_fIncreasedWidth * 2,
		curImage.height() * 0.4 + m_fIncreasedHeight * 2);

	QPointF widgetCenter = curImage.rect().center();

	painter.save();

	QRectF rectF(widgetCenter.x() - image.width() / 2,
		widgetCenter.y() - image.height() / 2, image.width(), image.height());
	painter.drawImage(rectF,image);

	painter.restore();
}

void KSwitchView::resizeEvent(QResizeEvent *event)
{
	/*m_rectImageArea = QRectF(this->width() *0.15, this->height() * 0.1,
		this->width() * 0.7, this->height() * 0.8);*/
	m_rectImageArea = this->rect();
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

	//test
	bool i =m_imagePrePPT.load("E:\\MyTest\\pptSwtichDoor\\pptSwtichDoor\\test.png");

	m_imageCurPPT = m_imageCurPPT.scaled(m_rectImageArea.width(),
		m_rectImageArea.height());

	m_imagePrePPT = m_imagePrePPT.scaled(m_rectImageArea.width(), m_rectImageArea.height());

	//m_imagePrePPT.fill(m_preImageColor);
	m_imageCurPPT.fill(m_curImageColor);

	

	m_fSwitchImageDuration = curItem->getLanterSlideData()->getDuration();

	m_timeDuration->start(m_fTimeInterval);
}

void KSwitchView::stopSwitch()
{
	
	m_fTransformTime =0;
	m_timeDuration->stop();
	update(this->rect());
}

void KSwitchView::calTimeOutOffSet(float& switchAreaWidth, float& switchAreaHeight, float frame)
{

	/*if (frame < m_fImageMeetScale)
	{
		switchAreaWidth = switchAreaWidth * m_fImageMeetScale;
		switchAreaHeight = switchAreaHeight * m_fImageMeetScale;
	}

	float offsetWide = (switchAreaWidth  - m_imageCurPPT.width()) / 2;
	float offsetHeight = (switchAreaHeight - m_imageCurPPT.height()) / 2;

	m_fIncreasedWidth = offsetWide * frame;
	m_fIncreasedHeight = offsetHeight * frame;

	m_fIncreasedAngle = 90 * frame;
	m_fAlphaDecrease = 255 * (1 - frame);
	m_fPreImage_One_ScaleValue = (switchAreaWidth / 2) * frame;*/

}
void KSwitchView::setTransparent(QImage &image, int alpha)
{
	/*QColor color = m_preImageColor;
	color.setAlpha(alpha);
	image.fill(color);
*/
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
