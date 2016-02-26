#include "Account.h"
#ifndef World_h__
#define World_h__

class CX_LIB World :public uCallBack
{
public:
	World();
	virtual ~World();

	uDelegate onAccountEnterWorld;
	uDelegate onAccountLeaveWorld;

	bool initialize();

public:
	
	void reclaimAccount(Account* account);

	void onEnterWorld(Account* account);

	void sync(string account_guid, string cmd);

	virtual void onCallBack(const uDelegate& d, uEventArgs* e) override;

private:
	uMap<string, Account*> mAccounts;
};

#endif // World_h__
