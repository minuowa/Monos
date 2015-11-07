#include "stdafx.h"
#include "SQLHelper.h"
#include <iosfwd>
#include <io.h>
std::string SQLHelper::create_table(const char* table, const char* values)
{
	//mysql> insert into yw_account values(12345);
	std::stringstream str;
	str << "create table " << table << " values(" << values << ")";
	return str.str();
}
