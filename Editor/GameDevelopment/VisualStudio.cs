﻿using Editor.GameProject;
using Editor.Utilities;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.ComTypes;

namespace Editor.GameDevelopment
{
    static class VisualStudio
    {
        private static EnvDTE80.DTE2 _vsInstance = null;
        private static readonly string _progID = "VisualStudio.DTE.17.0";

        public static bool IsBuildSucceeded { get; private set; } = true;
        public static bool IsBuildDone { get; private set; } = true;

        [DllImport("ole32.dll")]
        private static extern int GetRunningObjectTable(uint reserved, out IRunningObjectTable pprot);

        [DllImport("ole32.dll")]
        private static extern int CreateBindCtx(uint reserved, out IBindCtx ppbc);

        public static void OpenVisualStudio(string solutionPath)
        {
            IRunningObjectTable rot = null;
            IEnumMoniker monikerTable = null;
            IBindCtx bindCtx = null;

            try
            {
                if (_vsInstance == null)
                {
                    // Find and open visual studio
                    var hResult = GetRunningObjectTable(0, out rot);
                    if(hResult < 0 || rot == null)
                    {
                        throw new COMException($"GetRunningObjectTable() returned HRESULT: {hResult:X8}");
                    }

                    rot.EnumRunning(out monikerTable);
                    monikerTable.Reset();

                    hResult = CreateBindCtx(0, out bindCtx);
                    if(hResult < 0 || bindCtx == null)
                    {
                        throw new COMException($"CreateBindCtx() returned HRESULT: {hResult:X8}");
                    }

                    IMoniker[] currentMoniker = new IMoniker[1];
                    while(monikerTable.Next(1, currentMoniker, IntPtr.Zero) == 0)
                    {
                        string name = string.Empty;
                        currentMoniker[0]?.GetDisplayName(bindCtx, null, out name);
                        if (name.Contains(_progID))
                        {
                            hResult = rot.GetObject(currentMoniker[0], out object obj);
                            if (hResult < 0 || obj == null)
                            {
                                throw new COMException($"Running object table's GetObject() returned HRESULT: {hResult:X8}");
                            }

                            EnvDTE80.DTE2 dte = obj as EnvDTE80.DTE2;
                            var solutionName = dte?.Solution.FullName;
                            if (solutionName == solutionPath)
                            {
                                _vsInstance = dte;
                                break;
                            }
                        }
                    }

                    if (_vsInstance == null)
                    {
                        Type visualStudioType = Type.GetTypeFromProgID(_progID, true);
                        Debug.Assert(visualStudioType != null);

                        _vsInstance = Activator.CreateInstance(visualStudioType) as EnvDTE80.DTE2;
                    }
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
                Logger.Log(MessageType.Error, "Failed to open Visual Studio");
            }
            finally
            {
                if(monikerTable != null)
                {
                    Marshal.ReleaseComObject(monikerTable);
                }

                if (rot != null)
                {
                    Marshal.ReleaseComObject(rot);
                }

                if (bindCtx != null)
                {
                    Marshal.ReleaseComObject(bindCtx);
                }
            }
        }

        public static void CloseVisualStudio()
        {
            if(_vsInstance?.Solution.IsOpen == true)
            {
                _vsInstance.ExecuteCommand("File.SaveAll");
                _vsInstance.Solution.Close(true);
            }

            _vsInstance?.Quit();
        }

        public static bool AddFilesToSolution(string solution, string projectName, string[] files)
        {
            Debug.Assert(files?.Length > 0);
            OpenVisualStudio(solution);

            try
            {
                if (_vsInstance != null)
                {
                    if (!_vsInstance.Solution.IsOpen)
                    {
                        _vsInstance.Solution.Open(solution);
                    }
                    else
                    {
                        _vsInstance.ExecuteCommand("File.SaveAll");
                    }

                    foreach (EnvDTE.Project project in _vsInstance.Solution.Projects)
                    {
                        if (!project.UniqueName.Contains(projectName))
                        {
                            continue;
                        }

                        foreach (var file in files)
                        {
                            project.ProjectItems.AddFromFile(file);
                        }
                    }

                    var cpp = files.FirstOrDefault(x => Path.GetExtension(x) == ".cpp");
                    if (!string.IsNullOrEmpty(cpp))
                    {
                        _vsInstance.ItemOperations.OpenFile(cpp, EnvDTE.Constants.vsViewKindTextView).Visible = true;
                    }

                    _vsInstance.MainWindow.Activate();
                    _vsInstance.MainWindow.Visible = true;
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
                Logger.Log(MessageType.Error, "Failed to add files to Visual Studio project");
                return false;
            }
            return true;
        }

        public static bool IsDebugging()
        {
            for(int i = 0; i < 3; ++i)
            {
                try
                {
                    return _vsInstance != null && (_vsInstance.Debugger.CurrentProgram != null || _vsInstance.Debugger.CurrentMode == EnvDTE.dbgDebugMode.dbgRunMode);
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex.Message);
                    Thread.Sleep(1000);
                }
            }

            return false;
        }

        public static void BuildSolution(Project project, string buildConfiguration, bool showWindow = true)
        {
            if (IsDebugging())
            {
                Logger.Log(MessageType.Error, "Visual Studio is currently running a process.");
                return;
            }

            OpenVisualStudio(project.Solution);
            IsBuildSucceeded = false;
            IsBuildDone = false;

            for(int i = 0; i < 3 && !IsBuildDone; ++i)
            {
                try
                {
                    if (!_vsInstance!.Solution.IsOpen)
                    {
                        _vsInstance.Solution.Open(project.Solution);
                    }

                    _vsInstance.MainWindow.Visible = showWindow;

                    _vsInstance.Events.BuildEvents.OnBuildProjConfigBegin += OnBuildSolutionBegin;
                    _vsInstance.Events.BuildEvents.OnBuildProjConfigDone += OnBuildSolutionDone;

                    try
                    {
                        foreach (var pdbFile in Directory.GetFiles(Path.Combine($"{project.Path}", $@"x64\{buildConfiguration}"), "*.pdb"))
                        {
                            File.Delete(pdbFile);
                        }
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex.Message);
                    }

                    _vsInstance.Solution.SolutionBuild.SolutionConfigurations.Item(buildConfiguration).Activate();
                    _vsInstance.ExecuteCommand("Build.BuildSolution");
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex.Message);
                    Debug.WriteLine($"Attempt {i}: failed to build {project.Name}");
                    Thread.Sleep(1000);
                }
            }
        }

        public static void RunGame(Project project, string configName, bool isDebug)
        {
            if(CanRunGame())
            {
                _vsInstance!.ExecuteCommand(isDebug ? "Debug.Start" : "Debug.StartWithoutDebugging");
            }
        }

        public static void StopGame()
        {
            if (CanStopGame())
            {
                _vsInstance!.ExecuteCommand("Debug.StopDebugging");
            }
        }

        private static bool CanRunGame()
        {
            return _vsInstance != null && !IsDebugging() && IsBuildDone && IsBuildSucceeded;
        }

        private static bool CanStopGame()
        {
            return _vsInstance != null && IsDebugging();
        }

        private static void OnBuildSolutionBegin(string project, string projectConfig, string platform, string solutionConfig)
        {
            _vsInstance!.Events.BuildEvents.OnBuildProjConfigBegin -= OnBuildSolutionBegin;
            Logger.Log(MessageType.Info, $"Building {project}, {projectConfig}, {platform}, {solutionConfig}.");
        }

        private static void OnBuildSolutionDone(string project, string projectConfig, string platform, string solutionConfig, bool success)
        {
            _vsInstance!.Events.BuildEvents.OnBuildProjConfigDone -= OnBuildSolutionDone;
            if (IsBuildDone)
            {
                return;
            }

            if (success)
            {
                Logger.Log(MessageType.Info, $"Building {projectConfig} configuration succeeded.");
            }
            else
            {
                Logger.Log(MessageType.Error, $"Building {projectConfig} configuration failed.");
            }

            IsBuildDone = true;
            IsBuildSucceeded = success;
        }
    }
}
