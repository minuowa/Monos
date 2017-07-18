#include "stdafx.h"
#include "Account.h"
#include "App.h"
#include "Role.h"
#include "Bundle.h"
Account::Account()
    : mNetInterface(nullptr) {
}


Account::~Account() {
}



void Account::enterGate() {
    this->getDBInterface()->fetch();
    //this->call("enterWorld");
}

void Account::onRoleRqEnterWorld(string guid) {
    mActiveRole = new Role();
    if (!mActiveRole->init()) {
        assert(0);
        return;
    }
    mActiveRole->setGuid(guid.c_str());
    mActiveRole->enterScene();
}

void Account::sendDBToClient(string data) {
    assert(mNetInterface);
    rtDBData rt;
    assert(data.length() < Default::DataSize);
    dMemoryCopy(rt.data, (void*)data.c_str(), data.length());
    SendPKG(mNetInterface->getSocket(), rt);
}

void Account::setConnection(Connection* connect) {
    mNetInterface = connect;
}

DBInterface* Account::getDBInterface() {
    return mDBInterface;
}

void Account::enterWorld() {
    App::World.onEnterWorld(this);
}



bool Account::init() {
    if (!__super::init())
        return false;
    mDBInterface = new DBInterface();
    if (!mDBInterface->init())
        return false;

    //setField("mDBInterface", mDBInterface->get());

    //call("onInitEnd");

    return true;
}
