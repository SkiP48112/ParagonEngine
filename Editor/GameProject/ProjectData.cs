using System.Runtime.Serialization;

namespace Editor.GameProject
{
    [DataContract]
    public class ProjectData
    {
        [DataMember] public string? Name { get; set; }
        [DataMember] public string? Path { get; set; }
        [DataMember] public DateTime Date { get; set; }

        public string FullPath { get => $"{Path}{Name}{ProjectConsts.PROJECT_EXTENSION}"; }
        public byte[]? Icon { get; set; }
        public byte[]? Screenshot { get; set; }
    }
}
