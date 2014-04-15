#include "kswitchview.h"
#include "klistwidgetitem.h"
#include "ksliderpptlist.h"
#include "klanternslide.h"
#include "gl\glut.h"
#include <QListWidgetItem>
#include <QPainter>
#include <QTimer>
#include <QtDebug>

KSwitchView::KSwitchView(QWidget *parent)
	: QGLWidget(parent)
	, m_switchStyle(SWITCHSTYLE_NONE)
	, m_pSliderListWidget(0)
	, m_preImageColor(Qt::black)
	, m_curImageColor(Qt::white)
	, m_fSwitchImageDuration(0)
	, m_fTimeInterval(30)
	, m_fTransformTime(0)
	, m_pBaseSwitch(0)
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

void KSwitchView::initializeGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glShadeModel(GL_SMOOTH);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void KSwitchView::resizeGL(int width, int height)
{
	stopSwitch();
	m_rectImageArea = this->rect();

	glViewport(0, 0,(GLint)width, (GLint)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (GLfloat)width / (GLfloat)height, 1.0f, 200.0f);

	glMatrixMode(GL_MODELVIEW);
}

void KSwitchView::setSliderListWidget(KSliderpptList *&sliderList)
{
	m_pSliderListWidget = sliderList;
}

void KSwitchView::paintEvent(QPaintEvent *event)
{
	makeCurrent();
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
	//painter.fillRect(m_rectImageArea, Qt::SolidPattern);

	if (m_timeDuration->isActive())
	{
		if (m_fTransformTime < (m_fSwitchImageDuration * 1000))
		{
			m_fTransformTime += m_fTimeInterval;

			float frame = m_fTransformTime / ((m_fSwitchImageDuration * 1000) * 1.0);
			if (m_pBaseSwitch)
			{
				m_pBaseSwitch->switchSlide(painter, m_imagePrePPT, m_imageCurPPT, frame);
			}
		}
		else
		{
			stopSwitch();
			//return;
		}
	}
	else
	{
		painter.drawImage(m_rectImageArea, m_imageCurPPT);
		if (m_pBaseSwitch)
		{
			delete m_pBaseSwitch;
			m_pBaseSwitch = 0;
		}
	}
	painter.end();
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
	m_switchStyle = style;
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
	m_imagePrePPT.load(".\\preImage.jpg");
	m_imageCurPPT.load(".\\curImage.jpg");

	m_imagePrePPT = m_imagePrePPT.scaled(m_rectImageArea.width(), m_rectImageArea.height());
	m_imageCurPPT = m_imageCurPPT.scaled(m_rectImageArea.width(),m_rectImageArea.height());

	//m_imagePrePPT.fill(m_preImageColor);
	//m_imageCurPPT.fill(m_curImageColor);

	m_fSwitchImageDuration = curItem->getLanterSlideData()->getDuration();
	
	m_pBaseSwitch->initial(m_rectImageArea.width(), m_rectImageArea.height(),m_imagePrePPT, m_imageCurPPT);

	m_timeDuration->start(m_fTimeInterval);
}

void KSwitchView::setBaseSwitch(KBaseSwitch *baseSwitch)
{
	m_pBaseSwitch = baseSwitch;
}

void KSwitchView::stopSwitch()
{
	m_fTransformTime =0;
	m_timeDuration->stop();
	update(this->rect());
}