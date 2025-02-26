using System.IO;
using System.Runtime.Serialization;
using System.Text;

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

        public override void WriteToBinary(BinaryWriter bw)
        {
            var nameBytes = Encoding.UTF8.GetBytes(Name!);
            bw.Write(nameBytes.Length);
            bw.Write(nameBytes);
        }
    }
}
