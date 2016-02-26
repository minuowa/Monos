#include "stdafx.h"
#include "SceneApp.h"
#include "NetAgent.h"

SceneApp::SceneApp(int narg, const char** args)
	:App(narg, args)
	,mNetAgent(nullptr)
{

}

SceneApp::~SceneApp()
{
	dSafeDelete(mNetAgent);
}

bool SceneApp::initialize()
{
	if (!__super::initialize())
		return false;
	mNetAgent = new NetAgent();
	Net.onMessage.push_back(mNetAgent);
	Net.onDisconnect.push_back(mNetAgent);
	return true;
}
