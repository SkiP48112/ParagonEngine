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
    public class GameProject : ViewModelBase
    {
        [DataMember] public string Name { get; private set; } = GameProjectConsts.DEFAULT_PROJECT_NAME;
        [DataMember] public string Path { get; private set; }
        public string FullPath => $"{Path}{Name}{GameProjectConsts.PROJECT_EXTENSION}";
        public static GameProject? CurrentGameProject => Application.Current.MainWindow.DataContext as GameProject;
        public static UndoRedoManager UndoRedoManager { get; } = new UndoRedoManager();

        public ICommand AddScene { get; private set; }
        public ICommand RemoveScene { get; private set; }
        public ICommand Undo { get; private set; }
        public ICommand Redo { get; private set; }

        private Scene _activeScene;
        public Scene ActiveScene
        {
            get => _activeScene;
            set
            {
                if(_activeScene != value)
                {
                    _activeScene = value;
                    OnPropertyChanged(nameof(ActiveScene));
                }
            }

        }

        [DataMember(Name = "Scenes")] 
        private ObservableCollection<Scene> _scenes = new ObservableCollection<Scene>();
        public ReadOnlyObservableCollection<Scene> Scenes
        {
            get;
            private set;
        }

        public GameProject(string name, string path)
        {
            Name = name;
            Path = path;


            OnDeserialized(new StreamingContext());
        }

        private void AddSceneInternal(string name)
        {
            Debug.Assert(!string.IsNullOrEmpty(name.Trim()));
            _scenes.Add(new Scene(this, name));
        }

        private void RemoveSceneInternal(Scene scene)
        {
            Debug.Assert(_scenes.Contains(scene));
            _scenes.Remove(scene);
        }

        public static GameProject Load(string path)
        {
            Debug.Assert(File.Exists(path));
            return Serializer.FromFile<GameProject>(path);
        }

        public void Unload()
        {

        }

        public static void Save(GameProject project)
        {
            Serializer.ToFile(project, project.FullPath);
        }

        [OnDeserialized]
        private void OnDeserialized(StreamingContext context)
        {
            if(_scenes != null)
            {
                Scenes = new ReadOnlyObservableCollection<Scene>(_scenes);
                OnPropertyChanged(nameof(Scenes));
            }

            ActiveScene = Scenes.FirstOrDefault(x => x.IsActive);

            AddScene = new RelayCommand<object>(x =>
            {
                AddSceneInternal($"New Scene {_scenes.Count}");
                var newScene = _scenes.Last();
                var index = _scenes.Count - 1;

                UndoRedoManager.Add(new UndoRedoAction(
                    () => RemoveSceneInternal(newScene),
                    () => _scenes.Insert(index, newScene),
                    $"Add {newScene.Name}"
                    ));
            });

            RemoveScene = new RelayCommand<Scene>(x =>
            {
                var sceneIndex = _scenes.IndexOf(x);
                RemoveSceneInternal(x);

                UndoRedoManager.Add(new UndoRedoAction(
                    () => _scenes.Insert(sceneIndex, x),
                    () => RemoveSceneInternal(x),
                    $"Remove {x.Name}"
                    ));
            }, x => !x.IsActive);

            Undo = new RelayCommand<object>(x => UndoRedoManager.Undo());
            Redo = new RelayCommand<object>(x => UndoRedoManager.Redo());
        }
    }
}