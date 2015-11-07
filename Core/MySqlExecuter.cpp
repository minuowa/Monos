#include "stdafx.h"
#include "MySqlExecuter.h"



MySqlExecuter::MySqlExecuter()
{
}


MySqlExecuter::~MySqlExecuter()
{
}

bool MySqlExecuter::initialize(const DBConfig& config)
{
	mConfig = config;

	mConnection = mysql_init(0);
	assert(mConnection != 0);

	MYSQL* tmp = mysql_real_connect(mConnection, mConfig.host.c_str(), mConfig.user.c_str(), mConfig.password.c_str(),
		mConfig.dbName.c_str(), mConfig.port, 0, 0);
	assert(tmp == mConnection);

	MYSQL_STMT* hstmt = mysql_stmt_init(mConnection);
	assert(hstmt != 0);
	
	return true;
}


void MySqlExecuter::queryBegin(const char* cmd)
{
	assert(cmd);
	int res = mysql_real_query(mConnection, cmd, static_cast<unsigned long>(strlen(cmd)));
	printf("QUERY=>%s\n", cmd);
}



bool MySqlExecuter::queryEnd()
{
	MYSQL_RES* ress = mysql_store_result(mConnection);

	if (ress)
	{
		MYSQL_ROW row = mysql_fetch_row(ress);

		if (row)
			return true;
	}

	return false;
}


bool MySqlExecuter::queryEnd(stringVector& result)
{
	MYSQL_RES* ress = mysql_store_result(mConnection);

	if (ress)
	{
		MYSQL_ROW row = mysql_fetch_row(ress);

		int n = 0;
		if(ress->lengths)
			n = *ress->lengths;

		if (n>0)
		{
			for (unsigned long i = 0; i < n; ++i)
				result.push_back(row[i]);
		}
		else
		{
			while (row)
			{
				result.push_back(row[0]);
				row = mysql_fetch_row(ress);
			}
		}

	}
	else
	{
		return false;
	}

	return result.size() > 0;
}


bool MySqlExecuter::queryEnd(std::vector<shared_ptr<stringVector>>& result)
{
	MYSQL_RES* ress = mysql_store_result(mConnection);

	if (ress)
	{
		MYSQL_ROW row = mysql_fetch_row(ress);
		int n = 0;
		while (row)
		{
			assert(n < result.size());
			shared_ptr<stringVector> vec(new stringVector());
			for (unsigned long i = 0; i < *ress->lengths; ++i)
				vec->push_back(row[i]);
			++n;
			row = mysql_fetch_row(ress);
		}
	}

	return result.size() > 0;
}

unsigned long MySqlExecuter::count()
{
	MYSQL_RES* ress = mysql_store_result(mConnection);
	MYSQL_ROW row = mysql_fetch_row(ress);
	if (ress)
		return ::atoi(row[0]);
	return 0;
}

