#include "stdafx.h"
#include "MySqlExecuter.h"



MySqlExecuter::MySqlExecuter() {
}


MySqlExecuter::~MySqlExecuter() {
}

bool MySqlExecuter::initialize(const DBConfig& config) {
    mConfig = config;

    mConnection = mysql_init(0);
    assert(mConnection != 0);

    MYSQL* tmp = mysql_real_connect(mConnection, mConfig.host.c_str(), mConfig.user.c_str(), mConfig.password.c_str(),
                                    mConfig.dbName.c_str(), mConfig.port, 0, 0);
    assert(tmp == mConnection);

    MYSQL_STMT* hstmt = mysql_stmt_init(mConnection);
    assert(hstmt != 0);

    return true;
}


void MySqlExecuter::queryBegin(const char* cmd) {
    assert(cmd);
    int res = mysql_real_query(mConnection, cmd, static_cast<unsigned long>(strlen(cmd)));
    printf("QUERY=>%s\n", cmd);
}



bool MySqlExecuter::queryEnd() {
    MYSQL_RES* ress = mysql_store_result(mConnection);

    if (ress) {
        MYSQL_ROW row = mysql_fetch_row(ress);

        if (row)
            return true;
    }

    return false;
}


bool MySqlExecuter::queryEnd(stringVector& result) {
    MYSQL_RES* ress = mysql_store_result(mConnection);
    if (ress) {
        assert(ress->row_count <= 1);

        MYSQL_ROW row = mysql_fetch_row(ress);

        if (row) {
            for (size_t i = 0; i < ress->field_count; ++i) {
                result.push_back(row[i]);
            }
        }
        return result.size() > 0;
    } else {
        return false;
    }

    return result.size() > 0;
}


bool MySqlExecuter::queryEnd(std::vector<shared_ptr<stringVector>>& result) {
    MYSQL_RES* ress = mysql_store_result(mConnection);

    if (ress) {
        for (size_t r = 0; r < ress->row_count; ++r) {
            shared_ptr<stringVector> vec(new stringVector());
            result.push_back(vec);

            MYSQL_ROW row = mysql_fetch_row(ress);

            if (row) {
                for (size_t i = 0; i < ress->field_count; ++i) {
                    vec->push_back(row[i]);
                }
            }
        }

        return result.size() > 0;
    }

    return result.size() > 0;
}

unsigned long MySqlExecuter::count() {
    MYSQL_RES* ress = mysql_store_result(mConnection);
    MYSQL_ROW row = mysql_fetch_row(ress);
    if (ress)
        return ::atoi(row[0]);
    return 0;
}

