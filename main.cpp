#include "ShpViewer.h"

std::vector<int> shpshape::scale;
std::vector<double> shpshape::transX;
std::vector<double> shpshape::transY;
bool shpshape::__init = shpshape::init();

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

	ShpViewer w;
	w.show();
	return a.exec();
}
