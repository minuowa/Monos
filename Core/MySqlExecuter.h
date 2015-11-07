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

	bool queryEnd(stringVector& result);

	bool queryEnd(std::vector<shared_ptr<stringVector>>& result);

	bool queryEnd();

	virtual unsigned long count() override;

private:

	DBConfig mConfig;

	MYSQL* mConnection;
};




#endif // DBExecuter_h__
