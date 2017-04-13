#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

#include "cocos2d.h"
#include "TypeDefine.h"

static int loop_count = 0;						//循环次数统计 (用于检查效率)
const int CircumferentialAngle = 360;			//圆周角度

//随机[min,max)
int Random(int min, int max);
//判断点是否在多边形内
bool pointInPolygon(float check_x, float check_y, std::vector<cocos2d::Vec2>& poly_points);
//计算y轴平行线和多边形的交点
void polygonIntersections(std::vector<cocos2d::Vec2>& poly_points, Intersections& points);


#endif