#ifndef DBExecuter_h__
#define DBExecuter_h__

#include "SQLExecuter.h"
#include "mysql.h"

class MySqlExecuter:public SQLExecuter
{
public:

	MySqlExecuter();

	~MySqlExecuter();

	bool initialize(const DBConfig& config);

	void queryBegin(const char* cmd);

	/*
		Only return first record or nothing.
	*/
	bool queryEnd(stringVector& result);

	/*
		return all eligible records.
	*/
	bool queryEnd(std::vector<shared_ptr<stringVector>>& result);

	bool queryEnd();

	virtual unsigned long count() override;

private:

	DBConfig mConfig;

	MYSQL* mConnection;
};




#endif // DBExecuter_h__
