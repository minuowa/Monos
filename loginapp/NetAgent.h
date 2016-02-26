#pragma once
class Account;
class NetAgent:public uCallBack
{
public:

	NetAgent();
	~NetAgent();

	virtual void onCallBack(const uDelegate& d, uEventArgs* e) override;

protected:
	bool on_rqCreateAccount(string user, string psd);
	bool on_rqLogin(string user, string psw, Connection* con);
protected:
	bool createAccount(string user, string psd);
private:
	static const char* YW_DB;
	static const char* YW_TABLE_ACCOUNT;
	static const char* YW_TABLE_ACCOUNT_USER;
	static const char* YW_TABLE_ACCOUNT_PSD;
	static const char* YW_TABLE_ACCOUNT_ID;
private:
	static uMap<Connection*, int> mClients;
};

