#include "stdafx.h"
#include "DataBase.h"
#include "SQLTable.h"
#include "MySqlExecuter.h"
DataBase::DataBase()
{
	mExecuter = new MySqlExecuter();
}


DataBase::~DataBase()
{
	delete mExecuter;
	dSafeDeleteMap2(mTables);
}


bool DataBase::initialize(const DBConfig& config)
{
	if (!mExecuter->initialize(config))
		return false;
	

	//Data::MySQL::Connector::registerConnector();

	//generateConnectString();

	//try
	//{
	//	_pSession = new Session(MySQL::Connector::KEY, _dbConnString);
	//}
	//catch (ConnectionFailedException& ex)
	//{
	//	std::cout << ex.displayText() << std::endl;
	//	std::cout << "Trying to connect without DB and create one ..." << std::endl;
	//	if (!createDB)
	//		return false;
	//}

	reGetTables();

	return true;
}

void DataBase::finish()
{
}

void DataBase::create_table_if_not_exist(const char* name)
{

}

bool DataBase::create_column_if_not_exist(const char* table, const char* key)
{
	auto res=mTables.find(table);
	if (res != mTables.end())
	{
		SQLTable* sqltable = res->second;
		if (!sqltable->exist(key))
		{
			return sqltable->insertCol(key);
		}
	}
	return false;
}


bool DataBase::createTable(const char* name, const char* cmd)
{
	stringstream cmdstring;
	cmdstring << "CREATE TABLE " << name << " " << cmd;
	mExecuter->queryBegin(cmdstring.str().c_str());
	reGetTables();
	return hasTable(name);
}

bool DataBase::hasTable(const char* name)
{
	return mTables.find(name) != mTables.end();
}

bool DataBase::queryColValue(string table, string key, const char* value)
{
	stringstream sm;
	sm << "SELECT COUNT(*) FROM " << table << " WHERE " << key << " = ?";
	mExecuter->queryBegin(sm.str().c_str());
	return mExecuter->queryEnd();
}

bool DataBase::queryWithOneRecord(const char* cmd, std::vector<string>& result)
{
	mExecuter->queryBegin(cmd);
	return mExecuter->queryEnd(result);
}

bool DataBase::insertRecordByGUID(const char* table, const char* guid)
{
	assert(table);
	assert(guid);
	assert(strlen(table));
	assert(strlen(guid));

	stringstream sm;
	sm << "INSERT INTO " << table << "(guid) VALUES (" << guid << ");";
	mExecuter->queryBegin(sm.str().c_str());
	return mExecuter->queryEnd();
}



SQLTable* DataBase::getTable(const char* name)
{
	auto p = mTables.find(name);
	if(p==mTables.end())
		return nullptr;
	return p->second;
}

bool DataBase::createDB()
{
	/*if (!connectNoDB)
		return false;

	try
	{
		_pSession = new Session(MySQL::Connector::KEY, _dbConnString);
	}
	catch (ConnectionFailedException& ex)
	{
		std::cout << ex.displayText() << std::endl;
		return false;
	}*/
	return true;
}

void DataBase::generateConnectString()
{
	/*std::stringstream ss;
	ss << "host=" << mConfig.host
		<< ";port=" << mConfig.port
		<< ";user=" << mConfig.user
		<< ";password=" << mConfig.password
		<< ";db=" << mConfig.dbName
		<< ";compress=true"
		<< ";auto-reconnect=true"
		<< ";secure-auth=true";*/
	//_dbConnString = ss.str();
}

void DataBase::reGetTables()
{
	dSafeDeleteMap2(mTables);

	stringstream sm;
	sm << "show tables;";
	mExecuter->queryBegin(sm.str().c_str());
		
	std::vector<shared_ptr<stringVector>> tableNames;

	if (!mExecuter->queryEnd(tableNames))
	{
		//assert(0);
	}
	for (auto record : tableNames)
	{
		SQLTable* table = new SQLTable();
		table->name = record->at(0);
		table->refreshRecordCount();
		mTables.insert(make_pair(table->name, table));
	}
}

bool DataBase::connectNoDB()
{
	/*try
	{
		Session session(MySQL::Connector::KEY, _dbConnString);
		std::cout << "Connected to [" << "MySQL" << "] without database." << std::endl;
		dbInfo(session);
		session << "CREATE DATABASE IF NOT EXISTS "<< mConfig.dbName<< "; now";
		std::cout << "Disconnecting ..." << std::endl;
		session.close();
		std::cout << "Disconnected." << std::endl;
		return true;
	}
	catch (ConnectionFailedException& ex)
	{
		std::cout << ex.displayText() << std::endl;
		return false;
	}*/
	return true;
}

//void DataBase::dbInfo(Poco::Data::Session& session)
//{
//	std::cout << "Server Info: " << MySQL::Utility::serverInfo(session) << std::endl;
//	std::cout << "Server Version: " << MySQL::Utility::serverVersion(session) << std::endl;
//	std::cout << "Host Info: " << MySQL::Utility::hostInfo(session) << std::endl;
//}

void DataBase::bareboneMySQLTest(const char* host, const char* user, const char* pwd, const char* db, int port, const char* tableCreateString)
{
	//bareboneMySQLTest(mConfig.host.c_str(), mConfig.user.c_str(), mConfig.password.c_str(), mConfig.dbName.c_str()
	//	, mConfig.port, "create table account;");

	/*int rc;
	MYSQL* hsession = mysql_init(0);
	assert(hsession != 0);

	MYSQL* tmp = mysql_real_connect(hsession, host, user, pwd, db, port, 0, 0);
	assert(tmp == hsession);

	MYSQL_STMT* hstmt = mysql_stmt_init(hsession);
	assert(hstmt != 0);

	std::string sql = "DROP TABLE Test";
	mysql_real_query(hsession, sql.c_str(), static_cast<unsigned long>(sql.length()));

	sql = tableCreateString;
	rc = mysql_stmt_prepare(hstmt, sql.c_str(), static_cast<unsigned long>(sql.length()));
	assert(rc == 0);

	rc = mysql_stmt_execute(hstmt);
	assert(rc == 0);

	sql = "INSERT INTO Test VALUES (?,?,?,?,?)";
	rc = mysql_stmt_prepare(hstmt, sql.c_str(), static_cast<unsigned long>(sql.length()));
	assert(rc == 0);

	std::string str[3] = { "111", "222", "333" };
	int fourth = 4;
	float fifth = 1.5;

	MYSQL_BIND bind_param[5] = { { 0 } };

	bind_param[0].buffer = const_cast<char*>(str[0].c_str());
	bind_param[0].buffer_length = static_cast<unsigned long>(str[0].length());
	bind_param[0].buffer_type = MYSQL_TYPE_STRING;

	bind_param[1].buffer = const_cast<char*>(str[1].c_str());
	bind_param[1].buffer_length = static_cast<unsigned long>(str[1].length());
	bind_param[1].buffer_type = MYSQL_TYPE_STRING;

	bind_param[2].buffer = const_cast<char*>(str[2].c_str());
	bind_param[2].buffer_length = static_cast<unsigned long>(str[2].length());
	bind_param[2].buffer_type = MYSQL_TYPE_STRING;

	bind_param[3].buffer = &fourth;
	bind_param[3].buffer_type = MYSQL_TYPE_LONG;

	bind_param[4].buffer = &fifth;
	bind_param[4].buffer_type = MYSQL_TYPE_FLOAT;

	rc = mysql_stmt_bind_param(hstmt, bind_param);
	assert(rc == 0);

	rc = mysql_stmt_execute(hstmt);
	assert(rc == 0);

	sql = "SELECT * FROM Test";
	rc = mysql_stmt_prepare(hstmt, sql.c_str(), static_cast<unsigned long>(sql.length()));
	assert(rc == 0);

	char chr[3][5] = { { 0 } };
	unsigned long lengths[5] = { 0 };
	fourth = 0;
	fifth = 0.0f;

	MYSQL_BIND bind_result[5] = { { 0 } };

	bind_result[0].buffer = chr[0];
	bind_result[0].buffer_length = sizeof(chr[0]);
	bind_result[0].buffer_type = MYSQL_TYPE_STRING;
	bind_result[0].length = &lengths[0];

	bind_result[1].buffer = chr[1];
	bind_result[1].buffer_length = sizeof(chr[1]);
	bind_result[1].buffer_type = MYSQL_TYPE_STRING;
	bind_result[1].length = &lengths[1];

	bind_result[2].buffer = chr[2];
	bind_result[2].buffer_length = sizeof(chr[2]);
	bind_result[2].buffer_type = MYSQL_TYPE_STRING;
	bind_result[2].length = &lengths[2];

	bind_result[3].buffer = &fourth;
	bind_result[3].buffer_type = MYSQL_TYPE_LONG;
	bind_result[3].length = &lengths[3];

	bind_result[4].buffer = &fifth;
	bind_result[4].buffer_type = MYSQL_TYPE_FLOAT;
	bind_result[4].length = &lengths[4];

	rc = mysql_stmt_bind_result(hstmt, bind_result);
	assert(rc == 0);

	rc = mysql_stmt_execute(hstmt);
	assert(rc == 0);
	rc = mysql_stmt_fetch(hstmt);
	assert(rc == 0);

	assert(0 == std::strncmp("111", chr[0], 3));
	assert(0 == std::strncmp("222", chr[1], 3));
	assert(0 == std::strncmp("333", chr[2], 3));
	assert(4 == fourth);
	assert(1.5 == fifth);

	rc = mysql_stmt_close(hstmt);
	assert(rc == 0);

	sql = "DROP TABLE Test";
	rc = mysql_real_query(hsession, sql.c_str(), static_cast<unsigned long>(sql.length()));
	assert(rc == 0);

	mysql_close(hsession);*/
}

