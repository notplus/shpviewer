#include "parseShp.h"

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

shpHeader parse_header(QFile &in)
{
	shpHeader header;
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

vector<abstractObject*> parseShp(QFile& in, Region* reg, size_t fileIndex)
{
	vector<abstractObject*> shp;
	shpHeader header = parse_header(in);
	initRegion(reg, header.header_2[3], header.header_2[1],
		header.header_2[0], header.header_2[2]);
	for (int filele = 0; filele < header.header_1[6] * 2 - 100;)
	{
		auto record = parse_record_header(in);
		filele += record[1];
		abstractObject* temp = parse_shape(in, fileIndex);
		Region minRegion;
		initRegion(&minRegion, 0, 0, 0, 0);
		includeMin(&changeManager::Instance()->TREE[fileIndex], &temp->region, &minRegion);
		qTree* view_tree;
		returnTree(&changeManager::Instance()->TREE[fileIndex], &minRegion, view_tree);
		if (view_tree->objects==NULL)
			view_tree->objects = new std::vector<abstractObject*>;
		view_tree->objects->push_back(temp);
		shp.push_back(temp);
	}

	return shp;
}

abstractObject* parse_shape(QFile& in, size_t fileIndex)
{
	int type;
	in.read((char*)&type, 4);
	abstractObject* ret;
	switch (type)
	{
	case 1:
		ret = parse_point(in,fileIndex);
		break;
	case 3:
		ret = parse_polyline(in, fileIndex);
		break;
	case 5:
		ret = parse_polygon(in, fileIndex);
		break;
	default:
		ret = parse_point(in, fileIndex);
		break;
	}

	return ret;
}

abstractObject* parse_point(QFile& in, size_t fileIndex)
{
	int NumParts;
	int NumPoints;
	Region region;
	shppoint* point = shapeFactory::Instance()->createPoint();
	double scope[4];
	for (size_t i = 0; i < 4; i++)
	{
		double a;
		in.read((char*)&a, 8);
		scope[i] = a;
	}
	initRegion(&region, scope[3], scope[1], scope[0], scope[2]);
	point->region = region;
	in.read((char*)&NumParts, 4);
	in.read((char*)&NumPoints, 4);
	point->ele_list.resize(NumPoints);
	point->ele_num = NumPoints;
	for (int i = 0; i < NumParts; i++)
	{
		int a;
		in.read((char*)&a, 4);
	}
	for (int i = 0; i < NumPoints; i++) {
		elePoint* ele = (elePoint*)malloc(sizeof(elePoint));
		double x, y;
		in.read((char*)& x, 8);
		in.read((char*)& y, 8);
		ele->y = y;
		ele->x = x;
		changeManager::Instance()->TREE[fileIndex].insertEle(&changeManager::Instance()->TREE[fileIndex], ele);
		point->ele_list[i] = ele;
	}
	
	return point;
}

abstractObject* parse_polyline(QFile& in, size_t fileIndex)
{
	int NumParts;
	int NumPoints;
	Region region;
	shpline* line = shapeFactory::Instance()->createLine();
	double scope[4];
	for (size_t i = 0; i < 4; i++)
	{
		double a;
		in.read((char*)&a, 8);
		scope[i] = a;
	}
	initRegion(&region, scope[3], scope[1], scope[0], scope[2]);
	line->region = region;
	in.read((char*)&NumParts, 4);
	in.read((char*)&NumPoints, 4);
	line->partsIndex = (int*)malloc(sizeof(int) * NumParts);
	line->ele_list.resize(NumPoints);
	line->ele_num = NumPoints;
	line->numParts = NumParts;
	for (int i = 0; i < NumParts; i++)
	{
		int a;
		in.read((char*)&a, 4);
		line->partsIndex[i]=a;
	}
	for (int i = 0; i < NumPoints; i++)
	{
		elePoint* ele = (elePoint*)malloc(sizeof(elePoint));
		double x, y;
		in.read((char*)& x, 8);
		in.read((char*)& y, 8);
		ele->y = y;
		ele->x = x;
		changeManager::Instance()->TREE[fileIndex].insertEle(&changeManager::Instance()->TREE[fileIndex], ele);
		line->ele_list[i] = ele;
	}

	return line;
}

abstractObject* parse_polygon(QFile& in, size_t fileIndex)
{
	int NumParts;
	int NumPoints;
	Region region;
	shppolygon* polygon = shapeFactory::Instance()->createPolygon();
	double scope[4];
	for (size_t i = 0; i < 4; i++)
	{
		double a;
		in.read((char*)&a, 8);
		scope[i] = a;
	}
	initRegion(&region, scope[3], scope[1], scope[0], scope[2]);
	polygon->region = region;
	in.read((char*)&NumParts, 4);
	in.read((char*)&NumPoints, 4);
	polygon->partsIndex = (int*)malloc(sizeof(int) * NumParts);
	polygon->ele_list.resize(NumPoints);
	polygon->numParts = NumParts;
	polygon->ele_num = NumPoints;
	for (int i = 0; i < NumParts; i++)
	{
		int a;
		in.read((char*)&a, 4);
		polygon->partsIndex[i]=a;
	}
	for (int i = 0; i < NumPoints; i++)
	{
		elePoint* ele = (elePoint*)malloc(sizeof(elePoint));
		double x, y;
		in.read((char*)& x, 8);
		in.read((char*)& y, 8);
		ele->y = y;
		ele->x = x;
		changeManager::Instance()->TREE[fileIndex].insertEle(&changeManager::Instance()->TREE[fileIndex], ele);
		polygon->ele_list[i] = ele;
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
	dbfHeader header;
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
	tempdbf.dbfdata = dbfdata;

	return tempdbf;
}