using System.Diagnostics;
using System.Runtime.Serialization;

namespace Editor.GameProject
{
    [DataContract]
    public class Scene : ViewModelBase
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
        public GameProject Project { get; private set; }

        private bool _isActive;
        [DataMember]
        public bool IsActive
        {
            get => _isActive;
            set
            {
                if(_isActive != value)
                {
                    _isActive = value;
                    OnPropertyChanged(nameof(IsActive));
                }
            }
        }

        public Scene(GameProject project, string name)
        {
            Debug.Assert(project != null);

            Project = project;
            Name = name;
        }
    }
}
