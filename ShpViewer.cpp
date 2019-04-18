#include "ShpViewer.h"
#include "view.h"
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

	View *view = new View("ShpViewer");

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
	scene = new QGraphicsScene(this);
	//populateScene();
	view->view()->setScene(scene);
	this->setCentralWidget(view);
	

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
		shpitems = parseSHP(file);
		for (auto it : shpitems)
		{
			QGraphicsItem *item = new shpItem(it->tree, 0, 0);
			scene->addItem(it);
		}
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
		if (!shpitems.empty())
		{
			for (size_t i = 0; i < shpitems.size(); i++)
			{
				shpitems[i]->dbfdata = dbfDATA.dbfdata[i];
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

void ShpViewer::populateScene()
{
	
}


void ShpViewer::parse()
{
	
}