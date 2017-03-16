#pragma once
#pragma execution_character_set("utf-8")

#ifndef __MAP_SCENE_H__
#define __MAP_SCENE_H__

#include "cocos2d.h"

//��¼����λ������
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
		UnFilled = 0,	//δ���
		PreFill = 1,	//׼�����
		Filled = 2,		//�����
	};

	CellPosFill(int _x, int _y)
	:CellPos(_x,_y){
		filled = UnFilled;
	}
	byte filled;//��¼�Ƿ����
};

struct Vec2_Rad
{
	Vec2_Rad(float x, float y){
		vec = cocos2d::Vec2(x, y);
		rad = 0;
	}
	float rad;		//����
	cocos2d::Vec2 vec;
};

//����
struct Intersections{	
	float line_y;			//y����
	std::list<float>	list_x;		//�����б߽����x���� ����������
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
	void initCellGrid(); //��ʼ������
	void RecordCellsAround(int x, int y);

	void FillCell(CellPos& pos, const cocos2d::Color4F &color);
	void DrawCell(int x, int y, const cocos2d::Color4F &color);
	void PushCell(int x, int y);

private:
	cocos2d::TMXTiledMap* map;
	cocos2d::Sprite* bgSprite;
	cocos2d::DrawNode* my_drawNode;
	cocos2d::LayerColor* bgColor;


	int map_width;	//��ͼ���
	int map_length;	//��ͼ����

	int grow_weight_x; //x���������Ȩ�� %
	int grow_weight_y; //y���������Ȩ�� %
	int grow_loop;	   //��������Ȧ��

	typedef std::vector<CellPosFill> CellArray;
	typedef std::vector<CellArray> CellGrid;
	CellGrid cell_grid;	//��ͼ����
	typedef std::queue<CellPos> CellList;
	CellList pre_cell_list;	//
	CellList after_cell_list;//


};
#endif // __MAP_SCENE_H__
