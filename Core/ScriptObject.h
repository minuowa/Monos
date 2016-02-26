#pragma once
#include "ScriptArgs.h"

class CX_LIB ScriptObject
{
public:
	
	const char* className();

	const char* nameSpace() const;
	
	ScriptObject();
	ScriptObject(const char* classname, const char* namespacename = nullptr);

	~ScriptObject();

public:
	MonoObject* get();

	const char* guid();
	virtual void setGuid(const char* guid);

	virtual bool init();

	template<typename T = void> 
	T call(const char* method, ScriptArgs* args = nullptr);

	template<> 
	void call(const char* method, ScriptArgs* args);

	MonoString* monoNewString(const char* str);

public:

	template<typename T>
	void setField(const char* field, T value);

	template<typename T>
	inline void getField(const char* field, T& value);
	template<>
	inline void ScriptObject::getField(const char* field, string& value);
	inline void ScriptObject::getField(const char* field, MonoString* value);
private:

	template<typename T>
	void setTheField(const char* namespacename, const char* classname, const char* field, T& value, MonoObject* obj);
	void setTheField(const char* namespacename, const char* classname, const char* field, string& value, MonoObject* obj);
	void setTheField(const char* namespacename, const char* classname, const char* field, MonoString* value, MonoObject* obj);
	void setTheField(const char* namespacename, const char* classname, const char* field, MonoObject* value, MonoObject* obj);
	void setTheField(const char* namespacename, const char* classname, const char* field, MonoArray* value, MonoObject* obj);
	void setTheField(const char* namespacename, const char* classname, const char* field, const char* value, MonoObject* obj);

	template<typename T>
	void getTheField(const char* namespacename, const char* classname, const char* field, T& value, MonoObject* obj);
	void getTheField(const char* namespacename, const char* classname, const char* field, string& value, MonoObject* obj);

protected:

	MonoClass* getMonoClass(const char* namespacename, const char* classname);

protected:

	MonoObject* mObject;
	
	string mGUID;

protected:

	string mClassName;
	string mNameSpace;

private:
	static const char* YW_SCRIPT_NAME_SPACE;
};

template<typename T>
T ScriptObject::call(const char* method, ScriptArgs* args)
{
	return App::Script.invoke<T>(nameSpace(), className(), method, mObject, args ? args->pointer() : nullptr);
}

template<typename T>
void ScriptObject::setField(const char* field, T value)
{
	setTheField(nameSpace(), className(), field, value, mObject);
}

template<typename T>
void ScriptObject::getField(const char* field, T& value)
{
	getTheField(nameSpace(), className(), field, value, mObject);
}
template<>
inline void ScriptObject::getField(const char* field, string& value)
{
	getTheField(nameSpace(), className(), field, value, mObject);
}
inline void ScriptObject::getField(const char* field, MonoString* value)
{
	getTheField(nameSpace(), className(), field, value, mObject);
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
inline MonoObject* ScriptObject::get()
{
	return mObject;
}