#include "stdafx.h"
#include "Test.h"
#include "MonoEngine.h"
#include "App.h"
Test gTest;

Test::Test()
{
}


Test::~Test()
{
}

void Test::DoWork()
{
	MonoEngine& gMono = App::Script;
	{
		MonoObject* client = gMono.newObject("Scripts","Test");
		ScriptArgs arg;

		gMono.invoke("Scripts", "Test", "testVoid", client, nullptr);

		arg << "stringparam"
			<< 15;
		gMono.invoke("Scripts", "Test", "testParam", client, arg.pointer());
	}

	{
		Account* account = new Account();
		account->init();
		account->setDBField("user", "liming");

		string suser;
		account->getDBField("user", suser);
		assert(suser == "liming");

		int initn = 0;
		account->getDBField("test", initn);
		assert(initn == 5);
		account->setDBField("test", 8);
		account->getDBField("test", initn);
		assert(initn == 8);
	}
	//MonoObject* client = gMono.newObject("Scripts","Client");
	//gMono.invoke("Scripts", "Client", "OutMe", client, nullptr);

	//client = gMono.newObject("Scripts", "S_Account");
	//gMono.invoke("Scripts", "S_Account", "enterScene", client, nullptr);

	//client = gMono.newObject("Scripts", "S_Account");
	//gMono.invoke("Scripts", "S_Account", "enterScene", client, nullptr);
}
