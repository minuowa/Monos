#pragma once
#include "ScriptArgs.h"
#include "mono\metadata\object.h"

class CX_LIB ScriptObject
{
public:
	virtual const char* className() = 0;
	virtual const char* dbClassName() = 0;
	const char* nameSpace() const;
	const char* dbNameSpace() const;

	const char* guid();

public:

	ScriptObject();
	~ScriptObject();

	virtual bool init();

	void call(const char* method, ScriptArgs* args);

	void callRemote(const char* method,ScriptArgs* args);

	bool fetchByDBField(const char* field);

	bool saveToDB();
	bool createAndInsertToDB();
	void requireGUID();
	void updateGUIDByScript();

public:

	template<typename T>
	void setField(const char* field, T& value);

	template<typename T>
	void getField(const char* field, T& value);


	template<typename T>
	void setDBField(const char* field, const T& value);
	void setDBField(const char* field, const char* value);
	void setDBField(const char* field, const string& value);

	template<typename T>
	void getDBField(const char* field, T& value);
	void getDBField(const char* field, string& value);

private:

	template<typename T>
	void setTheField(const char* namespacename, const char* classname, const char* field, T& value, MonoObject* obj);
	void setTheField(const char* namespacename, const char* classname, const char* field, string& value, MonoObject* obj);
	void setTheField(const char* namespacename, const char* classname, const char* field, MonoString* value, MonoObject* obj);

	template<typename T>
	void getTheField(const char* namespacename, const char* classname, const char* field, T& value, MonoObject* obj);
	void getTheField(const char* namespacename, const char* classname, const char* field, string& value, MonoObject* obj);

protected:

	MonoClass* getMonoClass(const char* namespacename, const char* classname);

protected:

	MonoObject* mObject;
	
	MonoObject* mDBObject;

private:
	
	string mGUID;

private:

	static const char* YW_SCRIPT_NAME_SPACE;

	static const char* YW_DATABASE_SPACE;
};

template<typename T>
void ScriptObject::setField(const char* field, T& value)
{
	setTheField(nameSpace(), className(), value, mObject);
}

template<typename T>
void ScriptObject::getField(const char* field, T& value)
{
	getField(nameSpace(), className(), field, value, mObject);
}

template<typename T>
void ScriptObject::setTheField(const char* namespacename, const char* classname, const char* field, T& value, MonoObject* obj)
{
	MonoClass *klass = getMonoClass(namespacename, classname);
	if (!klass)
		return;
	MonoClassField * monoField = mono_class_get_field_from_name(klass, field);
	if (!monoField)
		return;
	mono_field_set_value(obj, monoField, (void*)&value);
}

template<typename T>
void ScriptObject::getTheField(const char* namespacename, const char* classname, const char* field, T& value, MonoObject* obj)
{
	MonoClass *klass = getMonoClass(namespacename, classname);
	if (!klass)
		return;
	MonoClassField * monoField = mono_class_get_field_from_name(klass, field);
	if (!monoField)
		return;
	mono_field_get_value(obj, monoField, &value);
}
template<typename T>
void setDBField(const char* field, T value)
{
	setTheField(dbNameSpace(), dbClassName(), field, value, mDBInterface);
}

template<typename T>
void ScriptObject::setDBField(const char* field, const T& value)
{
	setTheField(dbNameSpace(), dbClassName(), field, value, mDBObject);
}

template<typename T>
void ScriptObject::getDBField(const char* field, T& value)
{
	getTheField(dbNameSpace(), dbClassName(), field, value, mDBObject);
}

inline const char* ScriptObject::guid()
{
	return mGUID.c_str();
}
