using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

//
namespace Scripts
{
    public class S_Account : ScriptObject
    {
        public S_Account()
        {
            Console.WriteLine("S_Account ctor ...");
        }

        public void enterWorld()
        {
            sendDB(mDBInterface);
            sendToClient();
        }
        public void enterScene(int sceneid)
        {
            //Console.WriteLine("Scene:" + sceneid.ToString());
        }

        public void callClient(string method, string args)
        {

        }
    }
}
