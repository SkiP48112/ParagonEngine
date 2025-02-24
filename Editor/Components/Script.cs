using System.Runtime.Serialization;

namespace Editor.Components
{
    [DataContract]
    class Script : Component
    {
        private string? _name = string.Empty;
        [DataMember]
        public string? Name
        {
            get => _name;
            set
            {
                if(_name != value) 
                {
                    _name = value;
                    OnPropertyChanged(nameof(Name));
                }
            }
        }

        public Script(GameEntity owner) : base(owner)
        {
        }

        public override IMSComponent GetMultiselectionComponent(MSEntity msEntity)
        {
            return new MSCScript(msEntity);
        }
    }
}
