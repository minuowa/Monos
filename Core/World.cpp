#include "stdafx.h"
#include "World.h"
#include "App.h"


World::World()
{
}


World::~World()
{
	App::Net.onDisconnect -= this;
	dSafeDeleteMap2(mAccounts);
}

bool World::initialize()
{
	App::Net.onDisconnect += this;
	return true;
}

void World::reclaimAccount(Account* account)
{
	onAccountLeaveWorld.trigger();

	assert(account);
	assert(mAccounts.find(account->guid()) != mAccounts.end());
	mAccounts.erase(account->guid());
	dSafeDelete(account);
}

void World::onEnterWorld(Account* account)
{
	assert(mAccounts.find(account->guid()) == mAccounts.end());
	mAccounts.insert(make_pair(account->guid(), account));
	account->enterGate();
	onAccountEnterWorld.trigger();
}

void World::sync(string account_guid, string cmd)
{
	auto acc = mAccounts.find(account_guid);
	assert(acc != mAccounts.end());
	Account* account = acc->second;
	account->sendDBToClient(cmd);
}

void World::onCallBack(const uDelegate& d, uEventArgs* e)
{
	if (d == App::Net.onDisconnect)
	{
		NetWork::ConnectArg* arg = (NetWork::ConnectArg*)e;

		uMap<string, Account*>::iterator ibegin = mAccounts.begin();
		uMap<string, Account*>::iterator iend = mAccounts.end();
		for (; ibegin != iend;++ibegin)
		{
			if (ibegin->second->getNetInterface() == arg->connect)
			{
				reclaimAccount(ibegin->second);
				return;
			}
		}
	}
}
