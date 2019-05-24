#include "paintWidget.h"
#include <QWheelEvent>
#include <qdebug.h>

paintWidget::paintWidget(QWidget *parent) :
	QWidget(parent)
{
	index = 0;

	zoomSlider = new QSlider;
	zoomSlider->setMinimum(1);
	zoomSlider->setMaximum(300);
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
	if (!shape.empty())
	{
		painter.rotate(rotateSlider->value());
		Region minRegion;
		initRegion(&minRegion, 0, 0, 0, 0);
		includeMin(&shpshape::tree[index], &region[index], &minRegion);
		//qDebug() << minRegion.up << " " << minRegion.bottom << " " << minRegion.left <<
		// " << minRegion.right << endl;
		qTree* view_tree;
		returnTree(&shpshape::tree[index], &minRegion, view_tree);
		//qDebug() << view_tree->depth << endl;
		shape[index][0]->render(view_tree, region[index], index,&painter);
	}
}

void paintWidget::updateScale(int)
{
	double lng = (region[index].left + region[index].right) / 2;
	double lat = (region[index].bottom + region[index].up) / 2;

	lng = (mousePos.x() - shpshape::transX[index]) / shpshape::scale[index] - 180;
	lat = 90 - (mousePos.y() - shpshape::transY[index]) / shpshape::scale[index];

	shpshape::scale[index] = zoomSlider->value();
	//shpshape::transX[index] = (180 + lng) * 4 - (180 + lng)*shpshape::scale[index];
	//shpshape::transY[index] = (90 - lat) * 4 - (90 - lat)*shpshape::scale[index];

	shpshape::transX[index] = mousePos.x() - (180 + lng)*shpshape::scale[index];
	shpshape::transY[index] = mousePos.y() - (90 - lat)*shpshape::scale[index];

	//double rl = 1440 /shpshape::scale[index];
	//double rh = 720 / shpshape::scale[index];
	double rl = this->size().width() / shpshape::scale[index];
	double rh = this->size().height() / shpshape::scale[index];
	region[index].up = lat + rh/2 ;
	region[index].bottom = lat - rh/2 ;
	region[index].left = lng - rl/2 ;
	region[index].right = lng + rl/2 ;
	//qDebug() << lng << " " << lat << endl;
	qDebug() << region[index].up << " " << region[index].bottom << " " << region[index].left
		<< " " << region[index].right << endl;
	//qDebug() << mousePos.x() << " " << mousePos.y() << endl;
	
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
	double lngg = (x - shpshape::transX[index]) / shpshape::scale[index] - 180;
	double latt = 90 - (y - shpshape::transY[index]) / shpshape::scale[index];
	//qDebug() << lng << " " << lat << " " << lngg << " " << latt << endl;
	//qDebug() << xc << " " << yc << endl;
	region[index].up =region[index].up- yc;
	region[index].bottom=region[index].bottom -yc;
	region[index].left =region[index].left- xc;
	region[index].right =region[index].right- xc;
	qDebug() << region[index].up << " " << region[index].bottom << " " << region[index].left
	<< " " << region[index].right << endl;
	shpshape::transX[index] = shpshape::transX[index] + x - mousePos.x();
	shpshape::transY[index] = shpshape::transY[index] + y - mousePos.y();
	update();
}

void paintWidget::zoomChange(int n)
{
	zoomSlider->setValue(zoomSlider->value() + n/10);
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
	qDebug() << event->x() << " " << event->y() << endl;
	double lng = (mousePos.x() - shpshape::transX[index]) / shpshape::scale[index] - 180;
	double lat = 90 - (mousePos.y() - shpshape::transY[index]) / shpshape::scale[index];
	qDebug() << lng << " " << lat << endl;
}

void paintWidget::updateRotate(int)
{
	angle[index] = rotateSlider->value();
	update();
}