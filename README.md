# Generate_Game_Map
自己搞的随机地图生成算法


主要目的是生成一些随机形状的地图块


从y最小值到y最大值之间选取与x轴平行的平行线
计算x轴平行线与多边形的相交线，保存在列表中
列表对象机构大致:
list<Intersections> intersections_list;

struct Intersections{	//交点
	int line_y;			//y坐标
	list<x>	list_x;		//和所有边交点的x坐标 从左到右排列
	int left_x;		//最左边的交点
	int right_x;	//最右边的交点
}

计算方式：只要平行线y坐标在，多边形一条边的y最小值和y最大值之间
	line_y > min_y && line_y < max_y
求出交点
	(line_y - min_y) / (max_y - min_y) * (max_x - min_x) + min_x
只要记录算出的x坐标就行了，并且记录最大x和最小x

判断填充的时候，循环intersections_list，判断从left_x开始到right_x结束，判断list_x中有奇数个点在check_x的右侧，则填充




