#include "view.h"

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#endif
#endif
#ifndef QT_NO_OPENGL
#include <QtOpenGL/qgl.h>
#else
#include <QtWidgets/qwidget.h>
#include <QtWidgets>
#endif
#include <qmath.h>
#include <QWheelEvent>

#if QT_CONFIG(wheelevent)
void GraphicsView::wheelEvent(QWheelEvent *e)
{
	if (e->modifiers() & Qt::ControlModifier) {
		if (e->delta() > 0)
			view->zoomIn(6);
		else
			view->zoomOut(6);
		e->accept();
	}
	else {
		QGraphicsView::wheelEvent(e);
	}
}
#endif

View::View(const QString &name, QWidget *parent)
	:QFrame(parent)
{
	setFrameStyle(Sunken | StyledPanel);
	graphicsView = new GraphicsView(this);
	graphicsView->setRenderHint(QPainter::Antialiasing, false);
	graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
	graphicsView->setOptimizationFlag(QGraphicsView::DontSavePainterState);
	graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
	graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

	zoomSlider = new QSlider;
	zoomSlider->setMinimum(0);
	zoomSlider->setMaximum(800);
	zoomSlider->setValue(200);
	zoomSlider->setTickPosition(QSlider::TicksRight);

	rotateSlider = new QSlider;
	rotateSlider->setOrientation(Qt::Horizontal);
	rotateSlider->setMinimum(-360);
	rotateSlider->setMaximum(360);
	rotateSlider->setValue(0);
	rotateSlider->setTickPosition(QSlider::TicksBelow);

	resetButton = new QToolButton;
	resetButton->setText(tr("0"));
	resetButton->setEnabled(false);

	QHBoxLayout *labelLayout = new QHBoxLayout;
	label = new QLabel(name);
	label2 = new QLabel(tr("Pointer Mode"));
	selectModeButton = new QToolButton;
	selectModeButton->setText(tr("Select"));
	selectModeButton->setCheckable(true);
	selectModeButton->setChecked(true);
	dragModeButton = new QToolButton;
	dragModeButton->setText(tr("Drag"));
	dragModeButton->setCheckable(true);
	dragModeButton->setChecked(false);
	antialiasButton = new QToolButton;
	antialiasButton->setText(tr("Antialiasing"));
	antialiasButton->setCheckable(true);
	antialiasButton->setChecked(false);
	openGlButton = new QToolButton;
	openGlButton->setText(tr("OpenGL"));
	openGlButton->setCheckable(true);
#ifndef  QT_NO_OPENGL
	openGlButton->setEnabled(QGLFormat::hasOpenGL());
#else
	openGlButton->setEnabled(false);
#endif
	
	QButtonGroup *pointerModeGroup = new QButtonGroup(this);
	pointerModeGroup->setExclusive(true);
	pointerModeGroup->addButton(selectModeButton);
	pointerModeGroup->addButton(dragModeButton);

	labelLayout->addWidget(label);
	labelLayout->addStretch();
	labelLayout->addWidget(label2);
	labelLayout->addWidget(selectModeButton);
	labelLayout->addWidget(dragModeButton);
	labelLayout->addStretch();
	labelLayout->addWidget(antialiasButton);
	labelLayout->addWidget(openGlButton);

	QGridLayout *topLayout = new QGridLayout;
	topLayout->addLayout(labelLayout, 0, 0);
	topLayout->addWidget(graphicsView, 1, 0);
	topLayout->addWidget(zoomSlider,1,1);
	topLayout->addWidget(rotateSlider,2,0);
	topLayout->addWidget(resetButton, 2, 1);
	setLayout(topLayout);

	connect(resetButton, SIGNAL(clicked()), this, SLOT(resetView()));
	connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
	connect(rotateSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
	connect(graphicsView->verticalScrollBar(), SIGNAL(valueChanged(int)),
		this, SLOT(setResetButtonEnabled()));
	connect(graphicsView->horizontalScrollBar(), SIGNAL(valueChanged(int)),
		this, SLOT(setResetButtonEnabled()));
	connect(selectModeButton, SIGNAL(toggled(bool)), this, SLOT(togglePointerMode()));
	connect(dragModeButton, SIGNAL(toggled(bool)), this, SLOT(togglePointerMode()));
	connect(antialiasButton, SIGNAL(toggled(bool)), this, SLOT(toggleAntialiasing()));
	connect(openGlButton, SIGNAL(toggled(bool)), this, SLOT(toggleOpenGL()));

	setupMatrix();
}

QGraphicsView *View::view() const
{
	return static_cast<QGraphicsView*>(graphicsView);
}

void View::resetView()
{
	zoomSlider->setValue(10);
	rotateSlider->setValue(0);
	setupMatrix();
	graphicsView->ensureVisible(QRectF(0, 0, 0, 0));

	resetButton->setEnabled(false);
}

void View::setResetButtonEnabled()
{
	resetButton->setEnabled(true);
}

void View::setupMatrix()
{
	qreal scale = qPow(qreal(10), (zoomSlider->value() - 800) / qreal(100));

	QMatrix matrix;
	matrix.scale(scale, scale);
	matrix.rotate(rotateSlider->value());

	graphicsView->setMatrix(matrix);
	setResetButtonEnabled();
}

void View::togglePointerMode()
{
	graphicsView->setDragMode(selectModeButton->isChecked()
		? QGraphicsView::RubberBandDrag
		: QGraphicsView::ScrollHandDrag);
	graphicsView->setInteractive(selectModeButton->isChecked());
}

void View::toggleOpenGL()
{
#ifndef  QT_NO_OPENGL
	graphicsView->setViewport(openGlButton->isChecked() ?
		new QGLWidget(QGLFormat(QGL::SampleBuffers)) : new QWidget);
#endif // ! QT_NO_OPENGL

}

void View::toggleAntialiasing()
{
	graphicsView->setRenderHint(QPainter::Antialiasing, antialiasButton->isChecked());
}

void View::zoomIn(int level)
{
	zoomSlider->setValue(zoomSlider->value() + level);
}

void View::zoomOut(int level)
{
	zoomSlider->setValue(zoomSlider->value() - level);
}

void View::rotateLeft()
{
	rotateSlider->setValue(rotateSlider->value() - 10);
}

void View::rotateRight()
{
	rotateSlider->setValue(rotateSlider->value() + 10);
}