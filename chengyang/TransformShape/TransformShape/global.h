///////////////////////////////////////////////////////////////
//
// FileName : global.h
// Creator : chengyang
// Date : 2014-03-14 14:55
// Comment :存放全局常量
//
///////////////////////////////////////////////////////////////

#ifndef __TRANSFORMSHAPE__GLOBAL_H__
#define __TRANSFORMSHAPE__GLOBAL_H__



const float g_cfPI = 3.14;			///圆周率的近似值
const float g_cfNearZero = 0.000001;	///一个接近于0的值
const int	g_cnShapeDefaultWidth = 80;

const enum SHAPETYPE
{
	None = -1,							//无
	Line,								//线条
	Rect,								//矩形
	Ellipse,							//椭圆
	Triangle							//三角形
};

const enum MICRORECTADDR
{
	NONE,
	TopLeft, 
	TopCentral, 
	TopRight,
	LeftCentral,
	RightCentral,
	BottomLeft, 
	BottomCentral,
	BottomRight,
	RotateRect
};


#endif	//__TRANSFORMSHAPE__GLOBAL_H__