#include "quadTree.h"
#include "parseShp.h"
#include <qdebug.h>

extern header header_1;

void initRegion(Region *region, double up, double bottom, double left, double right)
{
	region->bottom = bottom;
	region->left = left;
	region->right = right;
	region->up = up;
}

void qTree::initNode(qTree* node, int depth, Region region)
{

	node->depth = depth;
	node->is_leaf = true;
	node->ele_num = 0;
	node->region = region;
}

void qPoint::insertEle(qTree* node, element ele)
{
	if (node->is_leaf)
	{
		if (node->ele_num + 1 > MAX_ELE_NUM)
		{
			splitNode(node);
			insertEle(node, ele);
		}
		else
		{
			element* ele_ptr = (element*)(malloc(sizeof(element)));
			ele_ptr->ele_point.lat = ele.ele_point.lat;
			ele_ptr->ele_point.lng = ele.ele_point.lng;
			node->ele_list[node->ele_num] = ele_ptr;
			node->ele_num++;
		}
		return;
	}

	double midHor = (node->region.up + node->region.bottom) / 2;
	double midVer = (node->region.right + node->region.left) / 2;
	if (ele.ele_point.lat > midHor)
	{
		if (ele.ele_point.lng > midVer)
			insertEle(node->ru, ele);
		else
			insertEle(node->lu, ele);
	}
	else
	{
		if (ele.ele_point.lng > midVer)
			insertEle(node->rb, ele);
		else
			insertEle(node->lb, ele);
	}
}

void qTree::splitNode(qTree *node)
{
	double midHor = (node->region.up + node->region.bottom) / 2;
	double midVer = (node->region.right + node->region.left) / 2;

	node->is_leaf = false;
	node->ru = createChild(node, midHor, node->region.up, midVer, node->region.right);
	node->rb = createChild(node, node->region.bottom, midHor, midVer, node->region.right);
	node->lu = createChild(node, midHor, node->region.up, node->region.left, midVer);
	node->lb = createChild(node, node->region.bottom, midHor, node->region.left, midVer);

	for (size_t i = 0; i < MAX_ELE_NUM; i++)
	{
		insertEle(node, *node->ele_list[i]);
		free(node->ele_list[i]);
		node->ele_num--;
	}
}

qTree* qTree::createChild(qTree *node, double bottom, double up, double left, double right)
{
	int depth = node->depth + 1;
	qTree *childNode = (qTree*)(malloc(sizeof(qTree)));
	Region *region = (Region*)(malloc(sizeof(Region)));
	initRegion(region, up, bottom, left, right);
	initNode(childNode, depth, *region);

	return childNode;
}

void qLine::insertEle(qTree* node, element ele)
{
	if (node->is_leaf)
	{
		if (node->ele_num + 1 > MAX_ELE_NUM)
		{
			splitNode(node);
			insertEle(node, ele);
		}
		else
		{
			element *ele_ptr = (element*)(malloc(sizeof(element)));
			ele_ptr->ele_line.x1 = ele.ele_line.x1;
			ele_ptr->ele_line.x2 = ele.ele_line.x2;
			ele_ptr->ele_line.y1 = ele.ele_line.y1;
			ele_ptr->ele_line.y2 = ele.ele_line.y2;
			node->ele_list[node->ele_num] = ele_ptr;
			node->ele_num++;
		}
		return;
	}

	double midHor = (node->region.up + node->region.bottom) / 2;
	double midVer = (node->region.right + node->region.left) / 2;
	if (ele.ele_line.y1 > midHor && ele.ele_line.y2 > midHor)
	{
		if (ele.ele_line.x1 > midVer)
			insertEle(node->ru, ele);
		else
			insertEle(node->lu, ele);
	}
	else
	{
		if (ele.ele_line.x1 > midVer)
			insertEle(node->rb, ele);
		else
			insertEle(node->lb, ele);
	}

}

void qPolygon::insertEle(qTree* node, element ele)
{
	if (node->is_leaf)
	{
		if (node->ele_num + 1 > MAX_ELE_NUM)
		{
			splitNode(node);
			insertEle(node, ele);
		}
		else
		{
			element *ele_ptr = (element*)(malloc(sizeof(element)));
			ele_ptr->ele_line.x1 = ele.ele_line.x1;
			ele_ptr->ele_line.x2 = ele.ele_line.x2;
			ele_ptr->ele_line.y1 = ele.ele_line.y1;
			ele_ptr->ele_line.y2 = ele.ele_line.y2;
			node->ele_list[node->ele_num] = ele_ptr;
			node->ele_num++;
		}
		return;
	}

	double midHor = (node->region.up + node->region.bottom) / 2;
	double midVer = (node->region.right + node->region.left) / 2;
	if (ele.ele_line.y1 > midHor && ele.ele_line.y2 > midHor)
	{
		if (ele.ele_line.x1 > midVer)
			insertEle(node->ru, ele);
		else
			insertEle(node->lu, ele);
	}
	else
	{
		if (ele.ele_line.x1 > midVer)
			insertEle(node->rb, ele);
		else
			insertEle(node->lb, ele);
	}

}

void qPoint::render(qTree* node, Region region,QPainter *painter)
{
	if (node->is_leaf)
	{
		for (int i = 0; i < node->ele_num; i++)
		{
			
			if (node->ele_list[i]->ele_point.lng >= region.bottom &&
				node->ele_list[i]->ele_point.lng <= region.up &&
				node->ele_list[i]->ele_point.lat >= region.left &&
				node->ele_list[i]->ele_point.lat <= region.right)
			{
				qreal lng = (180+node->ele_list[i]->ele_point.lng)*1e6;
				qreal lat = (90-node->ele_list[i]->ele_point.lat)*1e6;
				painter->drawPoint(QPointF(lng, lat));
			}
		}
		return;
	}
	else
	{
		render(node->lb, region,painter);
		render(node->rb, region,painter);
		render(node->lu, region,painter);
		render(node->ru, region,painter);
	}
}

void qLine::render(qTree* node, Region region,QPainter *painter)
{
	double xl = header_1.header_2[2] - header_1.header_2[0];
	double yl = header_1.header_2[3] - header_1.header_2[1];
	double xyr = xl < yl ? yl : xl;
	double xmid = (header_1.header_2[0] + header_1.header_2[2]) / 2;
	double ymid = (header_1.header_2[1] + header_1.header_2[3]) / 2;

	if (node->is_leaf)
	{
		for (int i = 0; i < node->ele_num; i++)
		{
			if (node->ele_list[i]->ele_line.y1 >= region.bottom &&
				node->ele_list[i]->ele_line.y1 <= region.up &&
				node->ele_list[i]->ele_line.x1 >= region.left &&
				node->ele_list[i]->ele_line.x1 <= region.right &&
				node->ele_list[i]->ele_line.y2 >= region.bottom &&
				node->ele_list[i]->ele_line.y2 <= region.up &&
				node->ele_list[i]->ele_line.x2 >= region.left &&
				node->ele_list[i]->ele_line.x2 <= region.right)
			{
				qreal x1 = (180 + node->ele_list[i]->ele_line.x1) * 1e6;
				qreal y1 = (90 - node->ele_list[i]->ele_line.y1) * 1e6;
				qreal x2 = (180 + node->ele_list[i]->ele_line.x2) * 1e6;
				qreal y2 = (90 - node->ele_list[i]->ele_line.y2) * 1e6;
				painter->drawLine(QPointF(x1, y1), QPointF(x2, y2));
			}
		}
		return;
	}
	else
	{
		render(node->lb, region,painter);
		render(node->rb, region,painter);
		render(node->lu, region,painter);
		render(node->ru, region,painter);
	}
}

void qPolygon::render(qTree* node, Region region,QPainter *painter)
{
	if (node->is_leaf)
	{
		for (int i = 0; i < node->ele_num; i++)
		{
			if (node->ele_list[i]->ele_line.y1 >= region.bottom &&
				node->ele_list[i]->ele_line.y1 <= region.up &&
				node->ele_list[i]->ele_line.x1 >= region.left &&
				node->ele_list[i]->ele_line.x1 <= region.right &&
				node->ele_list[i]->ele_line.y2 >= region.bottom &&
				node->ele_list[i]->ele_line.y2 <= region.up &&
				node->ele_list[i]->ele_line.x2 >= region.left &&
				node->ele_list[i]->ele_line.x2 <= region.right)
			{
				qreal x1 = (180+node->ele_list[i]->ele_line.x1) * 1e6;
				qreal y1 = (90-node->ele_list[i]->ele_line.y1) * 1e6;
				qreal x2 = (180+node->ele_list[i]->ele_line.x2) * 1e6;
				qreal y2 = (90-node->ele_list[i]->ele_line.y2) * 1e6;
				painter->drawLine(QPointF(x1, y1), QPointF(x2, y2));
			}
		}
		return;
	}
	else
	{
		render(node->lb, region,painter);
		render(node->rb, region,painter);
		render(node->lu, region,painter);
		render(node->ru, region,painter);
	}
}
