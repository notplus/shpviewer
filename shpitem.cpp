#include "shpItem.h"
#include <QtWidgets>

shpItem::shpItem(qTree *tt,int x, int y)
{
	this->x = x;
	this->y = y;
	this->tree = tt;

	setFlags(ItemIsSelectable | ItemIsSelectable);
	setAcceptHoverEvents(true);
}

QRectF shpItem::boundingRect() const
{
	return QRectF(0, 0, 1e9, 1e9);
}

void shpItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(widget);
	Region region;
	initRegion(&region, 90, -90, -180, 180);
	const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());

	//painter = new QPainter;
	QPen pen;

	//painter->setPen(QPen(Qt::black));
	
	//painter->setBrush(Qt::white);
	if (lod <= 1e-5)
		pen.setWidth(1e5);
	else if (lod <= 1e-4)
		pen.setWidth(1e4);
	else if (lod <= 1e-3)
		pen.setWidth(1e3);
	else if (lod <= 1e-2)
		pen.setWidth(100);
	else if (lod < 1e-1)
		pen.setWidth(10);
	pen.setColor(Qt::black);
	painter->setPen(pen);
	tree->render(tree, region, painter);
}

void shpItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mousePressEvent(event);
	//update();
}

//void shpItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
//{
//	if (event->modifiers()&Qt::ShiftModifier)
//	{
//		//stuff << event->pos();
//		update();
//		return;
//	}
//	QGraphicsItem::mouseMoveEvent(event);
//}

void shpItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mouseReleaseEvent(event);
	//update();
}
