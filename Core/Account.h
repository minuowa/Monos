#pragma once
#include "ScriptObject.h"
#include "Connection.h"
#include "DBAccount.h"
class Role;
class CX_LIB Account
	:public ScriptObject
{
public:
	
	Account();

	~Account();

	virtual bool init() override;

	void enterGate();

	void onRqCreateRole();

	void onRoleRqEnterWorld(string guid);

	void sendDB(string data);

	void set_connect(Connection* connect);

	DBAccount* getDBObject();

	void sync(string data);
protected:

	void createRole();


protected:

	DBAccount* mDBObject;

	Role* mActiveRole;

	Connection* mConnect;
};

