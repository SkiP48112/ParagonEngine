using System.Diagnostics;
using System.Runtime.Serialization;

namespace Editor.GameProject
{
    [DataContract]
    public class SceneViewModel : ViewModelBase
    {
        private string _name = string.Empty;
        
        [DataMember]
        public string Name 
        {
            get => _name;
            set
            {
                if (_name != value)
                {
                    _name = value;
                    OnPropertyChanged(nameof(Name));
                }
            }
        }

        [DataMember]
        public GameProjectViewModel Project { get; private set; }

        public SceneViewModel(GameProjectViewModel project, string name)
        {
            Debug.Assert(project != null);

            Project = project;
            Name = name;
        }
    }
}
