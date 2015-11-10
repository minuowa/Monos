#pragma once
class CX_LIB ScriptArgs
{
public:

	ScriptArgs();

	~ScriptArgs();

	template<typename T>
	ScriptArgs& operator<<(const T& arg);

	void** pointer();

	void clear();

private:

	template<typename T>
	void* makeArg(const T& arg){
		return (void*)&arg;
	}
	void* makeArg(const char* arg);
	
	void* makeArg(const string& arg);
	void* makeArg(MonoArray* arg);
	void* makeArg(MonoObject* arg);
private:

	static const int MAX_ARG = 16;

protected:
	int mArgCount;
	void* mArgs[MAX_ARG];
	MonoDomain* mDomain;
};

template<typename T>
ScriptArgs& ScriptArgs::operator<<(const T& arg)
{
	assert(mArgCount < MAX_ARG);
	mArgs[mArgCount] = makeArg(arg);
	mArgCount++;
	return *this;
}


inline  void** ScriptArgs::pointer()
{
	return mArgs;
}


