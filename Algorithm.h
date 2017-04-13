#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

#include "cocos2d.h"
#include "TypeDefine.h"

static int loop_count = 0;						//ѭ������ͳ�� (���ڼ��Ч��)
const int CircumferentialAngle = 360;			//Բ�ܽǶ�

//���[min,max)
int Random(int min, int max);
//�жϵ��Ƿ��ڶ������
bool pointInPolygon(float check_x, float check_y, std::vector<cocos2d::Vec2>& poly_points);
//����y��ƽ���ߺͶ���εĽ���
void polygonIntersections(std::vector<cocos2d::Vec2>& poly_points, Intersections& points);


#endif