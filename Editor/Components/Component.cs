using System.Diagnostics;
using System.IO;
using System.Runtime.Serialization;

namespace Editor.Components
{
    [DataContract]
    abstract class Component : ViewModelBase
    {
        [DataMember]
        public GameEntity Owner { get; private set; }

        public Component(GameEntity owner)
        {
            Debug.Assert(owner != null);
            Owner = owner;
        }

        public abstract IMSComponent GetMultiselectionComponent(MSEntity msEntity);
        public abstract void WriteToBinary(BinaryWriter bw);
    }
}