using Scripts.DataBase;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Scripts
{
    public class main
    {
        public static int Main()
        {
            TestCallRemote();

            //DataBase.Account account = new Account();
            //account.createAndInsertToDB;
            //List<string> arr = DBSerializer.getDBValues(account);
            //string tabname = account.tableName;
            //object obj = account.fetchByFieldQuery("user");

            //DataInitializer dier = new DataInitializer();
            //dier.Init();

            return 0;
        }
        public static void TestCallRemote()
        {
            S_Account sp = new S_Account();
            string sm = sp.callClient("onEnterWorld", 15, "ShanHaiGuan");
            Console.WriteLine(sm);
            RemoteFunction remotefun = RemoteFunction.parse(sm);
        }
    }
}
