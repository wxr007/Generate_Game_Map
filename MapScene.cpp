#pragma execution_character_set("utf-8")
#include "MapScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

const Vec2 Cell_Rect = Vec2(Cell_Len, Cell_Len);
//mini	8400*2400
//mid	12600*3600
//large 16800*4800
const int Map_Width = 48*4;						//地图高
const int Map_Length = 64*4;					//地图长

#define Grow_Max_Loop 40						//最大成长圈数
#define Grow_Min_Loop 20						//最小成长圈数

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
	this->addChild(bgColor);

	my_drawNode = DrawNode::create();
	bgColor->addChild(my_drawNode);

	// 1 创建一个事件监听器
	auto TouchListener = EventListenerTouchAllAtOnce::create();
	// 2 绑定触摸事件
	TouchListener->onTouchesBegan = CC_CALLBACK_2(MapScene::onTouchesBegan, this);// 触摸开始时触发
	TouchListener->onTouchesMoved = CC_CALLBACK_2(MapScene::onTouchesMoved, this);// 触摸移动时触发
	// 3 添加监听器
	_eventDispatcher->addEventListenerWithSceneGraphPriority(TouchListener, this);

	// 1 创建一个事件监听器
	auto listenerkeyPad = EventListenerKeyboard::create();//键盘监听
	// 2 绑定键盘事件
	listenerkeyPad->onKeyPressed = CC_CALLBACK_2(MapScene::onKeyPressed, this);
	listenerkeyPad->onKeyReleased = CC_CALLBACK_2(MapScene::onKeyReleased, this);
	// 3 添加监听器
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

	// 单点时，touches中只有一个Touch对象，所以通过touches[0]就可以得到触摸对象
	auto touch = touches[0];
	// 计算滑动过程中的滑动增量
	auto diff = touch->getDelta();
	// 得到当前bgSprite的位置
	auto currentPos = bgColor->getPosition();
	// 得到滑动后bgSprite应该所在的位置
	auto pos = currentPos + diff;
	// 得到此刻bgSprite的尺寸
	auto bgSpriteCurrSize = bgColor->getBoundingBox().size;

	//边界控制，约束pos的位置
// 	pos.x = MIN(pos.x, bgSpriteCurrSize.width * bgSprite->getAnchorPoint().x);
// 	pos.x = MAX(pos.x, -bgSpriteCurrSize.width + winSize.width + bgSpriteCurrSize.width * bgSprite->getAnchorPoint().x);
// 	pos.y = MIN(pos.y, bgSpriteCurrSize.height * bgSprite->getAnchorPoint().y);
// 	pos.y = MAX(pos.y, -bgSpriteCurrSize.height + winSize.height + bgSpriteCurrSize.height * bgSprite->getAnchorPoint().y);
	// 重设bgSprite位置
	bgColor->setPosition(pos);

	Vec2 origin = Director::getInstance()->getVisibleOrigin();

//	CCLOG("org %f,%f", origin.x,origin.y);
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
	MapPlate plate(x, y);
	FillPlateCell(plate);
	CCLOG("orgin Plate %d,%d ", x, y);
	int next_angle = 0;
	next_angle = AddNextPlate(x, y, next_angle);
	next_angle = AddNextPlate(x, y, next_angle);
}

int MapScene::AddNextPlate(int org_x, int org_y, int pre_angle){
	Vec2 unit = Vec2::UNIT_X;
	int angle = (Random(0, CircumferentialAngle - pre_angle) + pre_angle) % CircumferentialAngle;
	int distance = Random(Cell_Group, 4 * Cell_Group + 1);
	Vec2 next = unit.rotateByAngle(Vec2::ZERO, CC_DEGREES_TO_RADIANS(angle)) * distance;

	int next_x = int(next.x) + org_x;
	int next_y = int(next.y) + org_y;

	MapPlate plate(next_x, next_y);
	FillPlateCell(plate);
	CCLOG("next Plate %d,%d %d %d", next_x, next_y, angle, distance);
	return angle;
}

void MapScene::FillPlateCell(MapPlate& plate)
{
	for (MapPlate::CellList::iterator it = plate.PlateCells.begin(); it != plate.PlateCells.end(); it++){
		if (it->x >= 0 && it->x < map_length && it->y >= 0 && it->y < map_width){
			if (cell_grid[it->x][it->y].filled != CellInfo::Filled){
				cell_grid[it->x][it->y].filled = CellInfo::Filled;
				cell_grid[it->x][it->y].bound = false;
			}else if (cell_grid[it->x][it->y].bound){
				cell_grid[it->x][it->y].bound = false;
			}
		}
	}
	for (MapPlate::CellList::iterator it = plate.BoundCells.begin(); it != plate.BoundCells.end(); it++){
		if (it->x >= 0 && it->x < map_length && it->y >= 0 && it->y < map_width){
			if (cell_grid[it->x][it->y].filled != CellInfo::Filled){
				cell_grid[it->x][it->y].filled = CellInfo::Filled;
				cell_grid[it->x][it->y].bound = true;
			}
		}
	}
}

void MapScene::DrawMap()
{
	for (int i = 0; i < cell_grid.size();i++){
		for (int j = 0; j < cell_grid[i].size(); j++){
			if (cell_grid[i][j].filled == CellInfo::Filled){
				if (cell_grid[i][j].bound){
					DrawCell(i, j, Color4F::WHITE);
				}else{
					DrawCell(i, j, Color4F::GRAY);
				}
			}
		}
	}
}
