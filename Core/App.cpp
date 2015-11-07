#include "stdafx.h"
#include "App.h"

App::App(int narg, const char** args)
	:mType(APP_TYPE_LOGIN)
	, mCommandLine(narg, args)
{

}

App::~App()
{
}

bool App::initialize()
{
	if (mCommandLine.arg_count())
	{
		mCommandLine.get("type", (int&)mType);
	}

	if (!Config.initialize("..\\config.xml"))
		return false;

	if (!Net.initialize(Config.loginApp.net))
		return false;

	if (!DataBase.initialize(Config.loginApp.db))
		return false;

	if (!Script.initialize())
		return false;

	if (!World.initialize())
		return false;
	return true;
}

void App::run()
{
	while (true)
	{
		Net.prosess();
		uPlatform::sleep(5);
	}
}

void App::finish()
{
	Script.finish();
}

Config App::Config;

DataBase App::DataBase;

NetWork App::Net;

World App::World;

MonoEngine App::Script;
