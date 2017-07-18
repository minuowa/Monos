#pragma once
#include "Config.h"
#include "NetWork.h"
#include "DataBase.h"
#include "World.h"

class CX_LIB App {
  public:
    enum APP_TYPE {
        APP_TYPE_LOGIN,
        APP_TYPE_CENTER,
        APP_TYPE_BASE,
    };
  public:

    static Config Config;
    static DataBase DataBase;
    static NetWork	Net;
    static World World;

  public:
    App(int narg, const char** args);
    virtual ~App();

    virtual bool initialize();

    void run();

    void finish();

    void archive();

  protected:

    bool InitNet();

    bool InitDataBase();

    bool InitScript();

  private:
    APP_TYPE mType;
    int mServerID;
    uCommandLine mCommandLine;
};

