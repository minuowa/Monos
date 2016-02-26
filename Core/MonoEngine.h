#pragma once
/*
	在子线程中执行mono会发生异常。
*/
class CX_LIB MonoEngine
{
public:
	MonoEngine();
	~MonoEngine();

	bool initialize();

	void finish();

	void registAllFunction();

	MonoObject* newObject(const char* namespacename, const char* classname);

	MonoDomain* domain();

	MonoArray* createArray(const char* namespacename, const char* classname, u32 cnt);

	MonoArray* createByteArray(u32 cnt,void* data);

	MonoArray* createArray(MonoClass* monoclass, u32 cnt);

	MonoString* createString(const char* text);

	//MonoArray* createStringArray(const char* namespacename, const char* classname, u32 cnt);

	//mono_array_new

	MonoObject* simpleInvoke(const char* namespacename, const char* classname, const char* method, MonoObject* instance, void** params);
	
	template<typename T = void> T invoke(const char* namespacename, const char* classname, const char* method, MonoObject* instance, void** params);
	
	template<> string invoke(const char* namespacename, const char* classname, const char* method, MonoObject* instance, void** params);
	
	template<> void invoke(const char* namespacename, const char* classname, const char* method, MonoObject* instance, void** params);

	void registerFunction(const char* namespacename, const char* classname, const char* method, const void* fun, bool statical=true);

	MonoClass* getClass(const char* namespacename, const char* classname);

private:
	
	bool initBase();

	void debug_info();

private:

	MonoDomain* mDomain;
	MonoAssembly* mAssembly;
	MonoImage* mImage;
	const char* mManagedBinaryPath;
};

inline MonoDomain* MonoEngine::domain()
{
	return mDomain;
}
template<typename T>
T MonoEngine::invoke(const char* namespacename, const char* classname, const char* method, MonoObject* instance, void** params)
{
	MonoObject* ret = simpleInvoke(namespacename, classname, method, instance, params);
	T t=Default::DefaultValue<T>();
	if (ret)
		t = *((T*)mono_object_unbox(ret));
	return t;
}
template<>
string MonoEngine::invoke(const char* namespacename, const char* classname, const char* method, MonoObject* instance, void** params)
{
	MonoString* ret = (MonoString*)simpleInvoke(namespacename, classname, method, instance, params);
	assert(ret);
	return mono_string_to_utf8(ret);
}
template<>
void MonoEngine::invoke(const char* namespacename, const char* classname, const char* method, MonoObject* instance, void** params)
{
	simpleInvoke(namespacename, classname, method, instance, params);
}