#include "ShpViewer.h"
#include <qlayout.h>
#include <qsplitter.h>
#include <qdebug.h>
#include <qtablewidget.h>
#include <qstatusbar.h>

ShpViewer::ShpViewer(QWidget *parent)
	: QMainWindow(parent)
{
	//setupUi(this);
	
	this->setWindowIcon(QIcon(":/ShpViewer/mainicon"));
	count = 0;

	paintw = new paintWidget;

	dockw = new QDockWidget("shp file",this);
	dockw->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);
	dockw->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	addDockWidget(Qt::LeftDockWidgetArea,dockw);

	listw = new QListWidget(dockw);
	dockw->setWidget(listw);
	connect(listw, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(dockRender(QListWidgetItem *)));

	openAction = new QAction(QIcon(":/doc-open"), tr("&Open..."), this);
	openAction->setShortcuts(QKeySequence::Open);
	openAction->setStatusTip(tr("Open an existing file"));
	connect(openAction, &QAction::triggered, this, &ShpViewer::openFile);
	//connect(openAction, &QAction::triggered, this, &ShpViewer::populateScene);
	
	opendbfAction = new QAction(tr("Open dbf"), this);
	opendbfAction->setStatusTip(tr("Open an dbf file"));
	connect(opendbfAction, &QAction::triggered, this, &ShpViewer::openDBF);

	QMenu *file = menuBar()->addMenu(tr("&File"));
	file->addAction(openAction);
	file->addAction(opendbfAction);

	QStatusBar *bar = statusBar();
	this->setCentralWidget(paintw);
	

	setWindowTitle(tr("Shpviewer"));
}

void ShpViewer::openFile()
{
	QString path = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("Shp Files(*.shp)"));
	if (!path.isEmpty()) {
		QFile file(path);
		if (!file.open(QIODevice::ReadOnly)) {
			QMessageBox::warning(this, tr("Read File"), tr("Cannot open file:\n%1").arg(path));
			return;
		}
		QFileInfo fi;
		fi = QFileInfo(path);
		QListWidgetItem *a = new QListWidgetItem(fi.fileName(), listw);
		itemName.push_back(fi.fileName());
		listw->addItem(a);
		
		Region r;
		paintw->shape.push_back(parseShp(file,&r, count));
		paintw->region.push_back(r);
		
		double cx = (paintw->region[count].left + paintw->region[count].right) / 2;
		double cy = (paintw->region[count].up + paintw->region[count].bottom) / 2;

		//int scaleX = (int)(1440 / ((paintw->region[count].right - paintw->region[count].left)*1));
		//int scaleY = (int)(720 / ((paintw->region[count].up - paintw->region[count].bottom)*1));
		int scaleX = (int)(paintw->width() / ((paintw->region[count].right - paintw->region[count].left)));
		int scaleY = (int)(paintw->height() / ((paintw->region[count].up - paintw->region[count].bottom)));
		int scale = scaleX < scaleY ? scaleX : scaleY;
		cx = (paintw->region[count].left + paintw->region[count].right) / 2;
		cy = (paintw->region[count].up + paintw->region[count].bottom) / 2;
		paintw->mousePos.setX((180+cx)*4);
		paintw->mousePos.setY((90-cy)*4);

		if (shpshape::scale.size() == 1)
		{
			shpshape::scale[count] = scale;
			shpshape::scale.push_back(scale);
			shpshape::transX[count] = (180 + cx) * 4 - (180 + cx)*shpshape::scale[count];
			shpshape::transY[count] = (90 - cy) * 4 - (90 - cy)*shpshape::scale[count];
		}
		else
		{
			shpshape::scale[count] = scale;
			shpshape::scale.push_back(scale);
			shpshape::transX.push_back((180 + cx) * 4 - (180 + cx)*shpshape::scale[count]);
			shpshape::transY.push_back((90 - cy) * 4 - (90 - cy)*shpshape::scale[count]);
		}
		
		paintw->angle.push_back(0);
		paintw->zoomSlider->setValue(scale);
		paintw->index = count;
		paintw->update();
		a->setSelected(true);
		paintw->updateTrans(paintw->size().width()/2, paintw->size().height()/2);
		paintw->mousePos.setX(paintw->size().width()/2);
		paintw->mousePos.setY(paintw->size().height()/2);

		count++;
		shpshape::tree.push_back(creatRoot());
		
		file.close();
	}
	else {
		QMessageBox::warning(this, tr("Path"), tr("You did not select any file."));
	}
}

void ShpViewer::openDBF()
{
	QString path = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("DBF Files(*.dbf)"));
	if (!path.isEmpty()) {
		QFile file(path);
		if (!file.open(QIODevice::ReadOnly)) {
			QMessageBox::warning(this, tr("Read File"), tr("Cannot open file:\n%1").arg(path));
			return;
		}
		dbfDATA = parse_dbf(file);
		
		if (!paintw->shape[paintw->index].empty())
		{
			for (size_t i = 0; i < paintw->shape[paintw->index].size(); i++)
			{
				paintw->shape[paintw->index][i]->dbfdata = dbfDATA.dbfdata[i];
			}
		}
		QTableWidget* table = new QTableWidget();//创建一个表格
		table->setWindowIcon(QIcon(":/ShpViewer/mainicon"));
		table->setRowCount(dbfDATA.dbfheader.recordsNum);    //设置行数
		table->setColumnCount(dbfDATA.dbfRecord.size()); //设置列数
		QStringList horlabel;
		for (size_t i = 0; i < dbfDATA.dbfRecord.size(); i++)
		{
			horlabel<<dbfDATA.dbfRecord[i].name;
		}
		table->setHorizontalHeaderLabels(horlabel);
		
		for (size_t i = 0; i < dbfDATA.dbfheader.recordsNum; i++)
		{
			for (size_t j = 0; j < dbfDATA.dbfRecord.size(); j++)
			{
				QString tttstr = QString(QString::fromLocal8Bit(dbfDATA.dbfdata[i][j].c_str()));
				table->setItem(i, j, new QTableWidgetItem(tttstr));
			}	
		}
		table->show();
		file.close();
	}
	else {
		QMessageBox::warning(this, tr("Path"), tr("You did not select any file."));
	}
}

void ShpViewer::dockRender(QListWidgetItem *item)
{
	for (size_t i = 0; i < itemName.size(); i++)
	{
		if (itemName[i] == item->text())
		{
			paintw->index = &itemName[i] - &itemName[0];
			paintw->zoomSlider->setValue(shpshape::scale[paintw->index]);
			paintw->rotateSlider->setValue(paintw->angle[paintw->index]);
		}
	}
	paintw->update();
}
