#include "stdafx.h"
#include "Account.h"
#include "App.h"

const char* Account::className()
{
	return "S_Account";
}

const char* Account::dbClassName()
{
	return "Account";
}

Account::Account()
{
}


Account::~Account()
{
}

bool Account::fetch()
{
	return this->fetchByDBField("guid");
}

void Account::enterScene(int sceneid)
{
	ScriptArgs* args=new ScriptArgs();
	*args << sceneid;
	this->call("enterScene", args);
}

void Account::enterWorld()
{

}
