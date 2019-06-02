#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <qfile.h>
#include "qTree.h"
#include <qdebug.h>
#include "shpshape.h"

using namespace std;

class shpHeader
{
public:
	vector<int> header_1;
	vector<double> header_2;
};


struct dbfHeader
{
	char version;
	int year;
	char date[2];
	int recordsNum;
	short bytesInHeader;
	short lenthPerRecord;
	char reserved_1[2];
	char unfinshed;
	char dBASE;
	char reserved_2[12];
	bool MDXmark;
	char languageID;
	char reserved_3[2];
};

struct Record
{
	QString name;
	char type;
	int reserved_1;
	int length;
	int precison;
	short reserved_2;
	char workspaceID;
	char reserved_3[10];
	bool MDXmark;
};

struct logical
{
	bool value;
	bool init;
};


int32_t swapInt32(int32_t value);
vector<int> parse_header_1(QFile& in);
vector<double> parse_header_2(QFile& in);
shpHeader parse_header(QFile& in);
vector<int> parse_record_header(QFile& in);

vector<abstractObject*> parseShp(QFile& in, Region* reg, size_t fileIndex);
abstractObject* parse_shape(QFile& in, size_t fileIndex);
abstractObject* parse_point(QFile& in, size_t fileIndex);
abstractObject* parse_polyline(QFile& in, size_t fileIndex);
abstractObject* parse_polygon(QFile& in, size_t fileIndex);

struct parseDBF
{
	vector< vector<string> > dbfdata;
	dbfHeader dbfheader;
	vector<Record> dbfRecord;
};

parseDBF parse_dbf(QFile& input);

void initRegion(Region *region, double up, double bottom, double left, double right);
