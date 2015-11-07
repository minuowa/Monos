using Scripts.DataBase;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace Scripts
{
    public class ReturnValue
    {
        public List<string> coloumns;
        public List<string> values;
    }
    public class DBSerializer
    {
        public static char ListSeparation = ';';
        public static DBType getDBType(CustomAttributeData attrData)
        {
            DBType dbtp = DBType.Defalut;

            if (attrData.AttributeType == typeof(DB))
            {
                int len = attrData.ConstructorArguments.Count;
                if (len > 0)
                    dbtp = (DBType)attrData.ConstructorArguments[0].Value;
            }
            return dbtp;
        }
        public static bool isTable(Type type)
        {
            if (type.GetCustomAttributesData().Count > 0)
            {
                foreach (var attr in type.CustomAttributes)
                {
                    if (attr.AttributeType == typeof(DB))
                    {
                        DBType dbtp = getDBType(attr);
                        if (dbtp == DBType.Table)
                            return true;
                    }
                }
            }
            return false;
        }
        public static List<string> getDBColoumns(Type type)
        {
            if (!isTable(type))
                return null;
            List<string> strarray = new List<string>();

            FieldInfo[] fields = type.GetFields();
            foreach (FieldInfo field in fields)
            {
                int n = field.GetCustomAttributesData().Count;
                if (n > 0)
                {
                    CustomAttributeData attrdata = field.GetCustomAttributesData()[0];
                    if (getDBType(attrdata) == DBType.Defalut)
                        strarray.Add(field.Name);
                }
            }
            return strarray;
        }
        public static string serialize(object obj)
        {
            if (obj == null)
                return string.Empty;

            Type proptype = obj.GetType();

            if (proptype.IsGenericType && proptype.GetGenericTypeDefinition() == typeof(List<>))
            {
                Type listParamType = proptype.GetGenericArguments()[0];
                if (obj == null)
                    return string.Empty;

                int cnt = (int)proptype.GetProperty("Count").GetValue(obj, null);

                StringBuilder sb = new StringBuilder();

                for (int i = 0; i < cnt; ++i)
                {
                    object[] args = new object[1];
                    args[0] = i;

                    PropertyInfo propinfo = proptype.GetProperty("Item");

                    object item = propinfo.GetValue(obj, args);
                    string itemstr = Data.Converter.GetString(item.GetType(), item);
                    if (itemstr.Length > 0)
                    {
                        sb.Append(itemstr);
                        sb.Append(ListSeparation);
                    }
                }
                return sb.ToString();
            }
            else
            {
                return Data.Converter.GetString(proptype, obj);
            }
        }
        public static string serialize(FieldInfo prop, object obj)
        {
            object childValue = prop.GetValue(obj);
            return serialize(childValue);
        }
        public static List<string> getDBValues(object obj)
        {
            Type type = obj.GetType();
            if (!isTable(type))
                return null;
            List<string> strarray = getDBColoumns(type);
            List<string> valueArray = new List<string>();

            foreach (string fieldname in strarray)
            {
                FieldInfo field = type.GetField(fieldname);
                if (field != null)
                    valueArray.Add(serialize(field, obj));
            }

            return valueArray;
        }
        public static List<string> getDBDefines(Type type)
        {
            List<string> strarray = getDBColoumns(type);
            List<string> valueArray = new List<string>();

            foreach (string fieldname in strarray)
            {
                FieldInfo field = type.GetField(fieldname);
                if (field != null)
                    valueArray.Add(DBDefiner.Instance.GetCmd(field));
            }
            return valueArray;
        }
    }
}
