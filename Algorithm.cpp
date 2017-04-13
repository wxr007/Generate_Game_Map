#pragma execution_character_set("utf-8")

#include <algorithm>
#include "Algorithm.h"

USING_NS_CC;

//随机[min,max)
int Random(int min, int max){
	return min == max ? min : min + (rand() % std::max(max - min, 0));
}

//判断点是否在多边形内
bool pointInPolygon(float check_x, float check_y, std::vector<Vec2>& poly_points) {
	int i, j = poly_points.size() - 1;
	bool oddNodes = false;
	for (i = 0; i < poly_points.size(); i++) {
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
void polygonIntersections(std::vector<Vec2>& poly_points, Intersections& points){
	int i, j = poly_points.size() - 1;
	for (i = 0; i < poly_points.size(); i++) {
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