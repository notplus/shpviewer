#pragma once

#include <QtWidgets/QMainWindow>
#include <qaction.h>
#include <qmenu.h>
#include <qmenubar.h>
#include <qdialog.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include "parseShp.h"
#include "shpItem.h"
#include <qwidget.h>
#include "view.h"
QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QSplitter;
QT_END_NAMESPACE

class ShpViewer : public QMainWindow
	//,
	//public Ui::ShpViewerClass
{
	Q_OBJECT

public:
	ShpViewer(QWidget *parent = Q_NULLPTR);
	vector<shpItem*> shpitems;

private:
	//Ui::ShpViewerClass ui;
	void populateScene();
	void setupMatrix();
	void parse();

	parseDBF dbfDATA;
	QAction *openAction;
	QAction *opendbfAction;
	View *view;
	QGraphicsScene *scene;
	//QSplitter *h1Splitter;
	//QSplitter *h2Splitter;
private slots:
	void openFile();
	void openDBF();
};
