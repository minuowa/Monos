#include "stdafx.h"
#include "NetAgent.h"
#include "Account.h"

NetAgent::NetAgent() {
}


NetAgent::~NetAgent() {
}

void NetAgent::onCallBack(const uDelegate& d, uEventArgs* e) {
    if (d == App::Net.onMessage) {
        NetWork::MsgArgs* arg = (NetWork::MsgArgs*)e;
        PKG* pkg = arg->pkg;
        Connection* connect = arg->connect;

        switch (pkg->opcode) {
        case OPCODE::CenterClient: {
            switch (pkg->childid) {
            case OPCODE::CenterClientType::RqLogin: {
                rqLoginGame* rq = (rqLoginGame*)pkg;
                on_rqLogin(rq->user, rq->psw, connect);
            }
            break;
            case OPCODE::CenterClientType::RqAccount: {
                rqAccount* rq = (rqAccount*)pkg;
                on_rqCreateAccount(rq->user, rq->psw, connect);
            }
            break;
            case OPCODE::CenterClientType::ClientMessage: {
                rqClientMessage* rq = (rqClientMessage*)pkg;

                //MonoArray* arr = App::Script.createByteArray(rq->count, rq->data);

                //ScriptArgs arg;
                //arg << arr;
                //mScriptObject.call("OnMessage", &arg);
            }
            break;
            default:
                break;
            }
        }
        break;
        default:
            break;
        }
    } else if (d == App::Net.onDisconnect) {
        NetWork::ConnectArg* arg = (NetWork::ConnectArg*)e;
        mClients.erase(arg->connect);
    }
}

bool NetAgent::on_rqCreateAccount(string user, string psw, Connection* con) {
    rtAccount pkg;
    if (App::DataBase.queryColValue(YW_TABLE_ACCOUNT, YW_TABLE_ACCOUNT_USER, user.c_str())) {
        pkg.errorCode = AccountErrorCode::AccountErrorCode_Existed;
        SendPKG(con->getSocket(), pkg);
        return false;
    } else {
        Account* gateAccount = new Account();

        if (!gateAccount->init()) {
            delete gateAccount;
            assert(0);
            pkg.errorCode = AccountErrorCode::AccountErrorCode_None;
            SendPKG(con->getSocket(), pkg);
            return false;
        }

        gateAccount->getDBInterface()->setField(YW_TABLE_ACCOUNT_USER, user);

        if (!gateAccount->getDBInterface()->fetchByField(YW_TABLE_ACCOUNT_USER)) {
            gateAccount->getDBInterface()->setField(YW_TABLE_ACCOUNT_PSD, psw);
            gateAccount->getDBInterface()->setField(YW_TABLE_ACCOUNT_ID, uPlatform::generateGUIDSimpleString());

            //does not exist,create successfully!
            if (gateAccount->getDBInterface()->createAndInsertToDB()) {
                pkg.errorCode = AccountErrorCode::AccountErrorCode_Sucessed;
                SendPKG(con->getSocket(), pkg);
                return true;
            }
        } else {
            pkg.errorCode = AccountErrorCode::AccountErrorCode_Existed;
            SendPKG(con->getSocket(), pkg);
        }
    }
    return false;
}


bool NetAgent::on_rqLogin(string user, string psw, Connection* con) {
    //the account enter the net gate
    rtLoginGame pkg;
    Account* gateAccount = new Account();
    if (!gateAccount->init()) {
        delete gateAccount;
        pkg.errorCode = LoginErrorCode::LoginErrorCode_None;
        SendPKG(con->getSocket(), pkg);
        return nullptr;
    }

    bool res = false;

    gateAccount->setConnection(con);

    gateAccount->getDBInterface()->setField(YW_TABLE_ACCOUNT_USER, user);

    if (!gateAccount->getDBInterface()->fetchByField(YW_TABLE_ACCOUNT_USER)) {
        if (App::Config.center.db.autoCreateAccount) {
            gateAccount->getDBInterface()->setField(YW_TABLE_ACCOUNT_PSD, psw);
            gateAccount->getDBInterface()->setField(YW_TABLE_ACCOUNT_ID, uPlatform::generateGUIDSimpleString());

            //does not exist,create successfully!
            if (gateAccount->getDBInterface()->createAndInsertToDB()) {
                pkg.errorCode = LoginErrorCode::Sucessed;
                res = true;
            } else {
                pkg.errorCode = LoginErrorCode::UserCantFind;
                res = false;
            }
        }
    } else {
        string strpwd;
        gateAccount->getDBInterface()->getField(YW_TABLE_ACCOUNT_PSD, strpwd);
        if (strpwd == psw) {
            //exist,load successfully!
            pkg.errorCode = LoginErrorCode::Sucessed;
            res = true;
        }
    }

    SendPKG(con->getSocket(), pkg);

    if (res) {
        onLoginSucess(gateAccount);
    }

    return res;
}

void NetAgent::onLoginSucess(Account* account) {
    account->enterWorld();
}

const char* NetAgent::YW_DB = "yourworld";

const char* NetAgent::YW_TABLE_ACCOUNT = "yw_account";

const char* NetAgent::YW_TABLE_ACCOUNT_USER = "user";

const char* NetAgent::YW_TABLE_ACCOUNT_PSD = "psw";

const char* NetAgent::YW_TABLE_ACCOUNT_ID = "guid";

uMap<Connection*, int> NetAgent::mClients;

