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
	float m_fAlpha = (-frame / m_fImageMeetScale) + 1;
	float m_fPreImageScaleWidth = halfRectWidth * frame / m_fImageMeetScale;
	float m_fPreImageScaleHeight = halfRectHeight * frame / m_fImageMeetScale;

	painter.save();
	painter.setOpacity(m_fAlpha);

	//左边的门
	QRect leftRect( m_fPreImageScaleWidth, preImage.rect().y() + m_fPreImageScaleHeight / 2,
		halfRectWidth - m_fPreImageScaleWidth, preImage.rect().height() - m_fPreImageScaleHeight);

	QImage leftImage = preImage.scaled(preImage.width(), preImage.height() + m_fPreImageScaleHeight);
	QTransform leftTransform;
	leftTransform.rotate(-m_fIncreasedAngle, Qt::YAxis);
	leftImage.transformed(leftTransform);

	QRect tLeftRect = QRect(preImage.rect().x(), preImage.rect().y(), 
		halfRectWidth - m_fPreImageScaleWidth, preImage.height());
	painter.drawImage(tLeftRect, leftImage, leftRect);

	//右边的门
	QRect rightRect(halfRectWidth, preImage.rect().y() + m_fPreImageScaleHeight / 2,
		halfRectWidth - m_fPreImageScaleWidth, preImage.height() - m_fPreImageScaleHeight);

	QImage rightImage = preImage.scaled(preImage.width(), preImage.height() + m_fPreImageScaleHeight);
	QTransform rightTransform;
	rightTransform.rotate(m_fIncreasedAngle, Qt::YAxis);
	rightImage.transformed(rightTransform);

	QRect tRightRect = QRect(preImage.rect().x() + halfRectWidth + m_fPreImageScaleWidth, preImage.rect().y(), 
		halfRectWidth - m_fPreImageScaleWidth, preImage.rect().height());
	painter.drawImage(tRightRect, rightImage, rightRect);

	painter.restore();
}

void KSwitchView::drawCurImageEffect(QPainter &painter,const QImage &curImage, const float& frame)
{
	float switchAreaWidth = curImage.width();
	float switchAreaHeight = curImage.height();
	
	float offsetWide = (switchAreaWidth * 0.6) / 2;
	float offsetHeight = (switchAreaHeight * 0.6) / 2;

	//计算当前image增长的大小
	float m_fIncreasedWidth = offsetWide * frame / (m_fImageMeetScale * 1.2);
	float m_fIncreasedHeight = offsetHeight * frame / (m_fImageMeetScale * 1.2);
	QImage image = curImage.scaled(switchAreaWidth * 0.4 + m_fIncreasedWidth * 2,
		switchAreaHeight * 0.4 + m_fIncreasedHeight * 2);

	QPointF widgetCenter = curImage.rect().center();

	painter.save();

	QRectF rectF(widgetCenter.x() - image.width() / 2,
		widgetCenter.y() - image.height() / 2, image.width(), image.height());
	painter.drawImage(rectF,image);

	painter.restore();
}

void KSwitchView::resizeEvent(QResizeEvent *event)
{
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
	m_imagePrePPT.load("E:\\MyTest\\pptSwtichDoor\\test.png");
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
