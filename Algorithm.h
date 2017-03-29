#pragma once
#pragma execution_character_set("utf-8")

#include <vector>
#include "cocos2d.h"

static int loop_count = 0;						//循环次数统计 (用于检查效率)
const int CircumferentialAngle = 360;			//圆周角度

//记录格子位置类型
struct CellPos
{
	CellPos(){
		x = 0;
		y = 0;
	}
	CellPos(int _x, int _y){
		x = _x;
		y = _y;
	}
	int x;
	int y;
};

struct CellInfo :CellPos
{
	enum {
		UnFilled = 0,	//未填充
		PreFill = 1,	//准备填充
		Filled = 2,		//已填充
	};

	CellInfo(int _x, int _y)
		:CellPos(_x, _y){
		filled = UnFilled;
		bound = false;
	}
	byte filled;//记录是否被填充
	bool bound;//是否边界
};

//交点
struct Intersections{
	float line_y;			//y坐标
	std::list<float>	list_x;		//和所有边交点的x坐标 从左到右排列
};

//随机[min,max)
int Random(int min, int max);

bool pointInPolygon(float check_x, float check_y, std::vector<cocos2d::Vec2>& poly_points);

void calculateIntersectionsWithPolygon(std::vector<cocos2d::Vec2>& poly_points, Intersections& points);