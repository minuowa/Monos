using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Mono.Debugging.Client;
using Mono.Debugger.Soft;
using Mono.Debugging.Soft;
using System.Diagnostics;
using System.Threading;

namespace Microsoft.Samples.VisualStudio.MDebugger
{
    public class Executer : DebuggerInterface
    {
        public string attachProcessName;

        public static string PROCESS_NAME_PREFIX = "game.";

        ManualResetEvent mDoneEvent = new ManualResetEvent(false);
        DebuggerSession mDebuggerSession;
        Mono.Debugging.Client.StackFrame frame;
        public void Setup()
        {
            mDebuggerSession = Start(string.Empty);
            frame = mDebuggerSession.ActiveThread.Backtrace.GetFrame(0);
        }
        public Executer()
        {
            mDebuggerSession = CreateDebuggerSession();
            mDebuggerSession.OutputWriter = WriteCallBack;
            mDebuggerSession.TargetStopped += MDebuggerSession_TargetStopped;
        }

        void WriteCallBack(bool isStderr, string text)
        {
            Console.WriteLine("PROC:" + text);
        }

        private void MDebuggerSession_TargetStopped(object sender, TargetEventArgs e)
        {
            mDoneEvent.Set();
        }

        public void TearDown()
        {
            mDebuggerSession.Exit();
            mDebuggerSession.Dispose();
        }

        public void Attach()
        {
            ProcessInfo pi = GetCurPorcess();
            if (pi == null)
                return;

            DebuggerSessionOptions ops = new DebuggerSessionOptions();

            ops.EvaluationOptions = EvaluationOptions.DefaultOptions;
            ops.EvaluationOptions.EvaluationTimeout = 100000;

            mDebuggerSession.AttachToProcess(pi, ops);

            //Debugger.Launch();
        }

        ProcessInfo GetCurPorcess()
        {
            if (string.IsNullOrEmpty(attachProcessName))
                return null;

            Process[] myProcesses = Process.GetProcesses();

            foreach (var p in myProcesses)
            {
                if (p.ProcessName.StartsWith(PROCESS_NAME_PREFIX))
                {
                    if (p.ProcessName == attachProcessName)
                    {
                        ProcessInfo pi = new ProcessInfo(p.Id, p.ProcessName);
                        return pi;
                    }
                }
            }
            return null;
        }

        public static string[] GetMonoProcessNames()
        {
            List<string> processNames = new List<string>();

            Process[] myProcesses = Process.GetProcesses();

            foreach (var p in myProcesses)
            {
                if (p.ProcessName.StartsWith(PROCESS_NAME_PREFIX))
                {
                    processNames.Add(p.ProcessName);
                }
            }
            if (processNames.Count > 0)
            {
                string[] arr = new string[processNames.Count];

                for (int i = 0; i < processNames.Count; ++i)
                {
                    arr[i] = processNames[i];
                }
                return arr;
            }
            return null;
        }
    }
}
