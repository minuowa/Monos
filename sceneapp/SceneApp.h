#pragma once
class NetAgent;
class SceneApp :
	public App
{
public:
	SceneApp(int narg, const char** args);
	~SceneApp();
	virtual bool initialize();

private:
	NetAgent* mNetAgent;
};

