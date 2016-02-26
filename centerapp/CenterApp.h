#pragma once
class NetAgent;
class CenterApp :
	public App
{
public:
	CenterApp(int narg, const char** args);
	~CenterApp();
	virtual bool initialize();

private:
	NetAgent* mNetAgent;
};

