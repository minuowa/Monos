#include "stdafx.h"
#include "NetAgent.h"
#include "Account.h"



NetAgent::NetAgent()
{
}


NetAgent::~NetAgent()
{
}

void NetAgent::onCallBack(const uDelegate& d, uEventArgs* e)
{
	if (d == App::Net.onMessage)
	{
		NetWork::MsgArgs* arg = (NetWork::MsgArgs*)e;
		PKG* pkg = arg->pkg;
		Connection* connect = arg->connect;

		switch (pkg->code)
		{
		case PCODE::ACK:
		{
			assert(mClients.findkey(connect));
			PKGACK* ack = (PKGACK*)pkg;
			int value = mClients[connect];
			assert(value + 1 == ack->key);
		}
		break;
		case PCODE::SYN:
		{
			PKGSYN* syn = (PKGSYN*)pkg;
			assert(!mClients.findkey(connect));
			mClients.Insert(connect, syn->key);
			mClients[connect] = syn->key;

			PKGACK ack;
			ack.key = syn->key + 1;
			SendPKG(connect->getSocket(),ack);

			PKGSYN mysyn;
			SendPKG(connect->getSocket(), mysyn);
		}
		break;
		case PCODE::Create_Account:
		{
			rqCreateAccount* rq = (rqCreateAccount*)pkg;
			if (!on_rqCreateAccount(rq->user, rq->psd))
			{
				rtCreateAccountFailed_Exist rt;
				SendPKG(connect->getSocket(), rt);
			}
			else
			{

			}
		}
		break;
		case PCODE::Login:
		{
			if(!mClients.findkey(connect))
				mClients.Insert(connect, 1);

			rqLogin* rq = (rqLogin*)pkg;
			on_rqLogin(rq->user, rq->psd, connect);
		}
		break;
		}
	}
	else if (d == App::Net.onDisconnect)
	{
		NetWork::ConnectArg* arg = (NetWork::ConnectArg*)e;
		mClients.erase(arg->connect);
	}
}

bool NetAgent::on_rqCreateAccount(string user, string psd)
{
	if (App::DataBase.queryColValue(YW_TABLE_ACCOUNT, YW_TABLE_ACCOUNT_USER, user.c_str()))
	{
		return false;
	}
	else
	{
		//if (App::DataBase.insertRecordByGUID(YW_TABLE_ACCOUNT, user, psd))
		{
			return true;
		}
	}
	return false;
}


bool NetAgent::on_rqLogin(string user, string psw, Connection* con)
{
	//the account enter the net gate
	Account* gateAccount = new Account();
	gateAccount->set_connect(con);
	if (!gateAccount->init())
	{
		delete gateAccount;
		return nullptr;
	}

	bool res = false;

	gateAccount->getDBObject()->setField(YW_TABLE_ACCOUNT_USER, user);

	if(!gateAccount->getDBObject()->fetchByField(YW_TABLE_ACCOUNT_USER))
	{
		if (App::Config.loginApp.db.autoCreateAccount)
		{
			gateAccount->getDBObject()->setField(YW_TABLE_ACCOUNT_PSD, psw);
			gateAccount->getDBObject()->setField(YW_TABLE_ACCOUNT_ID, uPlatform::generateGUIDSimpleString());

			//does not exist,create successfully!
			if (gateAccount->getDBObject()->createAndInsertToDB())
			{
				res = true;
			}
		}
	}
	else 
	{
		MonoString* pwd;
		gateAccount->getDBObject()->getField(YW_TABLE_ACCOUNT_PSD, pwd);
		if (pwd)
		{
			string strpwd = mono_string_to_utf8(pwd);
			if (strpwd == psw)
			{
				//exist,load successfully!
				res = true;
			}
		}
	}

	if (!res)
	{
		rtLogin rt;
		rt.res = SC_Failed_PSD_Error;
		SendPKG(con->getSocket(), rt);

		delete gateAccount;
	}
	else
	{
		rtLogin rt;
		rt.res = LoginResult::Successed;
		SendPKG(con->getSocket(), rt);

		App::World.onEnterWorld(gateAccount);
	}

	return res;
}

const char* NetAgent::YW_DB = "yourworld";

const char* NetAgent::YW_TABLE_ACCOUNT = "yw_account";

const char* NetAgent::YW_TABLE_ACCOUNT_USER = "user";

const char* NetAgent::YW_TABLE_ACCOUNT_PSD = "pwd";

const char* NetAgent::YW_TABLE_ACCOUNT_ID = "guid";

uMap<Connection*, int> NetAgent::mClients;

