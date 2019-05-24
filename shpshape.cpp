#include "shpshape.h"

void shppoint::render(qTree* node,Region region, size_t index, QPainter* painter)
{
	if (node->is_leaf)
	{
		for (int i = 0; i < node->ele_num; i++)
		{

			if (node->ele_list[i]->ele_point.y >= region.bottom &&
				node->ele_list[i]->ele_point.y <= region.up &&
				node->ele_list[i]->ele_point.x >= region.left &&
				node->ele_list[i]->ele_point.x <= region.right)
			{
				qreal x = (180 + node->ele_list[i]->ele_point.x)*scale[index] + transX[index];
				qreal y = (90 - node->ele_list[i]->ele_point.y)*scale[index] + transY[index];
				painter->drawPoint(QPointF(x, y));
			}
		}
		return;
	}
	else
	{
		render(node->lb, region, index, painter);
		render(node->rb, region, index, painter);
		render(node->lu, region, index, painter);
		render(node->ru, region, index, painter);
	}
}

void shpline::render(qTree* node, Region region, size_t index, QPainter* painter)
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
				qreal x1 = (180 + node->ele_list[i]->ele_line.x1)*scale[index] + transX[index];
				qreal y1 = (90 - node->ele_list[i]->ele_line.y1)*scale[index] + transY[index];
				qreal x2 = (180 + node->ele_list[i]->ele_line.x2)*scale[index] + transX[index];
				qreal y2 = (90 - node->ele_list[i]->ele_line.y2)*scale[index] + transY[index];
				painter->drawLine(QPointF(x1, y1), QPointF(x2, y2));
			}
		}
		return;
	}
	else
	{
		render(node->lb, region, index, painter);
		render(node->rb, region, index, painter);
		render(node->lu, region, index, painter);
		render(node->ru, region, index, painter);
	}
}

void shppolygon::render(qTree* node, Region region, size_t index, QPainter* painter)
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
				qreal x1 = (180 + node->ele_list[i]->ele_line.x1)*scale[index] + transX[index];
				qreal y1 = (90 - node->ele_list[i]->ele_line.y1)*scale[index] + transY[index];
				qreal x2 = (180 + node->ele_list[i]->ele_line.x2)*scale[index] + transX[index];
				qreal y2 = (90 - node->ele_list[i]->ele_line.y2)*scale[index] + transY[index];
				//qDebug() << x1 << " " << y1 << endl;
				painter->drawLine(QPointF(x1, y1), QPointF(x2, y2));
			}
		}
		return;
	}
	else
	{
		render(node->lb, region, index, painter);
		render(node->rb, region, index, painter);
		render(node->lu, region, index, painter);
		render(node->ru, region, index, painter);
	}
}
