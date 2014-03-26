///////////////////////////////////////////////////////////////
//
// FileName : global.h
// Creator : chengyang
// Date : 2014-03-14 14:55
// Comment :���ȫ�ֳ���
//
///////////////////////////////////////////////////////////////

#ifndef __TRANSFORMSHAPE__GLOBAL_H__
#define __TRANSFORMSHAPE__GLOBAL_H__



const float g_cfPI = 3.14;			///Բ���ʵĽ���ֵ
const float g_cfNearZero = 0.000001;	///һ���ӽ���0��ֵ
const int	g_cnShapeDefaultWidth = 80;

const enum SHAPETYPE
{
	None = -1,							//��
	Line,								//����
	Rect,								//����
	Ellipse,							//��Բ
	Triangle							//������
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