#pragma once

#include "qTree.h"
#include <vector>
#include <qpainter.h>
#include <qdebug.h>



class shpshape : public abstractObject
{
public:
	virtual void render() = 0;
	std::vector<std::string> dbfdata;
	size_t index;
	QPainter* painterS;
	
	static std::vector<int> scale;
	static std::vector<double> transX;
	static std::vector<double> transY;

	static bool __init;
	static bool init() {
		
		scale.push_back(0);
		transX.push_back(0.0);
		transY.push_back(0.0);
		return true;
	}
};

class shppoint : public shpshape
{
public:
	virtual void render() override;

};

class shpline :public shpshape
{
public:
	virtual void render() override;

	int numParts;
	int* partsIndex;
};

class shppolygon:public shpshape
{
public:
	virtual void render() override;

	int numParts;
	int* partsIndex;
};

class shapeFactory
{
public:
	static shapeFactory* Instance();
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

private:
	static shapeFactory* _instance;
};

class changeManager
{
public:
	static changeManager* Instance();
	std::vector<qTree> TREE;
	std::vector<std::vector<abstractObject*>> shape;
private:
	static changeManager* _instance;

};