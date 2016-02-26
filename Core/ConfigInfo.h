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
struct LoginConfig 
{
	NetConfig net;
	DBConfig db;
};
struct CenterConfig 
{
	uMap<int, NetConfig*> centers;
	DBConfig db;
	~CenterConfig(){
		centers.destroySecond();
	}
};
struct GameConfig
{


	GameConfig();
};

#endif // ConfigInfo_h__