#pragma execution_character_set("utf-8")
#include "MapScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

const int CircumferentialAngle = 360;			//圆周角度
const float Cell_Len = 16;						//格子大小
const Vec2 Cell_Rect = Vec2(Cell_Len, Cell_Len);

const int Min_Radial_Len = 100;
const int Max_Radial_Len = 300;

const int Map_Width = 24*4;						//地图高
const int Map_Length = 32*4;					//地图长

#define Grow_Max_Loop 40						//最大成长圈数
#define Grow_Min_Loop 20						//最小成长圈数

static int loop_count = 0;						//循环次数统计 (用于检查效率)

int Random(int min, int max){
	return min + (rand() % std::max(max - min,0));
}

//判断点是否在多边形内
bool pointInPolygon(float check_x, float check_y, std::vector<Vec2>& poly_points) {
	int i, j = poly_points.size() - 1;
	bool oddNodes = false;
	for (i = 0; i < poly_points.size(); i++) {
		loop_count++;
		if ((poly_points[i].y < check_y && poly_points[j].y > check_y || poly_points[j].y < check_y && poly_points[i].y > check_y)){		//水平线是否能相交
			if (poly_points[i].x <= check_x || poly_points[j].x <= check_x){													//只要计算在x左边的一半
				if (poly_points[i].x + (check_y - poly_points[i].y) / (poly_points[j].y - poly_points[i].y) * (poly_points[j].x - poly_points[i].x) < check_x){ //水平相交点的坐标在x的左边
					oddNodes = !oddNodes;
				}
			}
		}
		j = i;
	}
	return oddNodes;
}

//计算y轴平行线和多边形的交点
void calculateIntersectionsWithPolygon(std::vector<Vec2>& poly_points, Intersections& points){
	int i, j = poly_points.size() - 1;
	for (i = 0; i < poly_points.size(); i++) {
		loop_count++;
		if ((poly_points[i].y < points.line_y && poly_points[j].y >= points.line_y || poly_points[j].y < points.line_y && poly_points[i].y >= points.line_y)){		//水平线是否能相交
			float intersection_x = (points.line_y - poly_points[i].y) / (poly_points[j].y - poly_points[i].y) * (poly_points[j].x - poly_points[i].x) + poly_points[i].x;
			points.list_x.push_back(intersection_x);
		}
		j = i;
	}
	if (points.list_x.size() > 0){
		points.list_x.sort();
	}
}

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

// on "init" you need to initialize your instance
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
	bgColor->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
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

	map_width = 0;
	map_length = 0;
	srand(time(NULL));

	grow_weight_x = 0;
	grow_weight_y = 0;

//	DrawBasePolyByPoint();
	DrawBasePloyByRadian();
//	DrawTest();

//	DrawMap(Map_Width, Map_Length);

	return true;
}

void MapScene::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
{
// 	Vec2 pos = touches[0]->getLocation();
// 	float rad = (pos-Vec2(100,100)).getAngle();
// 	float ang = CC_RADIANS_TO_DEGREES(rad);
// 	CCLOG("pos(%f,%f) rad = %f ang = %f", pos.x, pos.y, rad, ang);
	my_drawNode->clear();
	DrawBasePloyByRadian();
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
bool compare_rad(const Vec2_Rad& first, const Vec2_Rad& second)
{
	if (first.rad > second.rad){
		return true;
	}
	else if (first.rad == second.rad && first.vec.y < second.vec.y){
		return true;
	}
	return false;
}
void MapScene::DrawBasePolyByPoint(){
	int min_x = 100;
	int min_y = 100;
	int max_x = 600;
	int max_y = 600;

	std::list<Vec2_Rad> points_list;
	int len = Random(5, 12);
	CCLOG("len = %d", len);
	Vec2_Rad min_point = Vec2_Rad(max_x, max_y);
	for (int i = 0; i < len; i++){
		Vec2_Rad new_point = Vec2_Rad(Random(min_x, max_x), Random(min_y, max_y));
		points_list.push_back(new_point);
		if (new_point.vec.x < min_point.vec.x)
		{
			min_point = new_point;
		}
		else if (new_point.vec.x == min_point.vec.x && new_point.vec.y < new_point.vec.y){
			min_point = new_point;
		}
	}
	for (std::list<Vec2_Rad>::iterator it = points_list.begin(); it != points_list.end(); it++){
		if (it->vec.x == min_point.vec.x && it->vec.y == min_point.vec.y){
			it->rad = 2;
			continue;
		}
		it->rad = (it->vec - min_point.vec).getAngle();		
		CCLOG("rad(%f,%f) = %f", it->vec.x, it->vec.y, it->rad);
	}
	CCLOG("sort");
	points_list.sort(compare_rad);

	std::vector<Vec2> poly_points;
	poly_points.reserve(len);
	for (std::list<Vec2_Rad>::iterator it = points_list.begin(); it != points_list.end(); it++){
		poly_points.push_back(it->vec);
		CCLOG("rad(%f,%f) = %f", it->vec.x, it->vec.y, it->rad);
	}
	my_drawNode->drawPolygon(poly_points.data(), len, Color4F::GRAY, 1, Color4F::MAGENTA);

	Vec2 mid_point = Vec2(350, 350);
	my_drawNode->drawDot(mid_point, 5, Color4F::RED);

	if (pointInPolygon(mid_point.x, mid_point.y, poly_points)){
		CCLOG("in the poly");
	}
	else{
		CCLOG("out the poly");
	}
}

void MapScene::DrawBasePloyByRadian(){
	loop_count = 0;
	int count = Random(7, 17);		//随机顶点数
	//CCLOG("count = %d", count);
	std::vector<Vec2> poly_points;
	poly_points.reserve(count);

	float avg_angle = CircumferentialAngle / count;		//平均角度
	int rand_angle = int(avg_angle);	//平均角度
	Vec2 unit = Vec2::UNIT_X;

	Vec2 min, max;
	for (int i = 0; i < count; i++){
		float ang = avg_angle*i + Random(0, rand_angle);
		Vec2 vec = unit.rotateByAngle(Vec2::ZERO, CC_DEGREES_TO_RADIANS(ang)) * Random(Min_Radial_Len, Max_Radial_Len);
		min.x = std::min(min.x, vec.x);
		min.y = std::min(min.y, vec.y);
		max.x = std::max(max.x, vec.x);
		max.y = std::max(max.y, vec.y);
		poly_points.push_back(vec);
		//CCLOG("point(%f,%f)", vec.x, vec.y);
	}
	//my_drawNode->drawPolygon(poly_points.data(), count, Color4F::GREEN, 1, Color4F::MAGENTA);

	//添加瓦片
	int min_x = (int(min.x) / Cell_Len);
	int max_x = (int(max.x) / Cell_Len) + 1;
	int min_y = (int(min.y) / Cell_Len);
	int max_y = (int(max.y) / Cell_Len) + 1;

	//CCLOG("(%d,%d) (%d,%d) %d x %d", min_x, min_y, max_x, max_y ,(max_x-min_x),(max_y - min_y));

// 	for (int j = min_y; j < max_y; ++j){
// 		for (int i = min_x; i < max_x; ++i){
// 			if (pointInPolygon(i*Cell_Len, j*Cell_Len, poly_points)){
// 				DrawCell(i, j, Color4F::GRAY);
// 			}
// 		}
// 	}

	//获取交点队列
	std::vector<Intersections> intersection_list;
	intersection_list.reserve(max_y-min_y);
	for (int j = min_y; j < max_y; ++j){	//计算平行线的相交线
		my_drawNode->drawLine(Vec2(min.x, j*Cell_Len), Vec2(max.x, j*Cell_Len), Color4F::BLUE);
		Intersections inter;
		inter.line_y = j*Cell_Len;
		calculateIntersectionsWithPolygon(poly_points, inter);
		if (inter.list_x.size() > 0){
			intersection_list.push_back(inter);
		}
	}
	//以交点填充瓦片
	int cell_count = 0;
	for (std::vector<Intersections>::iterator it = intersection_list.begin(); it != intersection_list.end(); it++){
		int row_y = int(it->line_y) / Cell_Len;
		//CCLOG("row_y = %d list_x = %d", row_y, it->list_x.size());
		while (it->list_x.size()){
			int col_x1 = int(it->list_x.front()) / Cell_Len;
			it->list_x.pop_front();
			if (it->list_x.size() == 0){
				break;
			}
			int col_x2 = int(it->list_x.front()) / Cell_Len;
			it->list_x.pop_front();
//			CCLOG("row_y = %d col_x1 = %d, col_x2 = %d", row_y, col_x1, col_x2);
			for (int i = col_x1; i < col_x2; i++){
				DrawCell(i, row_y, Color4F::GRAY);
				loop_count++;
				cell_count++;
			}
		}
	}
	CCLOG("loop_count = %d", loop_count);
	CCLOG("cell_count = %d", cell_count);
}

void MapScene::DrawTest(){

//	auto glProgramState = getGLProgramState();

// 	auto array = PointArray::create(4);
// 	array->addControlPoint(Vec2(200, 200));
// 	array->addControlPoint(Vec2(400, 100));
//  array->addControlPoint(Vec2(-100, 100));
// 	array->addControlPoint(Vec2(600, -100));
// 	array->addControlPoint(Vec2(100, 500));

// 	my_drawNode->drawCatmullRom(array, 20, Color4F::GRAY);
// 
// 	my_drawNode->drawCardinalSpline(array, 1, 20, Color4F::RED);

// 	auto array = PointArray::create(3);
// 	array->addControlPoint(Vec2(200, 200));
// 	array->addControlPoint(Vec2(400, 400));
// 	array->addControlPoint(Vec2(600, 200));

//	my_drawNode->drawQuadBezier(Vec2(200, 200), Vec2(400, 100), Vec2(-100, 100), 20, Color4F(0.1, 1.0, 0.2, 1));

//	my_drawNode->drawSegment(poly_points[0], poly_points[1], 100, Color4F::YELLOW);
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
		initCellGrid();

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

void MapScene::initCellGrid(){
	if (map_width > 0 && map_length > 0){
		cell_grid.reserve(map_length);
		for (int i = 0; i < map_length; i++){
			CellArray cell_array;
			cell_grid.push_back(cell_array);
			cell_grid[i].reserve(map_width);
			for (int j = 0; j < map_width; j++){
				cell_grid[i].push_back(CellPosFill(i, j));
			}
		}
	}
}

void MapScene::FillCell(CellPos& pos, const Color4F &color){
	if (pos.x >= 0 && pos.x < map_length && pos.y >= 0 && pos.y < map_width){
		if (cell_grid[pos.x][pos.y].filled == CellPosFill::PreFill){
			DrawCell(pos.x, pos.y, color);
			cell_grid[pos.x][pos.y].filled = CellPosFill::Filled;
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

// 		if (x - 1 >= 0 && y - 1 >= 0){
// 			PushCell(x - 1, y - 1);
// 		}
// 		if (x - 1 >= 0 && y + 1 < map_width){
// 			PushCell(x - 1, y + 1);
// 		}
// 		if (x + 1 < map_length && y - 1 >= 0){
// 			PushCell(x + 1, y - 1);
// 		}
// 		if (x + 1 < map_length &&  y + 1 < map_width){
// 			PushCell(x + 1, y + 1);
// 		}
	}
}

void MapScene::PushCell(int x,int y){
	if (cell_grid[x][y].filled == CellPosFill::UnFilled){
		pre_cell_list.push(CellPos(x, y));
		cell_grid[x][y].filled = CellPosFill::PreFill;
	}
}

void MapScene::DrawCell(int x, int y, const Color4F &color){
	Vec2 origin = Vec2(x*Cell_Len, y*Cell_Len);
	my_drawNode->drawRect(origin, origin + Cell_Rect, Color4F::MAGENTA);
	my_drawNode->drawSolidRect(origin, origin + Cell_Rect, color);
}