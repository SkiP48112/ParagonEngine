using System.Collections.ObjectModel;
using System.Runtime.Serialization;

namespace Editor.GameProject
{
    [DataContract(Name = "Game")]
    public class GameProjectViewModel : ViewModelBase
    {
        [DataMember] public string Name { get; private set; }
        [DataMember] public string Path { get; private set; }
        public string FullPath => $"{Path}{Name}{GameProjectConsts.PROJECT_EXTENSION}";

        [DataMember(Name = "Scenes")] 
        private ObservableCollection<SceneViewModel> _scenes = new ObservableCollection<SceneViewModel>();
        public ReadOnlyObservableCollection<SceneViewModel> Scenes
        {
            get;
        }

        public GameProjectViewModel(string name, string path)
        {
            Name = name;
            Path = path;

            _scenes.Add(new SceneViewModel(this, GameProjectConsts.DEFAULT_SCENE_NAME));
        }
    }
}