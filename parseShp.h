#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>
#include <ctime>
#include "quadTree.h"
#include <qfile.h>
#include "shpItem.h"

using namespace std;

#define MAX_ELE_NUM 100

class header
{
public:
	vector<int> header_1;
	vector<double> header_2;
};


struct Header
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
	//QString name[11];
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
header parse_header(QFile& in);
vector<int> parse_record_header(QFile& in);

struct parseDBF
{
	vector< vector<string> > dbfdata;
	Header dbfheader;
	vector<Record> dbfRecord;
};

parseDBF parse_dbf(QFile& input);
vector<shpItem*> parseSHP(QFile& in);
qTree* parse_shape(QFile& in);
qTree* parse_point(QFile& in);
qTree* parse_polyline(QFile& in);
qTree* parse_polygon(QFile& in);
void initRegion(Region *region, double up, double bottom, double left, double right);
Region regionInclude(qTree* node, Region region);
bool regionContain(Region regionA, Region regionB);
void regionMax(qTree* node, Region region, Region* ret);