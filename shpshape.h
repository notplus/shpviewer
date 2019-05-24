#pragma once

#include "qTree.h"
#include <vector>
#include <qpainter.h>
#include <qdebug.h>

class shpshape
{
public:
	virtual void render(qTree* node, Region region, size_t index, QPainter* painter)=0;
	static std::vector<qTree> tree;
	std::vector<std::string> dbfdata;
	Region region;
	
	static std::vector<int> scale;
	static std::vector<double> transX;
	static std::vector<double> transY;

	static bool __init;
	static bool init() {
		qTree a=creatRoot();
		tree.push_back(a);
		scale.push_back(0);
		transX.push_back(0.0);
		transY.push_back(0.0);
		return true;
	}
};



class shppoint : public shpshape
{
public:
	virtual void render(qTree* node, Region region, size_t index, QPainter* painter) override;

};

class shpline :public shpshape
{
public:
	virtual void render(qTree* node, Region region, size_t index, QPainter* painter) override;

	std::vector<int> partsIndex;
};

class shppolygon:public shpshape
{
public:
	virtual void render(qTree* node, Region region, size_t index, QPainter* painter) override;
	std::vector<int> partsIndex;

private:

};

class shapeFactory
{
public:
	//shapeFactory();

	virtual shppoint* createPoint() 
	{
		return new shppoint;
	}

	virtual shpline* createLine() 
	{
		return new shpline;
	}

	virtual shppolygon* createPolygon() 
	{
		return new shppolygon;
	}

};