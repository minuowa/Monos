#include "stdafx.h"
#include "Account.h"
#include "App.h"
#include "Role.h"
#include "Bundle.h"
Account::Account()
	:mConnect(nullptr)
{
	mClassName = "S_Account";
}


Account::~Account()
{
}



void Account::enterGate()
{
	this->getDBObject()->fetch();
	this->call("enterWorld");
}

void Account::onRoleRqEnterWorld(string guid)
{
	mActiveRole = new Role();
	if (!mActiveRole->init())
	{
		assert(0);
		return;
	}
	mActiveRole->setGuid(guid.c_str());
	mActiveRole->enterScene();
}

void Account::sendDB(string data)
{
	assert(mConnect);
	rtDBData rt;
	assert(data.length() < Default::DataSize);
	dMemoryCopy(rt.data, (void*)data.c_str(), data.length());
	SendPKG(mConnect->getSocket(), rt);
}

void Account::set_connect(Connection* connect)
{
	mConnect = connect;
}

DBAccount* Account::getDBObject()
{
	return mDBObject;
}

bool Account::init()
{
	if (!__super::init())
		return false;
	mDBObject = new DBAccount();
	if (!mDBObject->init())
		return false;

	setField("mDBInterface", mDBObject->get());

	call("onInitEnd");

	return true;
}
