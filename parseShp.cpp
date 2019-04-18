#include "parseShp.h"
#include "quadTree.h"
#include <qdebug.h>
int FILELENGTH;

header header_1;

int32_t swapInt32(int32_t value)
{
	return ((value & 0x000000FF) << 24) |
		((value & 0x0000FF00) << 8) |
		((value & 0x00FF0000) >> 8) |
		((value & 0xFF000000) >> 24);
}

vector<int> parse_header_1(QFile& in)
{
	unsigned int num;
	vector<int> header_1;
	for (size_t i = 0; i < 9; i++)
	{
		if (i < 7)
		{
			in.read((char*)&num, 4);
			num = swapInt32(num);

			header_1.push_back(num);
		}
		else
		{
			in.read((char*)&num, 4);
			header_1.push_back(num);
		}
	}

	return header_1;
}

vector<double> parse_header_2(QFile& in)
{
	double num;
	vector<double> header_2;
	for (size_t i = 0; i < 8; i++)
	{
		in.read((char*)&num, 8);
		header_2.push_back(num);
	}

	return header_2;
}

header parse_header(QFile &in)
{
	header header;
	header.header_1 = parse_header_1(in);
	header.header_2 = parse_header_2(in);
	return header;
}

vector<int> parse_record_header(QFile& in)
{
	vector<int> record;
	int rn, cl;
	in.read((char*)&rn, 4);
	rn = swapInt32(rn);
	in.read((char*)&cl, 4);
	cl = swapInt32(cl);
	record.push_back(rn);
	record.push_back(cl);
	record[1] = (record[1] + 4) * 2;

	return record;
}

vector<shpItem*> parseSHP(QFile& in)
{
	vector<shpItem*> shp;
	header_1 = parse_header(in);
	for (int filele = 0; filele < header_1.header_1[6]*2 - 100;) {
		auto record = parse_record_header(in);
		filele += record[1];
		shpItem* shptemp = new shpItem(parse_shape(in),0,0);
		shp.push_back(shptemp);
	}

	return shp;
}

qTree* parse_shape(QFile& in)
{
	int type;
	in.read((char*)&type, 4);
	qTree* ret;
	switch (type)
	{
	case 1:
		ret = parse_point(in);
		break;
	case 3:
		ret = parse_polyline(in);
		break;
	case 5:
		ret = parse_polygon(in);
		break;
	default:
		ret = parse_point(in);
		break;
	}

	return ret;
}

qTree* parse_point(QFile& in)
{
	int NumParts;
	int NumPoints;
	Region rootRegion;
	initRegion(&rootRegion, 90, -90, -180, 180);
	qPoint* point = new qPoint;
	point->initNode(point, 1, rootRegion);
	for (size_t i = 0; i < 4; i++)
	{
		double a;
		in.read((char*)&a, 8);
	}
	in.read((char*)&NumParts, 4);
	in.read((char*)&NumPoints, 4);
	for (int i = 0; i < NumParts; i++)
	{
		int a;
		in.read((char*)&a, 4);
	}
	for (int i = 0; i < NumPoints; i++) {
		element ele;
		double x, y;
		in.read((char*)&x, 8);
		in.read((char*)&y, 8);
		ele.ele_point.lat = y;
		ele.ele_point.lng = x;
		point->insertEle(point, ele);
	}

	return point;
}

qTree* parse_polyline(QFile& in)
{
	int NumParts;
	int NumPoints;
	Region rootRegion;
	initRegion(&rootRegion, 90, -90, -180, 180);
	vector<int> pointIndex;
	qLine* line = new qLine;
	line->initNode(line, 1, rootRegion);
	for (size_t i = 0; i < 4; i++)
	{
		double a;
		in.read((char*)&a, 8);
	}
	in.read((char*)&NumParts, 4);
	in.read((char*)&NumPoints, 4);
	for (int i = 0; i < NumParts; i++)
	{
		int a;
		in.read((char*)&a, 4);
		pointIndex.push_back(a);
	}
	double x0, y0;
	for (int j = 0; j < NumParts; j++)
	{
		int index;
		if (j == NumParts - 1)
			index = NumPoints;
		else if (NumParts == 1)
			index = NumPoints;
		else
			index = pointIndex[j + 1];

		int size;
		if (j != NumParts - 1)
			size = pointIndex[j + 1] - pointIndex[j];
		else
			size = NumPoints - pointIndex[j];
		for (int i = 0; i < size / 2; i++)
		{
			element ele;
			double x1, y1, x2, y2;
			in.read((char*)&x1, 8);
			in.read((char*)&y1, 8);
			in.read((char*)&x2, 8);
			in.read((char*)&y2, 8);
			if (i)
			{
				element elel;
				elel.ele_line.x1 = x0;
				elel.ele_line.x2 = x1;
				elel.ele_line.y1 = y0;
				elel.ele_line.y2 = y1;
				line->insertEle(line, elel);
			}
			x0 = x2;
			y0 = y2;
			ele.ele_line.x1 = x1;
			ele.ele_line.x2 = x2;
			ele.ele_line.y1 = y1;
			ele.ele_line.y2 = y2;

			line->insertEle(line, ele);
		}
		if (size % 2)
		{
			element ele;
			double x1, y1;
			in.read((char*)&x1, 8);
			in.read((char*)&y1, 8);

			ele.ele_line.x1 = x0;
			ele.ele_line.x2 = x1;
			ele.ele_line.y1 = y0;
			ele.ele_line.y2 = y1;

			line->insertEle(line, ele);
		}
	}

	return line;
}

qTree* parse_polygon(QFile& in)
{
	int NumParts;
	int NumPoints;
	Region rootRegion;
	initRegion(&rootRegion, 90, -90, -180, 180);
	qLine temp;
	vector<int> pointIndex;
	qPolygon* polygon = new qPolygon;
	polygon->initNode(polygon, 1, rootRegion);
	for (size_t i = 0; i < 4; i++)
	{
		double a;
		in.read((char*)&a, 8);
	}
	in.read((char*)&NumParts, 4);
	in.read((char*)&NumPoints, 4);
	for (int i = 0; i < NumParts; i++)
	{
		int a;
		in.read((char*)&a, 4);
		pointIndex.push_back(a);
	}
	double x0, y0;
	for (int j = 0; j < NumParts; j++)
	{
		int index;
		double xi, yi, xl, yl;
		if (j == NumParts - 1)
			index = NumPoints;
		else if (NumParts == 1)
			index = NumPoints;
		else
			index = pointIndex[j + 1];

		int size;
		if (j != NumParts - 1)
			size = pointIndex[j + 1] - pointIndex[j];
		else
			size = NumPoints - pointIndex[j];
		for (int i = 0; i < size / 2; i++)
		{
			element ele;
			double x1, y1, x2, y2;
			in.read((char*)&x1, 8);
			in.read((char*)&y1, 8);
			in.read((char*)&x2, 8);
			in.read((char*)&y2, 8);
			if (i)
			{
				element elel;
				elel.ele_line.x1 = x0;
				elel.ele_line.x2 = x1;
				elel.ele_line.y1 = y0;
				elel.ele_line.y2 = y1;
				polygon->insertEle(polygon, elel);
			}
			else
				xi = x1, yi = y1;
			x0 = x2;
			y0 = y2;
			ele.ele_line.x1 = x1;
			ele.ele_line.x2 = x2;
			ele.ele_line.y1 = y1;
			ele.ele_line.y2 = y2;

			if (i == size / 2 - 1)
				xl = x2, yl = y2;
			polygon->insertEle(polygon, ele);
		}
		if (size % 2)
		{
			element ele;
			double x1, y1;
			in.read((char*)&x1, 8);
			in.read((char*)&y1, 8);

			ele.ele_line.x1 = x0;
			ele.ele_line.x2 = x1;
			ele.ele_line.y1 = y0;
			ele.ele_line.y2 = y1;
			polygon->insertEle(polygon, ele);

			ele.ele_line.x1 = x1;
			ele.ele_line.x2 = xi;
			ele.ele_line.y1 = y1;
			ele.ele_line.y2 = yi;
			polygon->insertEle(polygon, ele);
		}
		else
		{
			element ele;
			ele.ele_line.x1 = xl;
			ele.ele_line.x2 = xi;
			ele.ele_line.y1 = yl;
			ele.ele_line.y2 = yi;
			polygon->insertEle(polygon, ele);
		}
	}

	return polygon;
}


string parse_string(QFile &input, char length)
{
	string temp;
	char tempchar;
	for (size_t i = 0; i < length; i++)
	{
		input.read((char*)&tempchar, 1);
		temp.push_back(tempchar);
	}
	return temp;
}

logical parse_logical(QFile &input)
{
	char temp;
	logical log;
	input.read(&temp, 1);
	if (temp == 'Y' || temp == 'y' || temp == 'T' || temp == 't')
		log.value = true, log.init = true;
	else if (temp == '?')
		log.init = false;
	else
		log.value = false, log.init = true;
	return log;
}

string parse_logical_str(QFile &input)
{
	char temp;
	string log;
	input.read(&temp, 1);
	if (temp == 'Y' || temp == 'y' || temp == 'T' || temp == 't')
		log.push_back('t');
	else if (temp == '?')
		log.push_back('0');
	else
		log.push_back('f');
	return log;
}

parseDBF parse_dbf(QFile& input)
{
	parseDBF tempdbf;
	Header header;
	char tempchar;
	int tempint;
	short tempshort;
	input.read((char*)&tempchar, 1);
	header.version = tempchar;
	input.read((char*)&tempchar, 1);
	header.year = tempchar + 1900;
	for (int i = 0; i < 2; i++)
	{
		input.read((char*)&tempchar, 1);
		header.date[i] = tempchar;
	}
	input.read((char*)&tempint, 4);
	header.recordsNum = tempint;

	input.read((char*)&tempshort, 2);
	header.bytesInHeader = tempshort;
	input.read((char*)&tempshort, 2);
	header.lenthPerRecord = tempshort;

	for (size_t i = 0; i < 2; i++)
	{
		input.read((char*)&tempchar, 1);
		header.reserved_1[i] = tempchar;
	}
	input.read(&tempchar, 1);
	header.unfinshed = tempchar;
	input.read(&tempchar, 1);
	header.dBASE = tempchar;
	for (size_t i = 0; i < 12; i++)
	{
		input.read(&tempchar, 1);
		header.reserved_2[i] = tempchar;
	}
	input.read(&tempchar, 1);
	header.MDXmark = tempchar;
	input.read(&tempchar, 1);
	header.languageID = tempchar;
	for (size_t i = 0; i < 2; i++)
	{
		input.read((char*)&tempchar, 1);
		header.reserved_3[i] = tempchar;
	}
	tempdbf.dbfheader = header;
	vector<Record> rec;
	for (size_t jj = 0; jj < (header.bytesInHeader - 32 - 1) / 32; jj++)
	{
		Record temp;
		for (size_t i = 0; i < 11; i++)
		{
			input.read(&tempchar, 1);
			//temp.name[i] = tempchar;
			temp.name.append(tempchar);
		}
		input.read((char*)&tempchar, 1);
		temp.type = tempchar;
		input.read((char*)&tempint, 4);
		temp.reserved_1 = tempint;
		input.read((char*)&tempchar, 1);
		temp.length = tempchar;
		input.read((char*)&tempchar, 1);
		temp.precison = tempchar;
		input.read((char*)&tempshort, 2);
		temp.reserved_2 = tempshort;
		input.read(&tempchar, 1);
		temp.workspaceID = tempchar;

		for (size_t i = 0; i < 10; i++)
		{
			input.read(&tempchar, 1);
			temp.reserved_3[i] = tempchar;
		}

		input.read(&tempchar, 1);
		temp.MDXmark = tempchar;
		rec.push_back(temp);
	}

	input.read((char*)&tempchar, 1);
	input.read((char*)&tempchar, 1);

	tempdbf.dbfRecord = rec;
	vector<logical> parlog;
	vector<QString> parstr;
	vector< vector<string> > dbfdata;
	for (size_t i = 0; i < header.recordsNum; i++)
	{
		vector<string> tttt;
		for (size_t j = 0; j < rec.size(); j++)
		{
			string tempqst;
			
			if (rec[j].type == 'L')
				tempqst = parse_logical_str(input);
			else
				tempqst = parse_string(input,rec[j].length);
			tttt.push_back(tempqst);
		}
		dbfdata.push_back(tttt);
		input.read(&tempchar, 1);
	}

	input.read((char*)&tempchar, 1);
	cout << (int)tempchar;

	//tempdbf.logic = parlog;
	//tempdbf.str = parstr;
	tempdbf.dbfdata = dbfdata;
	return tempdbf;
}