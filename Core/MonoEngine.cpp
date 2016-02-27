#include "stdafx.h"
#include "MonoEngine.h"

#include <mono/mini/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-debug.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/object.h>
#include <mono/metadata/threads.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/mono-gc.h>
#include <assert.h>
#include <string>
#include "App.h"
#include "Test.h"

MonoEngine::MonoEngine()
	:mManagedBinaryPath("..\\scripts\\Scripts.dll")
	,mImage(nullptr)
{
}


MonoEngine::~MonoEngine()
{
}

bool MonoEngine::initialize()
{
	if (!initBase())
		return false;

	registAllFunction();

	gTest.DoWork();

	ScriptObject initializer("DataInitializer","DataBase");
	initializer.init();
	bool res = initializer.call<bool>("Init");
	assert(res);


	return res;
}


void MonoEngine::finish()
{
	if (mDomain)
	{
		mono_jit_cleanup(mDomain);
		mono_domain_unload(mDomain);
		mDomain = nullptr;
	}
}
namespace CoreScript
{
	bool DB_hasTable(MonoString* table)
	{
		string stable = mono_string_to_utf8(table);
		return App::DataBase.hasTable(stable.c_str());
	}
	bool DB_createEmptyTable(MonoString* table)
	{
		assert(0);
		string stable = mono_string_to_utf8(table);
		return true;
	}
	bool DB_createTable(MonoString* table, MonoString* cols)
	{
		string stable = mono_string_to_utf8(table);
		string cmd = mono_string_to_utf8(cols);
		return App::DataBase.createTable(stable.c_str(), cmd.c_str());
	}
	bool DB_alterTableColumnAttribute(MonoString* table, MonoString* cmd)
	{
		return true;
	}
	bool DB_insert(MonoString* tablename,MonoString* cmd)
	{
		string stable = mono_string_to_utf8(tablename);
		string scmd = mono_string_to_utf8(cmd);
		SQLTable* table = App::DataBase.getTable(stable.c_str());
		if (!table)
			return false;
		return table->insert(scmd.c_str());
	}
	bool DB_queryToScriptObject(MonoString* cmd, MonoObject* obj)
	{
		std::string sql = mono_string_to_utf8(cmd);
		std::vector<string> result;
		
		if (!App::DataBase.queryWithOneRecord(sql.c_str(), result))
			return false;

		if (result.size() == 0)
			return false;

		MonoEngine& scriptengine = App::Script;

		//MonoArray* monoStringArray = scriptengine.createArray("System", "String", result.size());
		MonoArray* monoStringArray = scriptengine.createArray(mono_get_string_class(), result.size());

		for (size_t i = 0; i < result.size();++i)
		{
			MonoString* ms = scriptengine.createString(result[i].c_str());
			mono_array_set(monoStringArray,MonoString*, i, ms);
		}

		ScriptArgs args;
		args << monoStringArray << obj;
		bool res = scriptengine.invoke<bool>("DataBase", "DataInitializer", "deseriallize", obj, args.pointer());
		return res;
	}

	void DB_SyncClient(MonoString* account_guid,MonoString* cmd)
	{
		string cmdstring = mono_string_to_utf8(cmd);
		string account_guidstring = mono_string_to_utf8(account_guid);
		App::World.sync(account_guidstring, cmdstring);
	}
}

void MonoEngine::registAllFunction()
{
	registerFunction("DataBase", "DataInitializer", "DB_hasTable", CoreScript::DB_hasTable);
	registerFunction("DataBase", "DataInitializer", "DB_createEmptyTable", CoreScript::DB_createEmptyTable);
	registerFunction("DataBase", "DataInitializer", "DB_createTable", CoreScript::DB_createTable);
	registerFunction("DataBase", "DataInitializer", "DB_alterTableColumnAttribute", CoreScript::DB_alterTableColumnAttribute);
	registerFunction("DataBase", "DataInitializer", "DB_insert", CoreScript::DB_insert);
	registerFunction("DataBase", "DataInitializer", "DB_queryToScriptObject", CoreScript::DB_queryToScriptObject);
	registerFunction("DataBase", "ScriptObject", "DB_Sync", CoreScript::DB_SyncClient);
}

MonoArray* MonoEngine::createArray(const char* namespacename, const char* classname, u32 cnt)
{
	MonoClass* monoClass = getClass(namespacename, classname);
	return mono_array_new(mDomain, monoClass, cnt);
}

MonoArray* MonoEngine::createArray(MonoClass* monoclass, u32 cnt)
{
	return mono_array_new(mDomain, monoclass, cnt);
}


MonoArray* MonoEngine::createByteArray(u32 cnt, void* data)
{
	int lowbound = 0;
	MonoArray* arr = mono_array_new(mDomain, mono_get_byte_class(), cnt);
	dMemoryCopy(mono_array_addr_with_size(arr, sizeof(byte), 0), data, sizeof(byte)*cnt);
	return arr;
}

MonoString* MonoEngine::createString(const char* text)
{
	return mono_string_new(mDomain, text);
}
MonoMethod* mono_class_get_method_from_name_recursive(MonoClass* clazz, MonoMethodDesc* desc)
{
	MonoMethod *method = nullptr;
	while (clazz != nullptr && method == nullptr) {
		method = mono_method_desc_search_in_class(desc, clazz);
		if (method == nullptr)
			clazz = mono_class_get_parent(clazz);
	}
	return method;
}
MonoObject* MonoEngine::simpleInvoke(const char* namespacename, const char* classname, const char* method, MonoObject* instance, void** params)
{
	assert(mImage);
	MonoClass* main_class = mono_class_from_name(mImage, namespacename, classname);

	assert(main_class);

	std::stringstream str_method;
	str_method << namespacename << "." << classname << ":" << method;

	MonoMethodDesc* entry_point_method_desc = mono_method_desc_new(str_method.str().c_str(), true);

	assert(entry_point_method_desc);
	MonoMethod* entry_point_method = mono_class_get_method_from_name_recursive(main_class, entry_point_method_desc);

	assert(entry_point_method);
	mono_method_desc_free(entry_point_method_desc);

	MonoObject *exc;
	MonoObject* ret = nullptr;
	try
	{
		ret = mono_runtime_invoke(entry_point_method, instance, params, &exc);
	}
	catch (std::exception* e)
	{
		printf("%s\n", e->what());
	}
	return ret;
}



void MonoEngine::registerFunction(const char* namespacename, const char* classname, const char* method, const void* fun, bool statical/*=true*/)
{
	stringstream str;
	if (namespacename)
		str << namespacename<<".";
	assert(classname&&method);
	str << classname << (statical ? "::" : "::") << method;
	mono_add_internal_call(str.str().c_str(), fun);
}

MonoClass* MonoEngine::getClass(const char* namespacename, const char* classname)
{
	assert(mImage);
	MonoClass* main_class = mono_class_from_name(mImage, namespacename, classname);
	return main_class;
}

bool MonoEngine::initBase()
{
	const char* min_runtime_lib_path = "..\\Lib";
	//lib path:the path of "mono\\2.0\\mscorlib.dll"
	mono_set_dirs(min_runtime_lib_path,"..\\Etc");

	//int port = (int)(56000 + (GetCurrentProcessId() % 1000));
	int port = 56080;
	char options[255];
	sprintf_s(options, 255, "--debugger-agent=transport=dt_socket,address=127.0.0.1:%d,server=y,suspend=n", port);
	char* args = options;
	mono_jit_parse_options(1, &args);
	mono_debug_init(MONO_DEBUG_FORMAT_MONO);

	mDomain = mono_jit_init("App");
	//mDomain = mono_jit_init(min_runtime_lib_path);
	assert(mDomain);

	//mono_assembly_setrootdir(min_runtime_lib_path);

	mAssembly = mono_domain_assembly_open(mDomain, mManagedBinaryPath);
	if (!mAssembly)
	{
		assert(mAssembly);
		printf("Script Path Error!(%s)", mManagedBinaryPath);
		return false;
	}

	mImage = mono_assembly_get_image(mAssembly);

	debug_info();

	return mImage != nullptr;
}

void MonoEngine::debug_info()
{
	printf("Mono Info:%s", mono_get_runtime_build_info());
}

MonoObject* MonoEngine::newObject(const char* namespacename, const char* classname)
{
	assert(mImage);
	MonoClass* main_class = mono_class_from_name(mImage, namespacename, classname);

	assert(main_class);
	MonoObject* obj= mono_object_new(mDomain, main_class); 
	if (obj)
	{
		invoke(namespacename, classname, ".ctor", obj, nullptr);
	}
	return obj;
}


