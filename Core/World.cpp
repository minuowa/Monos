#include "stdafx.h"
#include "World.h"


World::World()
{
}


World::~World()
{
	dSafeDeleteMap2(mAccounts);
}

bool World::initialize()
{
	return true;
}

void World::reclaimAccount(Account* account)
{
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
}

void World::sync(string account_guid, string cmd)
{
	auto acc = mAccounts.find(account_guid);
	assert(acc != mAccounts.end());
	Account* account = acc->second;
	account->sendDB(cmd);
}
