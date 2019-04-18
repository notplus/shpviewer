#pragma once
#include <qpainter.h>
#include <cstdlib>
#define MAX_ELE_NUM 100

struct Region
{
	double up;
	double bottom;
	double left;
	double right;
};

struct elePoint
{
	double lng;
	double lat;
};

struct eleLine
{
	double x1, y1;
	double x2, y2;
};

class qTree;
class qPoint;
class qLine;
class qPolygon;


struct element
{
	elePoint ele_point;
	eleLine ele_line;
};

class qTree
{
public:
	virtual void initNode(qTree* node, int depth, Region region);
	virtual void insertEle(qTree* node, element ele) = 0;
	virtual void splitNode(qTree *node);
	virtual qTree* createChild(qTree *node, double bottom, double up, double left, double right);
	virtual void render(qTree*node, Region region, QPainter *painter)=0;

	friend class qPoint;
	friend class qLine;
	friend class qPolygon;

protected:
	int depth;
	bool is_leaf;
	Region region;
	Region valid;
	qTree *lu;
	qTree *lb;
	qTree *ru;
	qTree *rb;
	int ele_num;
	element* ele_list[MAX_ELE_NUM];
};

class qPoint : public qTree
{
public:
	qPoint() = default;

	virtual void insertEle(qTree* node, element ele) override;
	virtual void render(qTree* node, Region region,QPainter *painter);
};

class qLine : public qTree
{
public:
	qLine() = default;

	virtual void insertEle(qTree* node, element ele) override;
	virtual void render(qTree* node, Region region,QPainter *painter) ;
};

class qPolygon : public qTree
{
public:
	qPolygon() = default;

	virtual void insertEle(qTree* node, element ele) override;
	virtual void render(qTree* node, Region region,QPainter *painter);
};

void initRegion(Region *region, double up, double bottom, double left, double right);