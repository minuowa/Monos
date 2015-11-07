#include "stdafx.h"
#include "ScriptArgs.h"
#include "App.h"
#include "MonoEngine.h"


ScriptArgs::ScriptArgs()
	:mArgCount(0)
	, mDomain(0)
{
	dMemoryZeroArray(mArgs);
	mDomain = App::Script.domain();
}


ScriptArgs::~ScriptArgs()
{
}

void ScriptArgs::clear()
{
	dMemoryZeroArray(mArgs);
	mArgCount = 0;
}

void* ScriptArgs::makeArg(const char* arg)
{
	string strarg = arg;
	return makeArg(strarg);
}


void* ScriptArgs::makeArg(const string& arg)
{
	MonoString* str = mono_string_new(mDomain, arg.c_str());
	return str;
}

