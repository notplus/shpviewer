#pragma once

#include <cstdlib>
#include <cmath>
#include <vector>
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
	double x;
	double y;
};

class abstractObject 
{
public:
	virtual void render()=0;

	Region region;
	int ele_num;
	std::vector<elePoint*> ele_list;
};


class qTree
{
public:
	void initNode(qTree* node, int depth, Region region);
	void insertEle(qTree* node, elePoint* ele);
	qTree* createChild(qTree *node, double bottom, double up, double left, double right);
	void splitNode(qTree* node);

	int depth;
	bool is_leaf;
	Region region;
	qTree *lu;
	qTree *lb;
	qTree *ru;
	qTree *rb;
	int ele_num;
	elePoint* ele_list[MAX_ELE_NUM];
	std::vector<abstractObject*> *objects;
};

void initRegion(Region *region, double up, double bottom, double left, double right);
void includeMin(qTree* tree, Region* region, Region* min_include);
qTree creatRoot();
void returnTree(qTree* tree, Region* region, qTree* &ret);

