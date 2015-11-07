using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Scripts
{
    namespace DataBase
    {
        public enum CommonState
        {
            None,
            CanFetch,
            Fetched,
        }

        [DB(DBType.Table)]
        public class Account : DBInterface
        {
            [DB]
            public string guid;

            [DB]
            public string user = "123";

            [DB]
            public string pwd;

            [DB]
            public string[] roles;

            [DB]
            public int test = 5;
            public Account()
            {
            }
        }
        [DB(DBType.Table)]
        public class Role : DBInterface
        {
            [DB]
            public string guid;

            [DB]
            public uint lastLoginTime;

            [DB]
            public Sign sign;

            [DB]
            public int[] soldiers;
        }
        public class Sign
        {
            [DB]
            public int count;

            [DB]
            public CommonState[] states;
        }

        [DB(DBType.Table)]
        public class Soldier : DBInterface
        {
            [DB]
            public string guid;

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
}
