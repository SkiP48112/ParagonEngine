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

        public string FullPath => $@"{Path}{Name}\{Name}{ProjectConsts.PROJECT_EXTENSION}";
        public static Project? CurrentGameProject => Application.Current.MainWindow.DataContext as Project;
        public static UndoRedoManager UndoRedoManager { get; } = new UndoRedoManager();

        public ICommand? AddSceneCommand { get; private set; }
        public ICommand? RemoveSceneCommand { get; private set; }
        public ICommand? UndoCommand { get; private set; }
        public ICommand? RedoCommand { get; private set; }
        public ICommand? SaveCommand { get; private set; }

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
            UndoRedoManager.Reset();
        }

        public static void Save(Project project)
        {
            Serializer.ToFile(project, project.FullPath);
            Logger.Log(MessageType.Info, $"Project saved to {project.FullPath}");
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

            AddSceneCommand = GetAddSceneCommand();
            RemoveSceneCommand = GetRemoveSceneCommand();

            UndoCommand = new RelayCommand<object>(x => UndoRedoManager.Undo());
            RedoCommand = new RelayCommand<object>(x => UndoRedoManager.Redo());
            SaveCommand = new RelayCommand<object>(x => Save(this));
        }
    }
}