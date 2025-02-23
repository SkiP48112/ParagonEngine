﻿using Editor.DLLWrapper;
using Editor.GameDevelopment;
using Editor.Utilities;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using System.Runtime.Serialization;
using System.Windows;
using System.Windows.Input;

namespace Editor.GameProject
{
    [DataContract(Name = "Game")]
    class Project : ViewModelBase
    {
        [DataMember] public string Name { get; private set; } = ProjectConsts.DEFAULT_PROJECT_NAME;
        [DataMember] public string Path { get; private set; }

        public string FullPath => $@"{Path}{Name}{ProjectConsts.PROJECT_EXTENSION}";
        public string Solution => $@"{Path}{Name}.sln";
        public static Project? CurrentGameProject => Application.Current.MainWindow.DataContext as Project;
        public static UndoRedoManager UndoRedoManager { get; } = new UndoRedoManager();

        private static readonly string[] _buildConfigurationNames = { "Debug", "Release", "DebugEditor", "ReleaseEditor" };

        public ICommand? AddSceneCommand { get; private set; }
        public ICommand? RemoveSceneCommand { get; private set; }
        public ICommand? UndoCommand { get; private set; }
        public ICommand? RedoCommand { get; private set; }
        public ICommand? SaveCommand { get; private set; }
        public ICommand? BuildCommand { get; private set; }

        private int _buildConfig;
        [DataMember]
        public int BuildConfig
        {
            get => _buildConfig;
            set
            {
                if(_buildConfig != value)
                {
                    _buildConfig = value;
                    OnPropertyChanged(nameof(BuildConfig));
                }
            }
        }

        public BuildConfiguration StandAloneBuildConfig => BuildConfig == 0 ? BuildConfiguration.Debug : BuildConfiguration.Release;
        public BuildConfiguration DllBuildConfig => BuildConfig == 0 ? BuildConfiguration.DebugEditor : BuildConfiguration.ReleaseEditor;

        private Scene? _activeScene;
        public Scene? ActiveScene
        {
            get => _activeScene;
            set
            {
                if (_activeScene != value)
                {
                    _activeScene = value;
                    OnPropertyChanged(nameof(ActiveScene));
                }
            }

        }

        [DataMember(Name = "Scenes")]
        private ObservableCollection<Scene> _scenes = new ObservableCollection<Scene>();
        public ReadOnlyObservableCollection<Scene>? Scenes
        {
            get;
            private set;
        }

        public Project(string name, string path)
        {
            Name = name;
            Path = path;


            OnDeserialized(new StreamingContext());
        }

        public static void AddNewUndoRedoAction(string name, Action undo, Action redo)
        {
            UndoRedoManager.Add(new UndoRedoAction(undo, redo, name));
        }

        public static Project? Load(string path)
        {
            Debug.Assert(File.Exists(path));
            return Serializer.FromFile<Project>(path);
        }

        public void Unload()
        {
            VisualStudio.CloseVisualStudio();
            UndoRedoManager.Reset();
        }

        public static void Save(Project project)
        {
            Serializer.ToFile(project, project.FullPath);
            Logger.Log(MessageType.Info, $"Project saved to {project.FullPath}");
        }

        private static string GetBuildConfigurationName(BuildConfiguration configuration)
        {
            return _buildConfigurationNames[(int)configuration];
        }

        private void AddScene(string name)
        {
            Debug.Assert(!string.IsNullOrEmpty(name.Trim()));
            _scenes.Add(new Scene(this, name));
        }

        private void RemoveScene(Scene scene)
        {
            Debug.Assert(_scenes.Contains(scene));
            _scenes.Remove(scene);
        }

        private RelayCommand<object> GetAddSceneCommand()
        {
            return new RelayCommand<object>(x =>
            {
                AddScene($"New Scene {_scenes.Count}");
                var newScene = _scenes.Last();
                var index = _scenes.Count - 1;
                AddNewUndoRedoAction($"Add {newScene.Name}", () => RemoveScene(newScene), () => _scenes.Insert(index, newScene));
            });
        }

        private RelayCommand<Scene> GetRemoveSceneCommand()
        {
            return new RelayCommand<Scene>(scene =>
            {
                Debug.Assert(scene != null, "Can't remove scene because it's null.");

                var sceneIndex = _scenes.IndexOf(scene);
                RemoveScene(scene);
                AddNewUndoRedoAction($"Remove {scene.Name}", () => _scenes.Insert(sceneIndex, scene), () => RemoveScene(scene));
            }, x => !x.IsActive);
        }

        private void BuildGameCodeDll(bool showWindow = true)
        {
            try
            {
                UnloadGameCodeDll();
                VisualStudio.BuildSolution(this, GetBuildConfigurationName(DllBuildConfig), showWindow);
                if (VisualStudio.IsBuildSucceeded)
                {
                    LoadGameCodeDll();
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
                Logger.Log(MessageType.Error, "Failed to build game code dll.");
                throw;
            }
            
        }

        private void LoadGameCodeDll()
        {
            var configName = GetBuildConfigurationName(DllBuildConfig);
            var dll = $@"{Path}x64\{configName}\{Name}.dll";
            if (File.Exists(dll) && EngineAPI.LoadGameCodeDll(dll) != 0)
            {
                Logger.Log(MessageType.Info, "Game code DLL loaded successfully.");
                return;
            }

            Logger.Log(MessageType.Warning, "Failed to load game code DLL file. Try to build the project first.");
        }

        private void UnloadGameCodeDll()
        {
            if (EngineAPI.UnloadGameCodeDll() != 0)
            {
                Logger.Log(MessageType.Info, "Game code DLL unloaded successfully.");
            }
        }

        [OnDeserialized]
        private void OnDeserialized(StreamingContext context)
        {
            if (_scenes != null)
            {
                Scenes = new ReadOnlyObservableCollection<Scene>(_scenes);
                OnPropertyChanged(nameof(Scenes));
            }

            Debug.Assert(Scenes != null);
            ActiveScene = Scenes.FirstOrDefault(x => x.IsActive);

            BuildGameCodeDll(false);

            AddSceneCommand = GetAddSceneCommand();
            RemoveSceneCommand = GetRemoveSceneCommand();

            UndoCommand = new RelayCommand<object>(x => UndoRedoManager.Undo(), x => UndoRedoManager.UndoList.Any());
            RedoCommand = new RelayCommand<object>(x => UndoRedoManager.Redo(), x => UndoRedoManager.RedoList.Any());
            SaveCommand = new RelayCommand<object>(x => Save(this));
            BuildCommand = new RelayCommand<bool>(x => BuildGameCodeDll(x), x => !VisualStudio.IsDebugging() && VisualStudio.IsBuildDone);
        }
    }
}