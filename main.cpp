#include "ShpViewer.h"
#include <QtWidgets/QApplication>
#include <qdebug.h>
#include <qtextcodec.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

	ShpViewer w;
	w.show();
	return a.exec();
}
