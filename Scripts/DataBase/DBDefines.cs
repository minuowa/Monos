using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DataBase
{
    public enum CommonState
    {
        None,
        CanFetch,
        Fetched,
    }

    [DB(DBType.Table)]
    public class Account : DBPkg
    {
        [DB]
        public string user = "123";

        [DB]
        public string pwd;

        [DB]
        public List<string> roles;

        [DB]
        public int test = 5;

        [DB]
        public string active_role;
    }
    [DB(DBType.Table)]
    public class Role : DBPkg
    {
        [DB]
        public string name;

        [DB]
        public bool sex;

        [DB]
        public uint lastLoginTime;

        [DB]
        public uint sceneid;

        [DB]
        public Sign sign;

        [DB]
        public List<string> soldiers;
    }
    public class Sign
    {
        [DB]
        public int count;

        [DB]
        public List<CommonState> states;
    }

    [DB(DBType.Table)]
    public class Soldier : DBPkg
    {
        [DB]
        public int type = 0;

        [DB]
        public byte star = 1;

        [DB]
        public string name;

        [DB]
        public uint exp = 0;

        [DB]
        public uint general = 0;
    }
}
