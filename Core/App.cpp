#include "stdafx.h"
#include "App.h"

App::App(int narg, const char** args)
    : mType(APP_TYPE_LOGIN)
    , mCommandLine(narg, args) {

}

App::~App() {
}

bool App::initialize() {
    if (mCommandLine.arg_count()) {
        mCommandLine.get("type", (int&)mType);
        mCommandLine.get("serverID", mServerID);
    }

    if (!Config.initialize("..\\config.xml"))
        return false;

    if (!InitNet())
        return false;

    if (!InitDataBase())
        return false;

    if (!InitScript())
        return false;

    if (!World.initialize())
        return false;
    return true;
}

void App::run() {
    while (true) {
        Net.prosess();
        uPlatform::sleep(5);
    }
}

void App::finish() {
    //Script.finish();
}

void App::archive() {

}

bool App::InitNet() {
    if (mType == APP_TYPE_LOGIN) {
        if (!Net.initialize(Config.login.net))
            return false;
    } else if (mType == APP_TYPE_CENTER) {
        NetConfig* netconfig;
        if (!Config.center.centers.Get(mServerID, netconfig)) {
            CXASSERT_RESULT_FALSE(0);
        } else {
            if (!Net.initialize(*netconfig))
                return false;
        }
    } else {
        CXASSERT_RESULT_FALSE(0);
    }
    return true;
}

bool App::InitDataBase() {
    if (mType == APP_TYPE_LOGIN) {
        if (!DataBase.initialize(Config.login.db))
            return false;
    } else if (mType == APP_TYPE_CENTER) {
        if (!DataBase.initialize(Config.center.db))
            return false;
    } else {
        CXASSERT_RESULT_FALSE(0);
    }
    return true;
}

bool App::InitScript() {
    //ScriptConfig scriptcfg;

    //if (mType == APP_TYPE_LOGIN) {
    //    scriptcfg = Config.login.script;

    //} else if (mType == APP_TYPE_CENTER) {
    //    scriptcfg = Config.center.script;
    //} else {
    //    CXASSERT_RESULT_FALSE(0);
    //}

    //if (scriptcfg.open) {
    //    if (!Script.initialize())
    //        return false;
    //}

    return true;


}

Config App::Config;

DataBase App::DataBase;

NetWork App::Net;

World App::World;
