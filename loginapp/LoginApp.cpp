#include "stdafx.h"
#include "LoginApp.h"
#include "NetAgent.h"

LoginApp::LoginApp(int narg, const char** args)
	:App(narg, args)
	,mNetAgent(nullptr)
{

}

LoginApp::~LoginApp()
{
	dSafeDelete(mNetAgent);
}

bool LoginApp::initialize()
{
	if (!__super::initialize())
		return false;
	mNetAgent = new NetAgent();
	Net.onMessage.push_back(mNetAgent);
	Net.onDisconnect.push_back(mNetAgent);
	return true;
}
