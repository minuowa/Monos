#ifndef PKG_h__
#define PKG_h__

#include "Default.h"

#pragma pack(push)
#pragma pack(1)

#define zeroArray(arr) memset(arr,0,sizeof(arr))

namespace PCODE
{
	enum ID :int
	{
		NONE,
		SYN,
		ACK,
		Create_Account,
		Login,
		Login_Result,
	};
}
enum CreateAccountResult
{
	CS_Require,
	SC_Failed_Exist,
};
enum LoginResult
{
	None,
	Successed,
	Failed,
	SC_Failed_PSD_Error,
};
struct PKG
{
	PCODE::ID code;
	u32 childid;
	PKG() 
	{
		code = PCODE::NONE;
		childid = 0;
	}
};
struct PKGACK :PKG
{
	int key;
	PKGACK()
		:key(1234)
	{
		code = PCODE::ACK;
	}
};
struct PKGSYN:PKG 
{
	int key;
	PKGSYN()
		:key(1234)
	{
		code = PCODE::SYN;
	}
};
struct rqCreateAccount :PKG
{
	char user[Default::NameSize];
	char psd[Default::PSDSize];
	rqCreateAccount()
	{
		zeroArray(user);
		zeroArray(psd);
		code = PCODE::Create_Account;
		childid = CS_Require;
	}
};

struct rtCreateAccountFailed_Exist :PKG
{
	rtCreateAccountFailed_Exist()
	{
		code = PCODE::Create_Account;
		childid = SC_Failed_Exist;
	}
};

struct rqLogin:PKG 
{
	char user[Default::NameSize];
	char psd[Default::PSDSize];
	rqLogin()
	{
		code = PCODE::Login;
		zeroArray(user);
		zeroArray(psd);
	}
};

struct rtLogin :PKG
{
	int res;
	rtLogin()
	{
		code = PCODE::Login_Result;
	}
};
#pragma pack(pop)
#endif // PKG_h__
