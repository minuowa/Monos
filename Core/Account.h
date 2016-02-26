#pragma once
#include "ScriptObject.h"
#include "Connection.h"
#include "DBInterface.h"
class Role;
class CX_LIB Account
	:public ScriptObject
{
public:
	
	Account();

	~Account();

	virtual bool init() override;

	void enterGate();

	void onRoleRqEnterWorld(string guid);

	void sendDBToClient(string data);

	void setConnection(Connection* connect);

	DBInterface* getDBInterface();

	void sync(string data);

	void enterWorld();

	inline Connection* getNetInterface();

protected:

	void createRole();

protected:

	DBInterface* mDBInterface;

	Role* mActiveRole;

	Connection* mNetInterface;
};
inline Connection* Account::getNetInterface()
{
	return mNetInterface;
}
