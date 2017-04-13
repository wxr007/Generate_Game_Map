#include "MapPlateGroup.h"

USING_NS_CC;

MapPlateGroup::MapPlateGroup(int x, int y, DrawNode* node)
	:plate_pos(x,y)
	,my_drawNode(node){
	InitPlates();
}

MapPlateGroup::~MapPlateGroup(){
	for (int i = 0; i < plate_array.size();i++){
		delete plate_array[i];
	}
	plate_array.clear();
}

void MapPlateGroup::InitPlates(){
	AddPlate(plate_pos.x, plate_pos.y);
	int next_angle = 0;
	for (int i = 0; i < 2; i++){
		next_angle = AddAroundPlate(plate_pos.x, plate_pos.y, next_angle);
	}
}

void MapPlateGroup::AddPlate(int x, int y){
	MapPlate* plate =  new MapPlate(x, y);
	plate_array.push_back(plate);
	CCLOG("orgin Plate %d,%d ", x, y);
}

int MapPlateGroup::AddAroundPlate(int org_x, int org_y, int pre_angle){
	Vec2 unit = Vec2::UNIT_X;
	int used_angle = pre_angle == 0 ? 0 : 120;
	int angle = (Random(0, CircumferentialAngle - used_angle) + pre_angle + 60) % CircumferentialAngle;
	int distance = Random(2 * Cell_Group, 4 * Cell_Group + 1);
	Vec2 next = unit.rotateByAngle(Vec2::ZERO, CC_DEGREES_TO_RADIANS(angle)) * distance;

	int next_x = int(next.x) + org_x;
	int next_y = int(next.y) + org_y;

	my_drawNode->drawLine(Vec2(org_x*Cell_Len, org_y*Cell_Len), Vec2(next_x*Cell_Len, next_y*Cell_Len), Color4F::RED);

	MapPlate* plate = new MapPlate(next_x, next_y);
	plate_array.push_back(plate);
	CCLOG("next Plate %d(%f),%d(%f) %d %d", next_x, next.x, next_y, next.y, angle, distance);
	return angle;
}

void MapPlateGroup::FilledCellGrid(CellGrid& cell_grid){
	for (int i = 0; i < plate_array.size(); i++){
		FilledPlateCells(plate_array[i], cell_grid);
	}
}

void MapPlateGroup::FilledPlateCells(MapPlate* plate, CellGrid& cell_grid){
	for (CellList::iterator it = plate->PlateCells.begin(); it != plate->PlateCells.end(); it++){
		if (it->x >= 0 && it->x < cell_grid.size() && it->y >= 0 && it->y < cell_grid.size()){
			if (cell_grid[it->x][it->y].filled != CellInfo::Filled){
				cell_grid[it->x][it->y].filled = CellInfo::Filled;
				cell_grid[it->x][it->y].bound = false;
			}else if (cell_grid[it->x][it->y].bound){
				cell_grid[it->x][it->y].bound = false;
			}
			if (plate->CenterPos.x == it->x && plate->CenterPos.y == it->y){
				cell_grid[it->x][it->y].value = Plate_Center;
			}
			if (plate_pos.x == it->x && plate_pos.y == it->y){
				cell_grid[it->x][it->y].value = Group_Center;
			}
		}
	}
	for (CellList::iterator it = plate->BoundCells.begin(); it != plate->BoundCells.end(); it++){
		if (it->x >= 0 && it->x < cell_grid.size() && it->y >= 0 && it->y < cell_grid.size()){
			if (cell_grid[it->x][it->y].filled != CellInfo::Filled){
				cell_grid[it->x][it->y].filled = CellInfo::Filled;
				cell_grid[it->x][it->y].bound = true;
			}
		}
	}
}