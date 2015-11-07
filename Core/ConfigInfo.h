#ifndef ConfigInfo_h__
#define ConfigInfo_h__
struct NetConfig
{
	string ip;
	u32 port;
	u32 threadCount;
	NetConfig();
};
struct DBConfig
{
	string dbName;
	string user;
	string password;
	string host;
	int port;
	bool autoCreateAccount;
	DBConfig()
		:host("localhost")
		, port(3306)
		, autoCreateAccount(true)
	{

	}
};

struct CommonConfig
{
	NetConfig net;
	DBConfig db;
	CommonConfig();
};

#endif // ConfigInfo_h__