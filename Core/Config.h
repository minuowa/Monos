#pragma once
#include "ConfigInfo.h"
class CX_LIB Config
{
public:
	Config();

	~Config();
public:
	LoginConfig login;
	CenterConfig center;
public:

	bool initialize(const char* file);
};

