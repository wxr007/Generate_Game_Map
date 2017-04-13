#ifndef __TYPE_DEFINE_H__
#define __TYPE_DEFINE_H__

#include <deque>
#include <vector>
#include <list>

enum CellType {
	Plate_Center = 1,
	Group_Center = 2,
};

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

//������Ϣ
struct CellInfo :CellPos
{
	enum {
		UnFilled = 0,	//δ���
		PreFill = 1,	//׼�����
		Filled = 2,		//�����
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
	byte filled;		//��¼�Ƿ����
	bool bound;			//�Ƿ�߽�
	int32_t value;
};

//����
struct Intersections{
	float line_y;					//y����
	std::list<float> list_x;		//�����б߽����x���� ����������
};

//������
typedef std::deque<CellInfo> CellArray;
//���ӱ�
typedef std::deque<CellArray> CellGrid;
//����λ�ö���
typedef std::queue<CellPos> CellQueue;
//����λ������
typedef std::list<CellPos> CellList;


#endif