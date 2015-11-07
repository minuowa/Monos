using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace Scripts
{
    public class RemoteFunction
    {
        public string method;
        public string guid;
        public string type;
        public object[] args;
        public string serialize()
        {
            XmlDocument doc = new XmlDocument();
            XmlElement root = doc.CreateElement("callClient");
            doc.AppendChild(root);

            root.SetAttribute("name", method);
            root.SetAttribute("guid", guid);
            root.SetAttribute("type", GetType().Name);
            XmlElement arg = doc.CreateElement("args");
            root.AppendChild(arg);
            foreach (var obj in args)
            {
                XmlElement argnode = doc.CreateElement("arg");
                arg.AppendChild(argnode);
                argnode.SetAttribute("type", obj.GetType().Name);
                argnode.SetAttribute("value", DBSerializer.serialize(obj));
            }
            MemoryStream ms = new MemoryStream(1024);
            doc.Save(ms);
            return Encoding.UTF8.GetString(ms.ToArray());
        }
        public static RemoteFunction parse(string source)
        {
            RemoteFunction remotefun = new RemoteFunction();

            MemoryStream ms = new MemoryStream(Encoding.UTF8.GetBytes(source));
            XmlDocument doc = new XmlDocument();
            doc.Load(ms);

            XmlNode root = doc.FirstChild;

            remotefun.guid = root.Attributes["guid"].Value;
            remotefun.method = root.Attributes["name"].Value;
            remotefun.type = root.Attributes["type"].Value;

            return remotefun;
        }
    }
}
