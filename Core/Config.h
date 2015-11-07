#pragma once
#include "ConfigInfo.h"
class CX_LIB Config
{
public:
	Config();

	~Config();
public:

	CommonConfig loginApp;

public:

	bool initialize(const char* file);
};

