#include "stdafx.h"
#include "ConfigInfo.h"

CommonConfig::CommonConfig()
{

}

NetConfig::NetConfig() :ip("127.0.0.1")
, port(9631)
, threadCount(4)
{

}

