#include "shpshape.h"

void shppoint::render()
{
	for (int i = 0; i < ele_num; i++)
	{
		qreal x = (180 + ele_list[i]->x) * scale[index] + transX[index];
		qreal y = (90 - ele_list[i]->y) * scale[index] + transY[index];
		painterS->drawPoint(QPointF(x, y));
	}
}

void shpline::render()
{
	for (int i = 0; i < numParts; i++)
	{
		QPainterPath path;
		int index_part = i < (numParts - 1) ? index_part = partsIndex[i + 1] - 1 : index_part = ele_num - 1;
		path.moveTo((180 + ele_list[partsIndex[i]]->x) * scale[index] + transX[index],
			(90 - ele_list[partsIndex[i]]->y) * scale[index] + transY[index]);
		for (int j = partsIndex[i]+1; j <= index_part; j++)
		{
			qreal x = (180 + ele_list[j]->x) * scale[index] + transX[index];
			qreal y = (90 - ele_list[j]->y) * scale[index] + transY[index];
			path.lineTo(x, y);
		}
		painterS->drawPath(path);
	}
}

void shppolygon::render()
{
	for (int i = 0; i < numParts; i++)
	{
		QPainterPath path;
		qreal x = (180 + ele_list[partsIndex[i]]->x) * scale[index] + transX[index];
		qreal y = (90 - ele_list[partsIndex[i]]->y) * scale[index] + transY[index];
		int index_part = i < (numParts - 1) ? index_part = partsIndex[i + 1] - 1 : index_part = ele_num - 1;
		path.moveTo(x,y);
		for (int j = partsIndex[i] + 1; j <= index_part; j++)
		{
			x = (180 + ele_list[j]->x) * scale[index] + transX[index];
			y = (90 - ele_list[j]->y) * scale[index] + transY[index];
			path.lineTo(x, y);
		}
		painterS->drawPath(path);
	}
}

shapeFactory* shapeFactory::_instance = 0;

shapeFactory* shapeFactory::Instance()
{
	if (_instance == 0)
	{
		_instance = new shapeFactory;
	}
	return _instance;
}

changeManager* changeManager::_instance = 0;

changeManager* changeManager::Instance()
{
	if (_instance == 0)
	{
		_instance = new changeManager;
		qTree a = creatRoot();
		_instance->TREE.push_back(a);
	}
	return _instance;
}