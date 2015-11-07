using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace Scripts
{
    namespace DataBase
    {
        public class DBInterface
        {
            public string tableName
            {
                get
                {
                    if(DBSerializer.isTable(GetType()))
                            return ("yw_" + GetType().Name).ToLower();
                    return string.Empty;
                }
            }
            public bool fetchByFieldQuery(string field)
            {
                FieldInfo fi = GetType().GetField(field);

                object ofield = null;
                if (fi != null)
                    ofield = fi.GetValue(this);

                if (ofield != null)
                {
                    string cmd = string.Format("SELECT * FROM {0} WHERE {1}={2};", tableName, field, ofield.ToString());
                    return DataInitializer.DB_queryToScriptObject(cmd, this);
                }

                return false;
            }
            public void fetch()
            {

            }
            public void save()
            {

            }
            public void remove()
            {

            }
            public void send()
            {

            }

            public void fill(byte[] buffer)
            {

            }
            public bool saveToDB()
            {
                return true;
            }
            
            public bool createAndInsertToDB()
            {
                //insert into yw_account(guid) values('8902');
                string cmdstring = "insert into " + tableName + " (";
                string values = "values(";

                Type tp = GetType();

                List<string> columns = DBSerializer.getDBColoumns(tp);
                List<string> defines = DBSerializer.getDBDefines(tp);

                for (int i = 0; i < columns.Count; ++i)
                {
                    cmdstring += columns[i] + ",";
                    FieldInfo field = tp.GetField(columns[i]);
                    values += "'"+DBSerializer.serialize(field, this) + "',";
                }

                cmdstring = cmdstring.Remove(cmdstring.Length - 1);
                values = values.Remove(values.Length - 1);
                cmdstring = cmdstring + ") " + values + ");";

                return DataInitializer.DB_insert(tableName, cmdstring);
            }
 
        }
    }
}
