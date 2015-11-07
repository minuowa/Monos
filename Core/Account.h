#pragma once
#include "ScriptObject.h"

class CX_LIB Account:public ScriptObject
{
public:
	
	virtual const char* className();
	virtual const char* dbClassName();

public:
	
	Account();
	~Account();

	bool fetch();

	void enterScene(int sceneid);

	void enterWorld();
};

