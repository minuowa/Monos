#pragma once
#include "BaseObject.h"
class CX_LIB DBObject :
    public BaseObject {
  public:
    DBObject();
    ~DBObject();

    bool fetchByField(const char* field);

    void sync(string data);

    bool fetch();

    bool saveToDB();

    bool createAndInsertToDB();

    void requireGUID();

    void updateGUIDByScript();

    virtual void setGuid(const char* guid) override;

};

