using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Mono.Debugging.Client;
using Mono.Debugger.Soft;
using Mono.Debugging.Soft;
using Mono.Debugging.Evaluation;
using System.IO;
using System.Threading;

namespace Microsoft.Samples.VisualStudio.MDebugger
{
    public class DebuggerInterface
    {
        public Mono.Debugger.Soft.VirtualMachineManager mVMMgr;

        public DebuggerSession CreateDebuggerSession()
        {
            return new SoftDebuggerSession();
        }

        public DebuggerStartInfo CreateDebuggerStartInfo()
        {
            return new DebuggerStartInfo();
        }

        protected DebuggerSession Start(string args)
        {
            try
            {
                DebuggerStartInfo si = CreateDebuggerStartInfo();
                si.Command = Path.Combine(Path.GetDirectoryName(GetType().Assembly.Location), "UnitTests.TestApp.exe");
                si.Arguments = args;

                DebuggerSessionOptions ops = new DebuggerSessionOptions();
                ops.EvaluationOptions = EvaluationOptions.DefaultOptions;
                ops.EvaluationOptions.EvaluationTimeout = 100000;

                DebuggerSession session = CreateDebuggerSession();
                string path = Path.Combine(GetTestsDir(), "UnitTests.TestApp", "Main.cs");
                path = Path.GetFullPath(path);

                ManualResetEvent done = new ManualResetEvent(false);

                session.OutputWriter = delegate (bool isStderr, string text)
                {
                    Console.WriteLine("PROC:" + text);
                };

                session.TargetStopped += delegate
                {
                    done.Set();
                };

                session.Run(si, ops);
                if (!done.WaitOne(3000))
                    throw new Exception("Timeout while waiting for initial breakpoint");

                return session;
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex);
                throw;
            }
        }

        public static string GetTestsDir()
        {
            var p = typeof(DebuggerInterface).Assembly.Location;
            return Path.Combine(Path.GetDirectoryName(p), "..", "..", "..");
        }

        public void AttachTo()
        {

        }
    }
}
