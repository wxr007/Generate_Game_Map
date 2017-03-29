#pragma once
#pragma execution_character_set("utf-8")

#ifndef __MAP_SCENE_H__
#define __MAP_SCENE_H__

#include "cocos2d.h"
#include "MapPlate.h"

class MapScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(MapScene);

	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event);
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event);

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event);
protected:
	void DrawMap(int width, int length);
	void initCellGrid(); //��ʼ������
	void RecordCellsAround(int x, int y);

	void FillCell(CellPos& pos, const cocos2d::Color4F &color);
	void DrawCell(int x, int y, const cocos2d::Color4F &color);
	void PushCell(int x, int y);
protected:
	void InitMap(int width, int length);
	void FillPlateCell(MapPlate& plate);
	void DrawMap();
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

	typedef std::vector<CellInfo> CellArray;
	typedef std::vector<CellArray> CellGrid;
	CellGrid cell_grid;	//��ͼ����
	typedef std::queue<CellPos> CellList;
	CellList pre_cell_list;	//
	CellList after_cell_list;//


};
#endif // __MAP_SCENE_H__
