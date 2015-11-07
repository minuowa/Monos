#pragma once
class NetAgent;
class LoginApp :
	public App
{
public:
	LoginApp(int narg, const char** args);
	~LoginApp();
	virtual bool initialize();

private:
	NetAgent* mNetAgent;
};

