#pragma execution_character_set("utf-8")

#include "MapPlate.h"

USING_NS_CC;

const int MinVertexsCount = 7;		//最小顶点数
const int MaxVertexsCount = 17;		//最多顶点数

const int Min_Radial_Len = 5 * Cell_Len;		//最短放射长度
const int Max_Radial_Len = 20 * Cell_Len;		//最长放射长度


MapPlate::MapPlate(int x, int y)
	:CenterPos(x,y){
	DrawBasePloy();
	GetIntersections();
	FillCells();
	CCLOG("center :[%d,%d]", CenterPos.x, CenterPos.y);
}
MapPlate::~MapPlate(){
	poly_points.clear();
	intersection_list.clear();
	PlateCells.clear();
	BoundCells.clear();
}

void MapPlate::DrawBasePloy(){
	int count = Random(MinVertexsCount, MaxVertexsCount);//随机顶点数
	poly_points.reserve(count);

	float avg_angle = CircumferentialAngle / count;		//平均角度
	int rand_angle = int(avg_angle);					//平均角度
	Vec2 unit = Vec2::UNIT_X;

	Vec2 offset(CenterPos.x*Cell_Len, CenterPos.y*Cell_Len);
	Vec2 min = offset;
	Vec2 max = offset;
	for (int i = 0; i < count; i++){
		float ang = avg_angle*i + Random(0, rand_angle);
		Vec2 vec = unit.rotateByAngle(Vec2::ZERO, CC_DEGREES_TO_RADIANS(ang)) * Random(Min_Radial_Len, Max_Radial_Len);
		vec += offset;
		min.x = std::min(min.x, vec.x);
		min.y = std::min(min.y, vec.y);
		max.x = std::max(max.x, vec.x);
		max.y = std::max(max.y, vec.y);
		poly_points.push_back(vec);						//压入顶点
	}
	MinPos.x = (int(min.x) / Cell_Len);
	MaxPos.x = (int(max.x) / Cell_Len) + 1;
	MinPos.y = (int(min.y) / Cell_Len);
	MaxPos.y = (int(max.y) / Cell_Len) + 1;
}

void MapPlate::GetIntersections(){
	intersection_list.reserve(MaxPos.y - MinPos.y);
	for (int j = MinPos.y; j <= MaxPos.y; ++j){			//计算平行线的相交线
		Intersections inter;
		inter.line_y = j*Cell_Len;
		calculateIntersectionsWithPolygon(poly_points, inter);
		if (inter.list_x.size() > 0){
			intersection_list.push_back(inter);
		}
	}
	CCLOG("area :[%d,%d],[%d,%d]", MinPos.x, MinPos.y, MaxPos.x, MaxPos.y);
}

void MapPlate::FillCells(){
	int cell_count = 0;
	for (std::vector<Intersections>::iterator it = intersection_list.begin(); it != intersection_list.end(); it++){
		int row_y = int(it->line_y) / Cell_Len;
		while (it->list_x.size()){
			int col_x1 = int(it->list_x.front()) / Cell_Len;
			it->list_x.pop_front();
			if (it->list_x.size() == 0){
				break;
			}
			int col_x2 = int(it->list_x.front()) / Cell_Len;
			it->list_x.pop_front();
			for (int i = col_x1; i <= col_x2; i++){
				if (i == col_x1 || i == col_x2){
					BoundCells.push_back(CellPos(i, row_y));
				}else{
					PlateCells.push_back(CellPos(i, row_y));
				}
				cell_count++;
			}
		}
	}
	CCLOG("cell_count = %d", cell_count);
}