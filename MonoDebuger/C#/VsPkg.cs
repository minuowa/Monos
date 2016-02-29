/***************************************************************************
 
Copyright (c) Microsoft Corporation. All rights reserved.
THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.

***************************************************************************/

using EnvDTE;
using Microsoft.VisualStudio;
using Microsoft.VisualStudio.Shell;
using Microsoft.VisualStudio.Shell.Interop;
using System;
using System.Collections.Generic;
using System.ComponentModel.Design;
using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Globalization;
using System.Runtime.InteropServices;
using VSLangProj;

[assembly: SuppressMessage("Microsoft.Design", "CA1020:AvoidNamespacesWithFewTypes", Scope = "namespace", Target = "Microsoft.Samples.VisualStudio.MDebugger")]
namespace Microsoft.Samples.VisualStudio.MDebugger
{
    /// <summary>
    /// This is the class that implements the package exposed by this assembly.
    ///
    /// The minimum requirement for a class to be considered a valid package for Visual Studio
    /// is to implement the IVsPackage interface and register itself with the shell.
    /// This package uses the helper classes defined inside the Managed Package Framework (MPF)
    /// to do it: it derives from the Package class that provides the implementation of the 
    /// IVsPackage interface and uses the registration attributes defined in the framework to 
    /// register itself and its components with the shell.
    /// </summary>

    // This attribute tells the registration utility (regpkg.exe) that this class needs
    // to be registered as package.
    [PackageRegistration(UseManagedResourcesOnly = true)]

    // This attribute is used to register the informations needed to show the this package
    // in the Help/About dialog of Visual Studio.
    [InstalledProductRegistration("#100", "#102", "1.0", IconResourceID = 400)]

    // This attribute is needed to let the shell know that this package exposes VS commands (menus, buttons, etc...)
    [ProvideMenuResource("Menus.ctmenu", 1)]

    // This attribute registers a tool window exposed by this package.
    [Guid(GuidList.guidComboBoxPkgString)]
    public sealed class MDPackage : Package
    {
        /// <summary>
        /// Default constructor of the package.
        /// Inside this method you can place any initialization code that does not require 
        /// any Visual Studio service because at this point the package object is created but 
        /// not sited yet inside Visual Studio environment. The place to do all the other 
        /// initialization is the Initialize method.
        /// </summary>
        public MDPackage()
        {
        }

        /////////////////////////////////////////////////////////////////////////////
        // Overriden Package Implementation
        #region Package Members

        Executer mExecuter;

        EnvDTE.DTE mDte;

        /// <summary>
        /// Initialization of the package; this method is called right after the package is sited, so this is the place
        /// where you can put all the initilaization code that rely on services provided by VisualStudio.
        /// </summary>
        protected override void Initialize()
        {
            base.Initialize();

            mDte = GetService(typeof(EnvDTE.DTE)) as EnvDTE.DTE;

            mExecuter = new Executer();

            OleMenuCommandService mcs = GetService(typeof(IMenuCommandService)) as OleMenuCommandService;
            if (null != mcs)
            {
                CommandID menuMyDynamicComboCommandID = new CommandID(GuidList.guidComboBoxCmdSet, (int)PkgCmdIDList.ctrlIDIP);
                OleMenuCommand menuMyDynamicComboCommand = new OleMenuCommand(new EventHandler(OnMenuIP), menuMyDynamicComboCommandID);
                mcs.AddCommand(menuMyDynamicComboCommand);

                CommandID menuMyDynamicComboGetListCommandID = new CommandID(GuidList.guidComboBoxCmdSet, (int)PkgCmdIDList.ctrlIDIPList);
                MenuCommand menuMyDynamicComboGetListCommand = new OleMenuCommand(new EventHandler(OnMenuIPGetList), menuMyDynamicComboGetListCommandID);
                mcs.AddCommand(menuMyDynamicComboGetListCommand);

                CommandID menuAttachTo = new CommandID(GuidList.guidAttach, (int)PkgCmdIDList.ctrlIDAttach);
                MenuCommand mcAttachTo = new OleMenuCommand(new EventHandler(OnAttachTo), menuAttachTo);
                mcs.AddCommand(mcAttachTo);
            }
        }

        #endregion

        #region Combo Box Commands

        private string[] dropDownComboChoices = { Resources.Apples, Resources.Oranges, Resources.Pears, Resources.Bananas };
        private string currentDropDownComboChoice = Resources.Apples;

        private void OnMenuMyDropDownCombo(object sender, EventArgs e)
        {
            OleMenuCmdEventArgs eventArgs = e as OleMenuCmdEventArgs;

            if (eventArgs != null)
            {
                string newChoice = eventArgs.InValue as string;
                IntPtr vOut = eventArgs.OutValue;

                if (vOut != IntPtr.Zero)
                {
                    // when vOut is non-NULL, the IDE is requesting the current value for the combo
                    Marshal.GetNativeVariantForObject(currentDropDownComboChoice, vOut);
                }

                else if (newChoice != null)
                {
                    // new value was selected or typed in
                    // see if it is one of our items
                    bool validInput = false;
                    int indexInput = -1;
                    for (indexInput = 0; indexInput < dropDownComboChoices.Length; indexInput++)
                    {
                        if (string.Compare(dropDownComboChoices[indexInput], newChoice, StringComparison.CurrentCultureIgnoreCase) == 0)
                        {
                            validInput = true;
                            break;
                        }
                    }

                    if (validInput)
                    {
                        currentDropDownComboChoice = dropDownComboChoices[indexInput];
                        ShowMessage(Resources.MyDropDownCombo, currentDropDownComboChoice);
                    }
                    else
                    {
                        throw (new ArgumentException(Resources.ParamNotValidStringInList)); // force an exception to be thrown
                    }
                }
            }
            else
            {
                // We should never get here; EventArgs are required.
                throw (new ArgumentException(Resources.EventArgsRequired)); // force an exception to be thrown
            }
        }

        private void OnMenuMyDropDownComboGetList(object sender, EventArgs e)
        {
            OleMenuCmdEventArgs eventArgs = e as OleMenuCmdEventArgs;

            if (eventArgs != null)
            {
                object inParam = eventArgs.InValue;
                IntPtr vOut = eventArgs.OutValue;

                if (inParam != null)
                {
                    throw (new ArgumentException(Resources.InParamIllegal)); // force an exception to be thrown
                }
                else if (vOut != IntPtr.Zero)
                {
                    Marshal.GetNativeVariantForObject(dropDownComboChoices, vOut);
                }
                else
                {
                    throw (new ArgumentException(Resources.OutParamRequired)); // force an exception to be thrown
                }
            }

        }

        private string[] indexComboChoices = { Resources.Lions, Resources.Tigers, Resources.Bears };
        private int currentIndexComboChoice = 0;

        private void OnMenuMyIndexCombo(object sender, EventArgs e)
        {
            if ((null == e) || (e == EventArgs.Empty))
            {
                // We should never get here; EventArgs are required.
                throw (new ArgumentException(Resources.EventArgsRequired)); // force an exception to be thrown
            }

            OleMenuCmdEventArgs eventArgs = e as OleMenuCmdEventArgs;

            if (eventArgs != null)
            {
                object input = eventArgs.InValue;
                IntPtr vOut = eventArgs.OutValue;

                if (vOut != IntPtr.Zero && input != null)
                {
                    throw (new ArgumentException(Resources.BothInOutParamsIllegal)); // force an exception to be thrown
                }
                if (vOut != IntPtr.Zero)
                {
                    // when vOut is non-NULL, the IDE is requesting the current value for the combo
                    Marshal.GetNativeVariantForObject(indexComboChoices[currentIndexComboChoice], vOut);
                }

                else if (input != null)
                {
                    int newChoice = -1;
                    if (!int.TryParse(input.ToString(), out newChoice))
                    {
                        // user typed a string argument in command window.
                        for (int i = 0; i < indexComboChoices.Length; i++)
                        {
                            if (string.Compare(indexComboChoices[i], input.ToString(), StringComparison.CurrentCultureIgnoreCase) == 0)
                            {
                                newChoice = i;
                                break;
                            }
                        }
                    }

                    // new value was selected or typed in
                    if (newChoice != -1)
                    {
                        currentIndexComboChoice = newChoice;
                        ShowMessage(Resources.MyIndexCombo, currentIndexComboChoice.ToString(CultureInfo.CurrentCulture));
                    }
                    else
                    {
                        throw (new ArgumentException(Resources.ParamMustBeValidIndexOrStringInList)); // force an exception to be thrown
                    }
                }
                else
                {
                    // We should never get here; EventArgs are required.
                    throw (new ArgumentException(Resources.EventArgsRequired)); // force an exception to be thrown
                }
            }
            else
            {
                // We should never get here; EventArgs are required.
                throw (new ArgumentException(Resources.EventArgsRequired)); // force an exception to be thrown
            }
        }

        private void OnMenuMyIndexComboGetList(object sender, EventArgs e)
        {
            if (e == EventArgs.Empty)
            {
                // We should never get here; EventArgs are required.
                throw (new ArgumentException(Resources.EventArgsRequired)); // force an exception to be thrown
            }

            OleMenuCmdEventArgs eventArgs = e as OleMenuCmdEventArgs;

            if (eventArgs != null)
            {
                object inParam = eventArgs.InValue;
                IntPtr vOut = eventArgs.OutValue;

                if (inParam != null)
                {
                    throw (new ArgumentException(Resources.InParamIllegal)); // force an exception to be thrown
                }
                else if (vOut != IntPtr.Zero)
                {
                    Marshal.GetNativeVariantForObject(indexComboChoices, vOut);
                }
                else
                {
                    throw (new ArgumentException(Resources.OutParamRequired)); // force an exception to be thrown
                }
            }
        }

        private double[] numericZoomLevels = { 4.0, 3.0, 2.0, 1.5, 1.25, 1.0, .75, .66, .50, .33, .25, .10 };
        private string zoomToFit = Resources.ZoomToFit;
        private string zoom_to_Fit = Resources.Zoom_to_Fit;
        private string[] zoomLevels = null;
        private NumberFormatInfo numberFormatInfo;
        private double currentZoomFactor = 1.0;

        bool EmptyArg(EventArgs e)
        {
            if ((null == e) || (e == EventArgs.Empty))
            {
                // We should never get here; EventArgs are required.
                throw (new ArgumentException(Resources.EventArgsRequired)); // force an exception to be thrown
            }
            return false;
        }

        private void OnMenuIP(object sender, EventArgs e)
        {
            if (EmptyArg(e))
                return;

            OleMenuCmdEventArgs eventArgs = e as OleMenuCmdEventArgs;

            if (eventArgs != null)
            {
                object input = eventArgs.InValue;
                IntPtr vOut = eventArgs.OutValue;

                // when vOut is non-NULL, the IDE is requesting the current value for the combo
                if (vOut != IntPtr.Zero)
                {
                    Marshal.GetNativeVariantForObject(mExecuter.attachProcessName, vOut);
                }
                else if (input != null)
                {
                    string inputString = input.ToString();
                    mExecuter.attachProcessName = inputString;
                }
            }
            else
            {
                // We should never get here; EventArgs are required.
                throw (new ArgumentException(Resources.EventArgsRequired)); // force an exception to be thrown
            }
        }
        private void OnAttachTo(object sender, EventArgs e)
        {
            EnvDTE.Project selectedProject = GetActiveProject();
            if (selectedProject == null)
                return;

            //if (0!= selectedProject.Kind.CompareTo(PrjKind.prjKindVSAProject))
            //    return;

            foreach(OutputGroup output in selectedProject.ConfigurationManager.ActiveConfiguration.OutputGroups)
            {
                Debug.WriteLine("{0} : {1}", output.FileNames, output.FileURLs);
            }

            //object[] objFileNames = selectedProject.ConfigurationManager.ActiveConfiguration.OutputGroups.Item(1).FileURLs as object[];

            //string fileName = objFileNames[0] as string;
            //System.Uri fileUri = new System.Uri(fileName);

            //string _filePath = fileUri.LocalPath;

            //var props = selectedProject.Properties;

            //foreach (Property p in props)
            //{
            //    try
            //    {
            //        Debug.WriteLine("{0} : {1}", p.Name, p.Value);
            //    }
            //    catch (Exception exc)
            //    {
            //        Debug.WriteLine(exc.Message);
            //    }
            //}
            mExecuter.Attach();
        }
        EnvDTE.Project GetActiveProject()
        {
            Array activeSolutionProjects = (Array)mDte.ActiveSolutionProjects;
            if (activeSolutionProjects != null && activeSolutionProjects.Length > 0)
            {
                return (EnvDTE.Project)activeSolutionProjects.GetValue(0);
            }
            return null;
        }
        private void OnMenuIPGetList(object sender, EventArgs e)
        {
            if ((null == e) || (e == EventArgs.Empty))
            {
                // We should never get here; EventArgs are required.
                throw (new ArgumentNullException(Resources.EventArgsRequired)); // force an exception to be thrown
            }

            OleMenuCmdEventArgs eventArgs = e as OleMenuCmdEventArgs;

            if (eventArgs != null)
            {
                object inParam = eventArgs.InValue;
                IntPtr vOut = eventArgs.OutValue;

                if (inParam != null)
                {
                    throw (new ArgumentException(Resources.InParamIllegal)); // force an exception to be thrown
                }
                else if (vOut != IntPtr.Zero)
                {
                    string[] names = Executer.GetMonoProcessNames();

                    if (names != null)
                        Marshal.GetNativeVariantForObject(names, vOut);
                }
                else
                {
                    throw (new ArgumentException(Resources.OutParamRequired)); // force an exception to be thrown
                }
            }
        }
        #endregion

        // Helper method to show a message box using the SVsUiShell/IVsUiShell service
        public void ShowMessage(string title, string message)
        {
            IVsUIShell uiShell = (IVsUIShell)GetService(typeof(SVsUIShell));
            Guid clsid = Guid.Empty;
            int result = VSConstants.S_OK;
            int hr = uiShell.ShowMessageBox(0,
                                ref clsid,
                                title,
                                message,
                                null,
                                0,
                                OLEMSGBUTTON.OLEMSGBUTTON_OK,
                                OLEMSGDEFBUTTON.OLEMSGDEFBUTTON_FIRST,
                                OLEMSGICON.OLEMSGICON_INFO,
                                0,        // false = application modal; true would make it system modal
                                out result);
            ErrorHandler.ThrowOnFailure(hr);
        }
    }
}