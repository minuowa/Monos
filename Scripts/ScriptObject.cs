using Scripts.DataBase;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace Scripts
{
    public class ScriptObject
    {
        protected DataBase.DBInterface mDBInterface;
        public string guid()
        {
            return "xfjcdej";
        }
        public string callClient(string method, params object[] args)
        {
            RemoteFunction remotefun = new RemoteFunction();
            remotefun.method = method;
            remotefun.args = args;
            remotefun.guid = guid();
            remotefun.type = GetType().Name;
            return remotefun.serialize();
        }
        public void sendDB(DBInterface data)
        {

        }
        public void sendToClient()
        {

        }
    }
}
