///////////////////////////////////////////////////////////////
//
// FileName : kshape.h
// Creator : chengyang
// Date : 2014-03-13 20:45
// Comment :图形的基本数据
//
///////////////////////////////////////////////////////////////
#ifndef __TRANSFORMSHAPE_KSHAPE_H__
#define __TRANSFORMSHAPE_KSHAPE_H__

#include "global.h"

class KShape
{

public:
	KShape();
	~KShape();
	void			setTopLeftPointX(const float &x);
	float			getTopLeftPointX(){return m_fTopLeftPointX;}
	void			setTopLeftPointY(const float &y);
	float			getTopLeftPointY(){return m_fTopLeftPointY;}
	void			setBottomRightPointX(const float &x);
	float			getBottomRightPointX(){return m_fBottomRightPointX;}
	void			setBottomRightPointY(const float &y);
	float			getBottomRightPointY(){return m_fBottomRightPointY;}
	void			setShapeWidth(const float &width){m_fShapeWidth = width;}
	float			getShapeWidth(){return m_fShapeWidth;} 
	void			setShapeHeight(const float &height){m_fShapeHeight = height;}
	float			getShapeHeight(){return m_fShapeHeight;}
	void			setShapeType( SHAPETYPE type){m_nShapeType = type;}
	SHAPETYPE		getShapeType(){return m_nShapeType;}
	void			setShapeRotateRate(int rate){m_fShapeRotateRate = rate;}
	float			getShapeRotateRate(){return m_fShapeRotateRate;}

private:
	float			m_fTopLeftPointX;
	float			m_fTopLeftPointY;
	float			m_fBottomRightPointX;
	float			m_fBottomRightPointY;
	float			m_fShapeWidth;
	float			m_fShapeHeight;
	SHAPETYPE		m_nShapeType;
	float			m_fShapeRotateRate;
};

#endif // __TRANSFORMSHAPE_KSHAPE_H__
