#include "stdafx.h"
#include "ScriptObject.h"
#include "App.h"


//const char* ScriptObject::className()
//{
//	return "S_ScriptObject";
//}
//
//const char* ScriptObject::dbClassName()
//{
//	return "ScriptObject";
//}

ScriptObject::ScriptObject()
{
}


ScriptObject::~ScriptObject()
{
	this->saveToDB();
}

bool ScriptObject::init()
{
	mObject = App::Script.newObject(nameSpace(), className());
	mDBObject = App::Script.newObject(dbNameSpace(), dbClassName());

	return mObject != nullptr;
}

void ScriptObject::call(const char* method, ScriptArgs* args)
{
	App::Script.invoke(nameSpace(), className(), method, mObject, args ? args->pointer() : nullptr);
}

void ScriptObject::callRemote(const char* method, ScriptArgs* args)
{
	App::Script.invoke(nameSpace(), className(), method, mObject, args ? args->pointer() : nullptr);
}

void ScriptObject::setDBField(const char* field, const char* value)
{
	string strvalue = value;
	setDBField(field, strvalue);;
}

void ScriptObject::setDBField(const char* field, const string& value)
{
	MonoString* monostr = mono_string_new(App::Script.domain(), value.c_str());
	setTheField(dbNameSpace(), dbClassName(), field, monostr, mDBObject);
}


void ScriptObject::getDBField(const char* field, string& value)
{
	getTheField(dbNameSpace(), dbClassName(), field, value, mDBObject);
}

bool ScriptObject::fetchByDBField(const char* field)
{
	ScriptArgs arg;
	arg << field;
	return App::Script.invoke<bool>(dbNameSpace(), dbClassName(), "fetchByFieldQuery", mDBObject, arg.pointer());
}

bool ScriptObject::saveToDB()
{
	bool res = App::Script.invoke<bool>(dbNameSpace(), dbClassName(), "saveToDB", mDBObject, nullptr);
	return res;
}

bool ScriptObject::createAndInsertToDB()
{
	bool res = App::Script.invoke<bool>(dbNameSpace(), dbClassName(), "createAndInsertToDB", mDBObject, nullptr);
	return res;
}

void ScriptObject::requireGUID()
{
	mGUID = uPlatform::generateGUIDSimpleString();
	setDBField("guid", mGUID);
}

void ScriptObject::updateGUIDByScript()
{
	getDBField("guid", mGUID);
}

const char* ScriptObject::nameSpace() const
{
	return YW_SCRIPT_NAME_SPACE;
}

const char* ScriptObject::dbNameSpace() const
{
	return YW_DATABASE_SPACE;
}



void ScriptObject::setTheField(const char* namespacename, const char* classname, const char* field, string& value, MonoObject* obj)
{
	MonoString* monostr = mono_string_new(App::Script.domain(), value.c_str());
	setTheField(namespacename, classname, field, monostr, obj);
}

void ScriptObject::setTheField(const char* namespacename, const char* classname, const char* field, MonoString* value, MonoObject* obj)
{
	MonoClass *klass = getMonoClass(namespacename, classname);
	if (!klass)
		return;
	MonoClassField * monoField = mono_class_get_field_from_name(klass, field);
	if (!monoField)
		return;
	mono_field_set_value(obj, monoField, value);
}

void ScriptObject::getTheField(const char* namespacename, const char* classname, const char* field, string& value, MonoObject* obj)
{
	MonoClass *klass = getMonoClass(namespacename, classname);
	if (!klass)
		return;
	MonoClassField * monoField = mono_class_get_field_from_name(klass, field);
	if (!monoField)
		return;
	MonoString* res = nullptr;
	mono_field_get_value(obj, monoField, &res);
	if (res)
		value = mono_string_to_utf8(res);
}

MonoClass* ScriptObject::getMonoClass(const char* namespacename, const char* classname)
{
	return App::Script.getClass(namespacename, classname);
}

const char* ScriptObject::YW_SCRIPT_NAME_SPACE = "Scripts";

const char* ScriptObject::YW_DATABASE_SPACE = "Scripts.DataBase";
