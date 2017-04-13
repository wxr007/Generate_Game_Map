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
	void FilledCellGrid(CellGrid& cell_grid);					//������
protected:
	void InitPlates();											//��ʼ����ͼ���
	void AddPlate(int x, int y);								//��ӵ�ͼ���
	int  AddAroundPlate(int org_x, int org_y, int pre_angle);	//�����Χ�İ��
	void FilledPlateCells(MapPlate* plate,CellGrid& cell_grid);	//��������
private:
	CellPos plate_pos;
	std::vector<MapPlate*> plate_array;
	cocos2d::DrawNode* my_drawNode;
};

#endif