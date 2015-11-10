#include "stdafx.h"
#include "ScriptObject.h"
#include "App.h"


void ScriptObject::setGuid(const char* guid)
{
	mGUID = guid;
}

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
	:mNameSpace(YW_SCRIPT_NAME_SPACE)
{
}


ScriptObject::ScriptObject(const char* classname, const char* namespacename /*= nullptr*/)
	: mNameSpace(YW_SCRIPT_NAME_SPACE)
{
	if (classname)
		mClassName = classname;
	if (namespacename)
		mNameSpace = namespacename;
}

ScriptObject::~ScriptObject()
{
	//this->saveToDB();
}



bool ScriptObject::init()
{
	mObject = App::Script.newObject(nameSpace(), className());
	return mObject != nullptr;
}




const char* ScriptObject::className()
{
	return mClassName.c_str();
}


const char* ScriptObject::nameSpace() const
{
	return mNameSpace.c_str();
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

void ScriptObject::setTheField(const char* namespacename, const char* classname, const char* field, MonoObject* value, MonoObject* obj)
{
	MonoClass *klass = getMonoClass(namespacename, classname);
	if (!klass)
		return;
	MonoClassField * monoField = mono_class_get_field_from_name(klass, field);
	if (!monoField)
		return;
	mono_field_set_value(obj, monoField, value);
}

void ScriptObject::setTheField(const char* namespacename, const char* classname, const char* field, MonoArray* value, MonoObject* obj)
{
	MonoClass *klass = getMonoClass(namespacename, classname);
	if (!klass)
		return;
	MonoClassField * monoField = mono_class_get_field_from_name(klass, field);
	if (!monoField)
		return;
	mono_field_set_value(obj, monoField, value);
}

void ScriptObject::setTheField(const char* namespacename, const char* classname, const char* field, const char* value, MonoObject* obj)
{
	MonoClass *klass = getMonoClass(namespacename, classname);
	if (!klass)
		return;
	MonoClassField * monoField = mono_class_get_field_from_name(klass, field);
	if (!monoField)
		return;
	MonoString* res = monoNewString(value);
	mono_field_set_value(obj, monoField, res);
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

template<>
void ScriptObject::call(const char* method, ScriptArgs* args)
{
	App::Script.invoke(nameSpace(), className(), method, mObject, args ? args->pointer() : nullptr);
}

MonoString* ScriptObject::monoNewString(const char* str)
{
	return App::Script.createString(str);
}

const char* ScriptObject::YW_SCRIPT_NAME_SPACE = "DataBase";

