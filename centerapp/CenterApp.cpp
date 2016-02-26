#include "stdafx.h"
#include "CenterApp.h"
#include "NetAgent.h"

CenterApp::CenterApp(int narg, const char** args)
	:App(narg, args)
	,mNetAgent(nullptr)
{

}

CenterApp::~CenterApp()
{
	dSafeDelete(mNetAgent);
}

bool CenterApp::initialize()
{
	if (!__super::initialize())
		return false;
	mNetAgent = new NetAgent();
	Net.onMessage.push_back(mNetAgent);
	Net.onDisconnect.push_back(mNetAgent);
	return true;
}
