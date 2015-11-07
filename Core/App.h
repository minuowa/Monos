#pragma once
#include "Config.h"
#include "MonoEngine.h"
#include "NetWork.h"
#include "DataBase.h"
#include "World.h"

class CX_LIB App
{
public:
	enum APP_TYPE
	{
		APP_TYPE_LOGIN,
		APP_TYPE_BASE,
	};
public:

	static Config Config;
	static DataBase DataBase;
	static MonoEngine Script;
	static NetWork	Net;
	static World World;

public:
	App(int narg,const char** args);
	virtual ~App();

	virtual bool initialize();

	void run();

	void finish();

private:
	APP_TYPE mType;
	uCommandLine mCommandLine;
};
