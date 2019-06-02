#include "paintWidget.h"
#include <QWheelEvent>
#include <qdebug.h>

paintWidget::paintWidget(QWidget *parent) :
	QWidget(parent)
{
	index = 0;

	zoomSlider = new QSlider;
	zoomSlider->setMinimum(1);
	zoomSlider->setMaximum(200);
	zoomSlider->setTickPosition(QSlider::TicksRight);

	rotateSlider = new QSlider;
	rotateSlider->setOrientation(Qt::Horizontal);
	rotateSlider->setMinimum(-360);
	rotateSlider->setMaximum(360);
	rotateSlider->setValue(0);
	rotateSlider->setTickPosition(QSlider::TicksBelow);

	QGridLayout *layout = new QGridLayout;

	layout->addWidget(zoomSlider, 0, 1);
	layout->addWidget(rotateSlider, 1, 0);
	setLayout(layout);

	connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(updateScale(int)));
	connect(rotateSlider, SIGNAL(valueChanged(int)), this, SLOT(updateRotate(int)));

	
}

void paintWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.drawRect(0, 0, 1440, 720);
	if (!changeManager::Instance()->shape.empty())
	{
		painter.rotate(rotateSlider->value());
		Region minRegion;
		initRegion(&minRegion, 0, 0, 0, 0);
		includeMin(&changeManager::Instance()->TREE[index], &region[index], &minRegion);
		qTree* view_tree;
		returnTree(&changeManager::Instance()->TREE[index], &minRegion, view_tree);
		paintShape(view_tree, &painter);
	}
}

void paintWidget::paintShape(qTree* node, QPainter* painter)
{
	if (node->is_leaf)
		return;
	if (node->objects != NULL)
	{
		for (int i = 0; i < node->objects->size(); i++)
		{
			auto o = (*(node->objects))[i];
			((shpshape*)o)->painterS = painter;
			((shpshape*)o)->index = index;
			o->render();
		}
	}
	paintShape(node->lb, painter);
	paintShape(node->lu, painter);
	paintShape(node->rb, painter);
	paintShape(node->ru, painter);
}

void paintWidget::updateScale(int)
{
	double lng = (mousePos.x() - shpshape::transX[index]) / shpshape::scale[index] - 180;
	double lat = 90 - (mousePos.y() - shpshape::transY[index]) / shpshape::scale[index];

	shpshape::scale[index] = zoomSlider->value();
	shpshape::transX[index] = mousePos.x() - (180 + lng)*shpshape::scale[index];
	shpshape::transY[index] = mousePos.y() - (90 - lat)*shpshape::scale[index];

	double rl = 1440 /shpshape::scale[index];
	double rh = 720 / shpshape::scale[index];

	region[index].up = lat + rh/2 ;
	region[index].bottom = lat - rh/2;
	region[index].left = lng - rl/2;
	region[index].right = lng + rl/2;
	
	update();
}

void paintWidget::updateTrans(int x,int y)
{
	double xc = (x - shpshape::transX[index]) / shpshape::scale[index] - 180
		- ((mousePos.x() - shpshape::transX[index]) / shpshape::scale[index] - 180);
	double yc = 90 - (y - shpshape::transY[index]) / shpshape::scale[index]
		- (90 - (mousePos.y() - shpshape::transY[index]) / shpshape::scale[index]);
	double lng = (mousePos.x() - shpshape::transX[index]) / shpshape::scale[index] - 180;
	double lat = 90 - (mousePos.y() - shpshape::transY[index]) / shpshape::scale[index];

	region[index].up =region[index].up- yc;
	region[index].bottom=region[index].bottom -yc;
	region[index].left =region[index].left- xc;
	region[index].right =region[index].right- xc;
	shpshape::transX[index] = shpshape::transX[index] + x - mousePos.x();
	shpshape::transY[index] = shpshape::transY[index] + y - mousePos.y();
	update();
}

void paintWidget::zoomChange(int n)
{
	zoomSlider->setValue(zoomSlider->value() + n/30);
}

void paintWidget::wheelEvent(QWheelEvent *event)
{
	mousePos.setX(event->x());
	mousePos.setY(event->y());
	zoomChange(event->delta());
}

void paintWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton)
	{
		updateTrans(event->x(), event->y());
		mousePos.setX(event->x());
		mousePos.setY(event->y());
	}
}

void paintWidget::mousePressEvent(QMouseEvent *event)
{
	mousePos.setX(event->x());
	mousePos.setY(event->y());
}

void paintWidget::updateRotate(int)
{
	angle[index] = rotateSlider->value();
	update();
}