using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using Editor.Utilities;

namespace Editor.GameProject
{
    class GameProjectViewModel : ViewModelBase
    {
        // TODO: Get the path from the installation location
        private readonly string _templatesPath = GameProjectConsts.TemplatesPath;

        private string _projectName = GameProjectConsts.DefaultProjectName;
        public string ProjectName 
        {
            get => _projectName;
            set 
            { 
                if(_projectName != value)
                {
                    _projectName = value;
                    ValidateProjectPath();
                    OnPropertyChanged(nameof(ProjectName));
                }
            } 
        }

        private string _projectPath = $@"{Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments)}\{GameProjectConsts.ProjectsFolderName}\";
        public string ProjectPath
        {
            get => _projectPath;
            set
            {
                if (_projectPath != value)
                {
                    _projectPath = value;
                    ValidateProjectPath();
                    OnPropertyChanged(nameof(ProjectPath));
                }
            }
        }

        private bool _isValid;
        public bool IsValid
        {
            get => _isValid;
            set
            {
                if (_isValid != value)
                {
                    _isValid = value;
                    OnPropertyChanged(nameof(IsValid));
                }
            }
        }

        private string _errorMsg = string.Empty;
        public string ErrorMsg
        {
            get => _errorMsg;
            set
            {
                if (_errorMsg != value)
                {
                    _errorMsg = value;
                    OnPropertyChanged(nameof(ErrorMsg));
                }
            }
        }

        private ObservableCollection<GameProjectTemplate> _projectTemplates = new ObservableCollection<GameProjectTemplate>();
        public ReadOnlyObservableCollection<GameProjectTemplate> ProjectTemplates 
        { 
            get; 
        }

        public string CreateProject(GameProjectTemplate template)
        {
            ValidateProjectPath();
            if (!IsValid)
            {
                return string.Empty;
            }

            if (!Path.EndsInDirectorySeparator(ProjectPath))
            {
                ProjectPath += @"\";
            }

            var path = $@"{ProjectPath}{ProjectName}";
            try
            {
                if (!Directory.Exists(path))
                {
                    Directory.CreateDirectory(path);
                }

                foreach(var folder in template.Folders)
                {
                    Directory.CreateDirectory(Path.GetFullPath(Path.Combine(Path.GetDirectoryName(path), folder)));
                }

                var dirInfo = new DirectoryInfo(path + @".Paragon\");
                dirInfo.Attributes |= FileAttributes.Hidden;
                File.Copy(template.IconPath, Path.GetFullPath(Path.Combine(dirInfo.FullName, GameProjectConsts.iconFileName)));
                File.Copy(template.ScreenshotPath, Path.GetFullPath(Path.Combine(dirInfo.FullName, GameProjectConsts.screenshotFileName)));


            }
            catch (Exception e)
            {
                // TODO: Log errors in editor logger
                Debug.WriteLine(e.Message);
            }
        }
        
        public GameProjectViewModel()
        {
            ProjectTemplates = new ReadOnlyObservableCollection<GameProjectTemplate>(_projectTemplates);

            try
            {
                var templates = Directory.GetFiles(_templatesPath, GameProjectConsts.templateFileName, SearchOption.AllDirectories);
                Debug.Assert(templates.Any());

                foreach (var file in templates)
                {
                    var fileDirectoryPath = Path.GetDirectoryName(file);
                    Debug.Assert(fileDirectoryPath != null);
                    
                    var template = Serializer.FromFile<GameProjectTemplate>(file);
                    template.IconPath = Path.GetFullPath(Path.Combine(fileDirectoryPath, GameProjectConsts.iconFileName));
                    template.Icon = File.ReadAllBytes(template.IconPath);

                    template.ScreenshotPath = Path.GetFullPath(Path.Combine(fileDirectoryPath, GameProjectConsts.screenshotFileName));
                    template.Screenshot = File.ReadAllBytes(template.ScreenshotPath);

                    template.ProjectPath = Path.GetFullPath(Path.Combine(fileDirectoryPath, template.File));
                    template.Screenshot = File.ReadAllBytes(template.ScreenshotPath);

                    _projectTemplates.Add(template);
                }

                ValidateProjectPath();
            }
            catch (Exception e)
            {
                // TODO: Log errors in editor logger
                Debug.WriteLine(e.Message);
            }
        }

        private bool ValidateProjectPath()
        {
            var path = ProjectPath;
            if (!Path.EndsInDirectorySeparator(path))
            {
                path += @"\";
            }

            path += $@"{ProjectName}\";

            IsValid = false;
            if (string.IsNullOrWhiteSpace(ProjectName.Trim()))
            {
                ErrorMsg = "Type in a project name.";
            }
            else if (ProjectName.IndexOfAny(Path.GetInvalidFileNameChars()) != -1)
            {
                ErrorMsg = "Invalid character(s) used in project name.";
            }
            else if (string.IsNullOrWhiteSpace(ProjectPath.Trim()))
            {
                ErrorMsg = "Type in a project path.";
            }
            else if (ProjectPath.IndexOfAny(Path.GetInvalidPathChars()) != -1)
            {
                ErrorMsg = "Invalid character(s) used in project path.";
            }
            else if(Directory.Exists(path) && Directory.EnumerateFileSystemEntries(path).Any())
            {
                ErrorMsg = "Selected project folder already exists and it's not empty.";
            }
            else
            {
                ErrorMsg = string.Empty;
                IsValid = true;
            }

            return IsValid;
        }
    }
}