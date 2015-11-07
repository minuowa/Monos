using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Scripts
{
    public enum DBType
    {
        Defalut,
        Table,
        IntArray,   //bulk
    }
    public class DB:Attribute
    {
        public DB(DBType type = DBType.Defalut, string def = null)
        {
            mDefine = def;
            mType = type;
        }
        protected string mDefine;
        protected DBType mType = DBType.Defalut;
    }
}
