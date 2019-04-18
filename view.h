#pragma once
#include <qframe.h>
#include <qgraphicsview.h>
#include <qslider.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qbuttongroup.h>
#include <qscrollbar.h>
#include <qopengl.h>

QT_BEGIN_NAMESPACE
class QLabel;
class QSlider;
class QToolButton;
QT_END_NAMESPACE

class View;

class GraphicsView :public QGraphicsView
{
	Q_OBJECT
public:
	GraphicsView(View *v) : QGraphicsView(),view(v) {}

protected:
#if QT_CONFIG(wheelevent)
	void wheelEvent(QWheelEvent *) override;
#endif

private:
	View *view;
};

class View :public QFrame
{
	Q_OBJECT
public:
	explicit View(const QString &name, QWidget *parent = Q_NULLPTR);
	GraphicsView *graphicsView;
	QGraphicsView *view() const;
	
public slots:
	void zoomIn(int level = 1);
	void zoomOut(int level = 1);

private slots:
	void resetView();
	void setResetButtonEnabled();
	void setupMatrix();
	void togglePointerMode();
	void toggleOpenGL();
	void toggleAntialiasing();
	//void print();
	void rotateLeft();
	void rotateRight();

private:

	QLabel *label;
	QLabel *label2;
	QToolButton *selectModeButton;
	QToolButton *dragModeButton;
	QToolButton *openGlButton;
	QToolButton *antialiasButton;
	//QToolButton *printButton;
	QToolButton *resetButton;
	QSlider *zoomSlider;
	QSlider *rotateSlider;
};