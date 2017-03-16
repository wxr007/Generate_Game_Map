#pragma once
#pragma execution_character_set("utf-8")

#ifndef __MAP_SCENE_H__
#define __MAP_SCENE_H__

#include "cocos2d.h"

//记录格子位置类型
struct CellPos
{
	CellPos(int _x, int _y){
		x = _x;
		y = _y;
	}
	int x;
	int y;
};

struct CellPosFill:CellPos
{
	enum {
		UnFilled = 0,	//未填充
		PreFill = 1,	//准备填充
		Filled = 2,		//已填充
	};

	CellPosFill(int _x, int _y)
	:CellPos(_x,_y){
		filled = UnFilled;
	}
	byte filled;//记录是否被填充
};

struct Vec2_Rad
{
	Vec2_Rad(float x, float y){
		vec = cocos2d::Vec2(x, y);
		rad = 0;
	}
	float rad;		//弧度
	cocos2d::Vec2 vec;
};

//交点
struct Intersections{	
	float line_y;			//y坐标
	std::list<float>	list_x;		//和所有边交点的x坐标 从左到右排列
};

class MapScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(MapScene);

	virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event);
	virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event);
protected:
	void DrawBasePolyByPoint();
	void DrawBasePloyByRadian();
	void DrawTest();

	void DrawMap(int width, int length);
	void initCellGrid(); //初始化网格
	void RecordCellsAround(int x, int y);

	void FillCell(CellPos& pos, const cocos2d::Color4F &color);
	void DrawCell(int x, int y, const cocos2d::Color4F &color);
	void PushCell(int x, int y);

private:
	cocos2d::TMXTiledMap* map;
	cocos2d::Sprite* bgSprite;
	cocos2d::DrawNode* my_drawNode;
	cocos2d::LayerColor* bgColor;


	int map_width;	//地图宽度
	int map_length;	//地图长度

	int grow_weight_x; //x方向的生长权重 %
	int grow_weight_y; //y方向的生长权重 %
	int grow_loop;	   //年轮生长圈数

	typedef std::vector<CellPosFill> CellArray;
	typedef std::vector<CellArray> CellGrid;
	CellGrid cell_grid;	//地图网格
	typedef std::queue<CellPos> CellList;
	CellList pre_cell_list;	//
	CellList after_cell_list;//


};
#endif // __MAP_SCENE_H__
