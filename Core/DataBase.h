#pragma once
#include "ConfigInfo.h"
#include "SQLExecuter.h"
#include "SQLTable.h"

class DBRecord;

class CX_LIB DataBase
{
public:
	DataBase();
	~DataBase();

	bool initialize(const DBConfig& config);
	
	void finish();
	
	void create_table_if_not_exist(const char* name);
	
	bool create_column_if_not_exist(const char* table,const char* key);

	bool createTable(const char* name, const char* cmd);

	bool hasTable(const char* name);

	bool queryColValue(string table, string key, const char* value);

	bool queryWithOneRecord(const char* cmd,std::vector<string>& result);

	bool insertRecordByGUID(const char* table, const char* guid);

	SQLExecuter& executer();

	SQLTable* getTable(const char* name);

public:

	bool createDB();

private:

	void generateConnectString();
	void reGetTables();
	bool connectNoDB();
	//static void dbInfo(Poco::Data::Session& session);
	void bareboneMySQLTest(const char* host, const char* user, const char* pwd, const char* db, int port, const char* tableCreateString);

private:

	SQLExecuter* mExecuter;

	//Poco::SharedPtr<Poco::Data::Session> _pSession;

	map<string,SQLTable*> mTables;
};



template<typename T>
bool DataBase::insertKeyValue(const char* table, T value, T value1)
{
	stringstream str;
	str << "INSERT INTO " << table << " VALUES (?,?) ";
	try
	{ 
		*_pSession << str.str(), use(value), use(value1), now;
		return true;
	}
	catch (ConnectionException& ce) 
	{
		std::cout << ce.displayText() << std::endl; 
	}
	catch (StatementException& se)
	{ 
		std::cout << se.displayText() << std::endl;
	}
	return false;
}
inline SQLExecuter& DataBase::executer()
{
	return *mExecuter;
}