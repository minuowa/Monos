#include "Account.h"
#ifndef World_h__
#define World_h__

class CX_LIB World
{
public:
	World();
	virtual ~World();

	bool initialize();

public:
	
	void reclaimAccount(Account* account);

	void onEnterWorld(Account* account);

private:
	map<string, Account*> mAccounts;
};

#endif // World_h__
