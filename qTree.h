#pragma once

#include <cstdlib>
#include <cmath>
#define MAX_ELE_NUM 100

struct Region
{
	double up;
	double bottom;
	double left;
	double right;
	void operator=(const Region& b);
	bool operator==(const Region& b);
	bool operator<(const Region& b);
	bool operator<=(const Region& b);
};

struct elePoint
{
	double x=1000;
	double y;
};

struct eleLine
{
	double x1, y1;
	double x2, y2;
};

struct element
{
	elePoint ele_point;
	eleLine ele_line;
};

class qTree
{
public:
	void initNode(qTree* node, int depth, Region region);
	void insertEle(qTree* node, elePoint ele);
	void insertEle(qTree* node, eleLine ele);
	void splitNode(qTree *node);
	qTree* createChild(qTree *node, double bottom, double up, double left, double right);

	int depth;
	bool is_leaf;
	Region region;
	qTree *lu;
	qTree *lb;
	qTree *ru;
	qTree *rb;
	int ele_num;
	element* ele_list[MAX_ELE_NUM];
};

void initRegion(Region *region, double up, double bottom, double left, double right);
void includeMin(qTree* tree, Region* region, Region* min_include);
qTree creatRoot();
void returnTree(qTree* tree, Region* region, qTree* &ret);