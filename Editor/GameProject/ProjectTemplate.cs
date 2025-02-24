using System.Runtime.Serialization;

namespace Editor.GameProject
{
    [DataContract]
    public class ProjectTemplate
    {
        [DataMember] public string? Type { get; set; }
        [DataMember] public string? File { get; set; }
        [DataMember] public List<string>? Folders { get; set; }

        public byte[]? Icon { get; set; }
        public byte[]? Screenshot { get; set; }
        public string? IconPath { get; set; }
        public string? ScreenshotPath { get; set; }
        public string? ProjectPath { get; set; }
        public string? TemplatePath { get; set; }
    }
}