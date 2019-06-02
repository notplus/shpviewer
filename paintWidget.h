#pragma once
#include <qpainter.h>
#include <qwidget.h>
#include <qslider.h>
#include "shpshape.h"
#include <qlayout.h>
#include <QMouseEvent>

class paintWidget : public QWidget
{
	Q_OBJECT
public:
	paintWidget(QWidget *parent = nullptr);
	friend class graphicsView;
	void wheelEvent(QWheelEvent *event);
	void zoomChange(int n);
	void mouseMoveEvent(QMouseEvent *event);
	void updateTrans(int ,int);
	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *event);
	void paintShape(qTree* node, QPainter* painter);
public slots:
	void updateScale(int i=1);
	void updateRotate(int i = 1);

public:
	size_t index;
	QSlider *zoomSlider;
	QSlider *rotateSlider;
	QPoint mousePos;
	std::vector<Region> region;
	std::vector<int> angle;
};

