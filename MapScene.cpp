#pragma execution_character_set("utf-8")
#include "MapScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

const Vec2 Cell_Rect = Vec2(Cell_Len, Cell_Len);
//mini	8400*2400
//mid	12600*3600
//large 16800*4800
const int Map_Width = 48*4;						//��ͼ��
const int Map_Length = 64*4;					//��ͼ��

#define Grow_Max_Loop 40						//���ɳ�Ȧ��
#define Grow_Min_Loop 20						//��С�ɳ�Ȧ��

Scene* MapScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MapScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

//on "init" you need to initialize your instance
bool MapScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	CCLOG("origin =(%f,%f)", origin.x, origin.y);

	bgColor = LayerColor::create(Color4B::BLACK);
//	bgColor->setAnchorPoint(Vec2::ZERO);
	bgColor->setPosition(Vec2::ZERO);
	bgColor->setPosition(Vec2((visibleSize.width - Map_Width * Cell_Len) / 2, (visibleSize.height - Map_Length * Cell_Len) / 2));
	this->addChild(bgColor);

	my_drawNode = DrawNode::create();
	bgColor->addChild(my_drawNode);

	

	// 1 ����һ���¼�������
	auto TouchListener = EventListenerTouchAllAtOnce::create();
	// 2 �󶨴����¼�
	TouchListener->onTouchesBegan = CC_CALLBACK_2(MapScene::onTouchesBegan, this);// ������ʼʱ����
	TouchListener->onTouchesMoved = CC_CALLBACK_2(MapScene::onTouchesMoved, this);// �����ƶ�ʱ����
	// 3 ��Ӽ�����
	_eventDispatcher->addEventListenerWithSceneGraphPriority(TouchListener, this);

	// 1 ����һ���¼�������
	auto listenerkeyPad = EventListenerKeyboard::create();//���̼���
	// 2 �󶨼����¼�
	listenerkeyPad->onKeyPressed = CC_CALLBACK_2(MapScene::onKeyPressed, this);
	listenerkeyPad->onKeyReleased = CC_CALLBACK_2(MapScene::onKeyReleased, this);
	// 3 ��Ӽ�����
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);

	map_width = 0;
	map_length = 0;
	srand(time(NULL));

	grow_weight_x = 0;
	grow_weight_y = 0;

//	DrawMap(Map_Width, Map_Length);

	InitMap(Map_Width, Map_Length);

	return true;
}

void MapScene::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
{

}

void MapScene::onKeyPressed(EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{

}

void MapScene::onKeyReleased(EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_SPACE){
		my_drawNode->clear();
		InitPlate();
		DrawMap();
	}
}

void MapScene::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
{
	auto winSize = Director::getInstance()->getWinSize();

	// ����ʱ��touches��ֻ��һ��Touch��������ͨ��touches[0]�Ϳ��Եõ���������
	auto touch = touches[0];
	// ���㻬�������еĻ�������
	auto diff = touch->getDelta();
	// �õ���ǰbgSprite��λ��
	auto currentPos = bgColor->getPosition();
	// �õ�������bgSpriteӦ�����ڵ�λ��
	auto pos = currentPos + diff;
	// �õ��˿�bgSprite�ĳߴ�
	auto bgSpriteCurrSize = bgColor->getBoundingBox().size;

	//�߽���ƣ�Լ��pos��λ��
// 	pos.x = MIN(pos.x, bgSpriteCurrSize.width * bgSprite->getAnchorPoint().x);
// 	pos.x = MAX(pos.x, -bgSpriteCurrSize.width + winSize.width + bgSpriteCurrSize.width * bgSprite->getAnchorPoint().x);
// 	pos.y = MIN(pos.y, bgSpriteCurrSize.height * bgSprite->getAnchorPoint().y);
// 	pos.y = MAX(pos.y, -bgSpriteCurrSize.height + winSize.height + bgSpriteCurrSize.height * bgSprite->getAnchorPoint().y);
	// ����bgSpriteλ��
	bgColor->setPosition(pos);

	Vec2 origin = Director::getInstance()->getVisibleOrigin();

//	CCLOG("pos %f %f", pos.x, pos.y);
}

void MapScene::DrawMap(int width,int length)
{
	if (width > 0 && length > 0){
		map_width = width;
		map_length = length;
		if (rand()%2 == 0){
			grow_weight_x = Random(60, 100);
			grow_weight_y = Random(30, 70);
		}
		else
		{
			grow_weight_x = Random(30, 70);
			grow_weight_y = Random(60, 100);
		}

		grow_loop = Random(Grow_Min_Loop, Grow_Max_Loop);
		CCLOG("grow %d/%d (%d)", grow_weight_x, grow_weight_y, grow_loop);
		InitCellGrid();

		CellPos startpos = CellPos(map_length / 2, map_width / 2);
		PushCell(startpos.x, startpos.y);
		//FillCell(startpos, Color4F::GREEN);
		int loop = 0;
		while (!pre_cell_list.empty())
		{
			after_cell_list.swap(pre_cell_list);
			while (!after_cell_list.empty()){
				auto pos = after_cell_list.front();
				//CCLOG("front %d,%d", pos.x, pos.y);
				//Color4B filled_color = Color4B(255, 255, 255, 255 - loop * 8);
				FillCell(pos, Color4F::GRAY);
				after_cell_list.pop();
			}
			loop++;
			if (loop >= grow_loop){
				break;
			}
		}
		CCLOG("loop = %d", loop);
	}
}

void MapScene::InitCellGrid(){
	if (map_width > 0 && map_length > 0){
		cell_grid.resize(map_length);
		for (int i = 0; i < map_length; i++){
			CellArray cell_array(map_width);
			cell_grid[i] = cell_array;
			for (int j = 0; j < map_width; j++){
				cell_grid[i][j] = CellInfo(i, j);
			}
		}
	}
	CCLOG("map size %d,%d", cell_grid.size(), cell_grid[0].size());
}

void MapScene::FillCell(CellPos& pos, const Color4F &color){
	if (pos.x >= 0 && pos.x < map_length && pos.y >= 0 && pos.y < map_width){
		if (cell_grid[pos.x][pos.y].filled == CellInfo::PreFill){
			DrawCell(pos.x, pos.y, color);
			cell_grid[pos.x][pos.y].filled = CellInfo::Filled;
	//		CCLOG("draw pos %d,%d", pos.x, pos.y);
			RecordCellsAround(pos.x, pos.y);
		}
		else{
			CCLOG("redraw pos %d,%d", pos.x, pos.y);
		}
	}
}

void MapScene::RecordCellsAround(int x, int y){
	if (x >= 0 && x < map_length && y >= 0 && y < map_width){
		if (x - 1 >= 0){
			int ran = Random(0,100);
			if (ran < grow_weight_x){
				PushCell(x - 1, y);
			}
		}
		if (x + 1 < map_length){
			int ran = Random(0, 100);
			if (ran < grow_weight_x){
				PushCell(x + 1, y);
			}
		}
		if (y - 1 >= 0){
			int ran = Random(0, 100);
			if (ran < grow_weight_y){
				PushCell(x, y - 1);
			}
		}
		if (y + 1 < map_width){
			int ran = Random(0, 100);
			if (ran < grow_weight_y){
				PushCell(x, y + 1);
			}
		}
	}
}

void MapScene::PushCell(int x,int y){
	if (cell_grid[x][y].filled == CellInfo::UnFilled){
		pre_cell_list.push(CellPos(x, y));
		cell_grid[x][y].filled = CellInfo::PreFill;
	}
}

void MapScene::DrawCell(int x, int y, const Color4F &color){
	Vec2 origin = Vec2(x*Cell_Len, y*Cell_Len);
	my_drawNode->drawRect(origin, origin + Cell_Rect, Color4F::MAGENTA);
	my_drawNode->drawSolidRect(origin, origin + Cell_Rect, color);
}

void MapScene::InitMap(int width, int length)
{
	if (width > 0 && length > 0){
		map_width = width;
		map_length = length;
		InitCellGrid();
		InitPlate();
		DrawMap();
	}
}

void MapScene::InitPlate(){
	int x = map_width / 2;
	int y = map_length / 2;

	MapPlateGroup plate_group(x, y, my_drawNode);
	plate_group.FilledCellGrid(cell_grid);
}

void MapScene::DrawMap()
{
	for (int i = 0; i < cell_grid.size();i++){
		for (int j = 0; j < cell_grid[i].size(); j++){
			if (cell_grid[i][j].filled == CellInfo::Filled){
				if (cell_grid[i][j].bound){
					DrawCell(i, j, Color4F::WHITE);
				}else{
					switch (cell_grid[i][j].value)
					{
					case Plate_Center:
						DrawCell(i, j, Color4F::GREEN);
						break;
					case Group_Center:
						DrawCell(i, j, Color4F::RED);
						break;
					default:
						DrawCell(i, j, Color4F::GRAY);
						break;
					}
				}
			}
		}
	}
}
