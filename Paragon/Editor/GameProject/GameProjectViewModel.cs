using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using System.Runtime.Serialization;
using Editor.Utilities;

namespace Editor.GameProject
{
    [DataContract]
    public class GameProjectTemplate
    {
        [DataMember] public string Type { get; set; }
        [DataMember] public string File { get; set; }
        [DataMember] public List<string> Folders { get; set; }
        public byte[] Icon { get; set; }
        public byte[] Screenshot { get; set; }
        public string IconPath { get; set; }
        public string ScreenshotPath { get; set; }
        public string ProjectPath { get; set; }
    }

    class GameProjectViewModel : ViewModelBase
    {
        // TODO: Get the path from the installation location
        private readonly string _templatesPath = @"..\..\Editor\Templates";

        private string _projectName = "NewProject";
        public string ProjectName 
        {
            get => _projectName;
            set 
            { 
                if(_projectName != value)
                {
                    _projectName = value;
                    OnPropertyChanged(nameof(ProjectName));
                }
            } 
        }

        private string _projectPath = $@"{Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments)}\ParagonProjects\";
        public string ProjectPath
        {
            get => _projectPath;
            set
            {
                if (_projectPath != value)
                {
                    _projectPath = value;
                    OnPropertyChanged(nameof(ProjectPath));
                }
            }
        }

        private ObservableCollection<GameProjectTemplate> _projectTemplates = new ObservableCollection<GameProjectTemplate>();
        public ReadOnlyObservableCollection<GameProjectTemplate> ProjectTemplates 
        { 
            get; 
        }

        public GameProjectViewModel()
        {
            ProjectTemplates = new ReadOnlyObservableCollection<GameProjectTemplate>(_projectTemplates);

            try
            {
                var templates = Directory.GetFiles(_templatesPath, "template.xml", SearchOption.AllDirectories);
                Debug.Assert(templates.Any());

                foreach (var file in templates)
                {
                    var template = Serializer.FromFile<GameProjectTemplate>(file);
                    template.IconPath = Path.GetFullPath(Path.Combine(Path.GetDirectoryName(file), "Icon.png"));
                    template.Icon = File.ReadAllBytes(template.IconPath);

                    template.ScreenshotPath = Path.GetFullPath(Path.Combine(Path.GetDirectoryName(file), "Screenshot.png"));
                    template.Screenshot = File.ReadAllBytes(template.ScreenshotPath);

                    template.ProjectPath = Path.GetFullPath(Path.Combine(Path.GetDirectoryName(file), template.File));
                    template.Screenshot = File.ReadAllBytes(template.ScreenshotPath);

                    _projectTemplates.Add(template);
                }
            }
            catch (Exception e)
            {
                // TODO: Log errors in editor logger
                Debug.WriteLine(e.Message);
            }
        }
    }
}