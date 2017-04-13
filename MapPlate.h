#ifndef __MAP_PLATE_H__
#define __MAP_PLATE_H__

#include <vector>
#include <list>
#include "cocos2d.h"
#include "Algorithm.h"

const int Cell_Len = 16;			//格子大小
const int Cell_Group = 5;

class MapPlate
{
public:
	MapPlate(int x,int y);
	~MapPlate();
protected:
	void DrawBasePloy();		//绘制基础形状
	void GetIntersections();	//计算形状x轴平行线交点
	void FillCells();			//填充格子
private:
	CellPos MinPos, MaxPos;
	std::vector<cocos2d::Vec2> poly_points;
	std::vector<Intersections> intersection_list; //交点队列
public:
	CellPos CenterPos;
	CellList PlateCells;			//板块中的格子
	CellList BoundCells;			//边缘上的格子
};

#endif // __MAP_PLATE_H__