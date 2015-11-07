#include "stdafx.h"
#include "SQLTable.h"
#include "App.h"

SQLTable::SQLTable()
	:recordCount(0)
	, mExecuter(&App::DataBase.executer())
{
}


SQLTable::~SQLTable()
{
}


bool SQLTable::insert(const char* cmd)
{
	int n = recordCount;
	mExecuter->queryBegin(cmd);
	refreshRecordCount();
	return n < recordCount;
}

//void SQLTable::freshColumns()
//{
//	/*stringstream str;
//	str << "SELECT COLUMN_NAME FROM " << dbName << ".COLUMNS WHERE TABLE_NAME = '" << name << "'";
//	try
//	{
//		*_pSession << str.str(), into(mcolumns), now;
//	}
//	catch (ConnectionException& ce)
//	{
//		std::cout << ce.displayText() << std::endl;
//	}
//	catch (StatementException& se)
//	{
//		std::cout << se.displayText() << std::endl;
//	}*/
//}

bool SQLTable::exist(const char* col)
{
	return std::find(mcolumns.begin(), mcolumns.end(), col) != mcolumns.end();
}

bool SQLTable::insertCol(const char* col)
{
	//alter table table1 add transactor varchar(10) not Null;
	/*stringstream str;
	str << "alter table " << name << "add " << col << " transactor varchar(10) not Null;";
	try
	{
		*_pSession << str.str(), now;
	}
	catch (ConnectionException& ce)
	{
		std::cout << ce.displayText() << std::endl;
	}
	catch (StatementException& se)
	{
		std::cout << se.displayText() << std::endl;
	}
	freshColumns();*/
	return exist(col);
}

int SQLTable::refreshRecordCount()
{
	stringstream sm;
	sm << "SELECT COUNT(*) FROM " << name << ";";

	mExecuter->queryBegin(sm.str().c_str());
	recordCount = mExecuter->count();
	return recordCount;
}
