#ifndef PKG_h__
#define PKG_h__

#include "Default.h"

#pragma pack(push)
#pragma pack(1)

#define zeroArray(arr) memset(arr,0,sizeof(arr))
#define zeroThis(p) memset(p,0,sizeof(*p))

namespace OPCODE
{
	enum ID :int
	{
		None,

		ClientLogin,

		CenterLogin,

		CenterClient,

		Create_Account,

		DBData,

		Protocolbuf,
	};

	enum LoginToCenterType :int
	{
		L2C_User,
	};
	enum ClientLoginType
	{
		RqGameServerInfo,
		RtGameServerInfo,
	};
	enum CenterLoginType
	{
		AppStart,
		AppInfo,
	};
	enum CenterClientType
	{
		RqLogin,
		RqAccount,

		RtLogin,
		RtAccount,

		ClientMessage,
		ServerMessage,
	};
}

struct PKG
{
	u32 opcode;
	u32 childid;
	PKG()
	{
		opcode = OPCODE::None;
		childid = 0;
	}
};
struct PKG_CenterLogin :PKG
{
	PKG_CenterLogin()
	{
		opcode = OPCODE::CenterLogin;
	}
};
struct PKG_CenterClient :PKG
{
	PKG_CenterClient()
	{
		this->opcode = OPCODE::CenterClient;
	}
};
enum AccountAction
{
	AccountAction_None,
	AccountAction_Create,
	AccountAction_Rename,
	AccountAction_Delete,
};

enum AccountErrorCode
{
	AccountErrorCode_None,
	AccountErrorCode_Sucessed,
	AccountErrorCode_Existed,
};

struct rqAccount :PKG_CenterLogin
{
	char user[Default::NameSize];
	char psw[Default::PasswordSize];

	AccountAction act;

	rqAccount()
	{
		this->childid = OPCODE::CenterClientType::RqAccount;
		act = AccountAction::AccountAction_None;
		zeroArray(user);
		zeroArray(psw);
	}
};

struct rtAccount :PKG_CenterClient
{
	AccountErrorCode errorCode;
	rtAccount()
	{
		this->childid = OPCODE::CenterClientType::RtAccount;
		errorCode = AccountErrorCode::AccountErrorCode_None;
	}
};

struct rtDBData :PKG
{
	char data[Default::DataSize];
	rtDBData()
	{
		zeroThis(this);
		opcode = OPCODE::DBData;
	}
};


struct rqClientMessage :PKG_CenterClient
{
	u32 count;

	char data[0];

	rqClientMessage()
	{
		count = 0;
		this->childid = OPCODE::CenterClientType::ClientMessage;
	}
};

enum LoginErrorCode
{
	LoginErrorCode_None,
	Sucessed,
	UserCantFind,
	PasswordError,
};
struct rqLoginGame :PKG_CenterClient
{
	char user[Default::NameSize];
	char psw[Default::NameSize];
	rqLoginGame()
	{
		zeroThis(this);
		this->childid = OPCODE::CenterClientType::RqLogin;
	}
};
struct rtLoginGame :PKG_CenterClient
{
	LoginErrorCode errorCode;

	rtLoginGame()
	{
		this->childid = OPCODE::CenterClientType::RtLogin;
	}
};
struct ClientToLoginPKG :PKG
{
	ClientToLoginPKG()
	{
		opcode = OPCODE::ClientLogin;
	}
};
struct ClientToLogin_RqGameServerInfo :ClientToLoginPKG
{
	int serverID;

	ClientToLogin_RqGameServerInfo()
	{
		this->childid = OPCODE::ClientLoginType::RqGameServerInfo;
		serverID = 0;
	}
};
struct ClientToLogin_RtGameServerInfo :ClientToLoginPKG
{
	char host[Default::HostSize];
	char name[Default::NameSize];

	int port;

	ClientToLogin_RtGameServerInfo()
	{
		this->childid = OPCODE::ClientLoginType::RtGameServerInfo;
		zeroArray(host);
		zeroArray(name);
		port = 0;
	}
};
struct PKG_ClientLogin :PKG
{
	PKG_ClientLogin()
	{
		opcode = OPCODE::ClientLogin;
	}
};



struct l2c_UsrPKG :PKG_ClientLogin
{
	int userTmpID;

	l2c_UsrPKG()
	{
		childid = OPCODE::L2C_User;
	}
};
struct c2l_CenterAppStartInfoPKG :PKG_CenterLogin
{
	char host[Default::HostSize];
	char name[Default::NameSize];

	int port;

	c2l_CenterAppStartInfoPKG()
	{
		this->childid = OPCODE::CenterLoginType::AppStart;
		zeroArray(host);
		zeroArray(name);
		port = 0;
	}
};

struct rtCenterAppInfo :PKG_CenterLogin
{
	u32 userCount;
	rtCenterAppInfo()
	{
		this->childid = OPCODE::CenterLoginType::AppInfo;
		userCount = 0;
	}
};
#pragma pack(pop)
#endif // PKG_h__
