#pragma once

#include <qgraphicsitem.h>
#include "quadTree.h"
#include "qpainter.h"
#include <vector>
#include <string>
using namespace std;

class shpItem :public QGraphicsItem
{
public:
	//shpItem(QGraphicsItem *parent = nullptr);
	shpItem(qTree*,int,int);
	//~shpItem();
	QRectF boundingRect() const override;
	//QPainterPath shape() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
	qTree *tree;
	friend class qTree;
	vector<string> dbfdata;
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	//void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
	int x;
	int y;
	
	//QVector<QPointF> stuff;
};