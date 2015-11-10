#include "stdafx.h"
#include "DBObject.h"
#include "App.h"


DBObject::DBObject()
{
}


DBObject::~DBObject()
{
}

bool DBObject::fetchByField(const char* field)
{
	ScriptArgs arg;
	arg << field;
	return App::Script.invoke<bool>(nameSpace(), className(), "fetchByField", mObject, arg.pointer());
}

void DBObject::sync(string data)
{

}

bool DBObject::fetch()
{
	bool res = fetchByField("guid");
	//callDB("onFetch", nullptr);
	return res;
}

bool DBObject::saveToDB()
{
	bool res = call<bool>("saveToDB", nullptr);
	return res;
}

bool DBObject::createAndInsertToDB()
{
	bool res = call<bool>("createAndInsertToDB", nullptr);
	return res;
}

void DBObject::requireGUID()
{
	mGUID = uPlatform::generateGUIDSimpleString();
	setField("guid", mGUID);
}

void DBObject::updateGUIDByScript()
{
	setField("guid", mGUID);
}



void DBObject::setGuid(const char* guid)
{
	__super::setGuid(guid);
	setField("guid", guid);
}
