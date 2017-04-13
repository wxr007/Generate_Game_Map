#ifndef __TYPE_DEFINE_H__
#define __TYPE_DEFINE_H__

#include <deque>
#include <vector>
#include <list>

enum CellType {
	Plate_Center = 1,
	Group_Center = 2,
};

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

//格子信息
struct CellInfo :CellPos
{
	enum {
		UnFilled = 0,	//未填充
		PreFill = 1,	//准备填充
		Filled = 2,		//已填充
	};
	CellInfo()
		:CellPos(0, 0){
		filled = UnFilled;
		bound = false;
		value = 0;
	}
	CellInfo(int _x, int _y)
		:CellPos(_x, _y){
		filled = UnFilled;
		bound = false;
		value = 0;
	}
	byte filled;		//记录是否被填充
	bool bound;			//是否边界
	int32_t value;
};

//交点
struct Intersections{
	float line_y;					//y坐标
	std::list<float> list_x;		//和所有边交点的x坐标 从左到右排列
};

//格子行
typedef std::deque<CellInfo> CellArray;
//格子表
typedef std::deque<CellArray> CellGrid;
//格子位置队列
typedef std::queue<CellPos> CellQueue;
//格子位置链表
typedef std::list<CellPos> CellList;


#endif