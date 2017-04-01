#pragma once
#pragma execution_character_set("utf-8")

#ifndef __MAP_PLATE_H__
#define __MAP_PLATE_H__

#include <vector>
#include <list>
#include "cocos2d.h"
#include "Algorithm.h"

const int Cell_Len = 16;			//���Ӵ�С
const int Cell_Group = 5;


class MapPlate
{
public:
	MapPlate(int x,int y);
	~MapPlate();
protected:
	void DrawBasePloy();		//���ƻ�����״
	void GetIntersections();	//������״x��ƽ���߽���
	void FillCells();			//������
private:
	CellPos MinPos, MaxPos,CenterPos;
	std::vector<cocos2d::Vec2> poly_points;
	std::vector<Intersections> intersection_list; //�������
public:
	typedef std::list<CellPos> CellList;
	CellList PlateCells;			//����еĸ���
	CellList BoundCells;			//��Ե�ϵĸ���
};

#endif // __MAP_PLATE_H__
