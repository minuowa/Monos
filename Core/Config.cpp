#include "stdafx.h"
#include "Config.h"


Config::Config()
{
}


Config::~Config()
{
}

bool Config::initialize(const char* file)
{
	xml_load(file);
	xml_get_node("App")
	{
		uString strType;
		xml_get_attr("type", strType);

		CommonConfig* cfg = nullptr;

		if (strType == "login")
		{
			cfg = &loginApp;
		}

		if (cfg)
		{
			xml_get_node("net")
			{
				xml_get_attr("ip", cfg->net.ip);
				xml_get_attr("port", cfg->net.port);
				xml_get_attr("threadCount", cfg->net.threadCount);
			}

			xml_get_node("db")
			{
				xml_get_attr("dbname", cfg->db.dbName);
				xml_get_attr("user", cfg->db.user);
				xml_get_attr("passwrod", cfg->db.password); 
				xml_get_attr("host", cfg->db.host);
				xml_get_attr("port", cfg->db.port); 
				xml_get_attr("autoCreateAccount", cfg->db.autoCreateAccount);
			}
		}
	}
	return true;
}
