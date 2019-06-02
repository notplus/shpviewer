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
#include <qwidget.h>
#include "paintWidget.h"
#include <qdockwidget.h>
#include <qlistwidget.h>
#include <qfileinfo.h>
#include <vector>
#include <qstring.h>
#include <qdesktopwidget.h>
#include <QtWidgets/QApplication>
#include <qdebug.h>
#include <qtextcodec.h>

class ShpViewer : public QMainWindow
{
	Q_OBJECT

public:
	ShpViewer(QWidget *parent = Q_NULLPTR);
	void initShape();
private:
	paintWidget *paintw;
	QDockWidget *dockw;
	QListWidget *listw;
	parseDBF dbfDATA;
	QAction *openAction;
	QAction *opendbfAction;
	size_t count;
	std::vector<QString> itemName;
	
private slots:
	void openFile();
	void openDBF();
	void dockRender(QListWidgetItem *item);
};

