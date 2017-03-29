#pragma once
#pragma execution_character_set("utf-8")

#include <vector>
#include "cocos2d.h"

static int loop_count = 0;						//ѭ������ͳ�� (���ڼ��Ч��)
const int CircumferentialAngle = 360;			//Բ�ܽǶ�

//��¼����λ������
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
		UnFilled = 0,	//δ���
		PreFill = 1,	//׼�����
		Filled = 2,		//�����
	};

	CellInfo(int _x, int _y)
		:CellPos(_x, _y){
		filled = UnFilled;
		bound = false;
	}
	byte filled;//��¼�Ƿ����
	bool bound;//�Ƿ�߽�
};

//����
struct Intersections{
	float line_y;			//y����
	std::list<float>	list_x;		//�����б߽����x���� ����������
};

//���[min,max)
int Random(int min, int max);

bool pointInPolygon(float check_x, float check_y, std::vector<cocos2d::Vec2>& poly_points);

void calculateIntersectionsWithPolygon(std::vector<cocos2d::Vec2>& poly_points, Intersections& points);