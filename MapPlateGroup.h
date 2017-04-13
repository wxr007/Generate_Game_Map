#ifndef __MAP_PLATE_GROUP_H__
#define __MAP_PLATE_GROUP_H__

#include "cocos2d.h"
#include "MapPlate.h"

class MapPlateGroup
{
public:
	MapPlateGroup(int x,int y,cocos2d::DrawNode* node);
	~MapPlateGroup();
public:
	void FilledCellGrid(CellGrid& cell_grid);					//填充格子
protected:
	void InitPlates();											//初始化地图板块
	void AddPlate(int x, int y);								//添加地图板块
	int  AddAroundPlate(int org_x, int org_y, int pre_angle);	//添加周围的板块
	void FilledPlateCells(MapPlate* plate,CellGrid& cell_grid);	//填充板块格子
private:
	CellPos plate_pos;
	std::vector<MapPlate*> plate_array;
	cocos2d::DrawNode* my_drawNode;
};

#endif