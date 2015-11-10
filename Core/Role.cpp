#include "stdafx.h"
#include "Role.h"


Role::Role()
{
	mClassName = "S_Role";
}


Role::~Role()
{
}

void Role::enterScene()
{
	this->call("enterScene", nullptr);
}


