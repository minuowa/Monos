#ifndef ConfigInfo_h__
#define ConfigInfo_h__
struct NetConfig
{
	string ip;
	string name;
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
struct ScriptConfig 
{
	bool open;
};
struct LoginConfig 
{
	NetConfig net;
	DBConfig db;
	ScriptConfig script;
};
struct CenterConfig 
{
	DBConfig db;
	ScriptConfig script;
	uMap<int, NetConfig*> centers;
	~CenterConfig() {
		centers.destroySecond();
	}
};

#endif // ConfigInfo_h__