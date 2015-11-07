#pragma once
#include "SQLExecuter.h"
class SQLTable
{
public:
	string dbName;
	string name;
private:
	int recordCount;
	SQLExecuter* mExecuter;
public:
	SQLTable();
	~SQLTable();
	
	bool insert(const char* cmd);
	
	bool exist(const char* col);
	
	bool insertCol(const char* col);

	int refreshRecordCount();

private:
	
	std::vector<string> mcolumns;
};

