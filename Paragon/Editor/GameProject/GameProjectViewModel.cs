using Editor.Utilities;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using System.Runtime.Serialization;
using System.Windows;

namespace Editor.GameProject
{
    [DataContract(Name = "Game")]
    public class GameProjectViewModel : ViewModelBase
    {
        [DataMember] public string Name { get; private set; } = GameProjectConsts.DEFAULT_PROJECT_NAME;
        [DataMember] public string Path { get; private set; }
        public string FullPath => $"{Path}{Name}{GameProjectConsts.PROJECT_EXTENSION}";
        public static GameProjectViewModel? CurrentGameProject => Application.Current.MainWindow.DataContext as GameProjectViewModel;

        private SceneViewModel _activeScene;
        public SceneViewModel ActiveScene
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
        private ObservableCollection<SceneViewModel> _scenes = new ObservableCollection<SceneViewModel>();
        public ReadOnlyObservableCollection<SceneViewModel> Scenes
        {
            get;
            private set;
        }

        public static GameProjectViewModel Load(string path)
        {
            Debug.Assert(File.Exists(path));
            return Serializer.FromFile<GameProjectViewModel>(path);
        }

        public void Unload()
        {

        }

        public static void Save(GameProjectViewModel project)
        {
            Serializer.ToFile(project, project.FullPath);
        }

        [OnDeserialized]
        private void OnDeserialized(StreamingContext context)
        {
            if(_scenes != null)
            {
                Scenes = new ReadOnlyObservableCollection<SceneViewModel>(_scenes);
                OnPropertyChanged(nameof(Scenes));
            }

            ActiveScene = Scenes.FirstOrDefault(x => x.IsActive);
        }

        public GameProjectViewModel(string name, string path)
        {
            Name = name;
            Path = path;

            OnDeserialized(new StreamingContext());
        }
    }
}