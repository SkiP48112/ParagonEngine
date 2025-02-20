using Editor.Utilities;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;

namespace Editor.GameProject
{
    class CreateProject : ViewModelBase
    {
        // TODO: Get the path from the installation location
        private readonly string _templatesPath = ProjectConsts.TEMPLATES_PATH;

        private string _projectName = ProjectConsts.DEFAULT_PROJECT_NAME;
        public string ProjectName
        {
            get => _projectName;
            set
            {
                if (_projectName != value)
                {
                    _projectName = value;
                    ValidateProjectPath();
                    OnPropertyChanged(nameof(ProjectName));
                }
            }
        }

        private string _projectPath = $@"{Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments)}\{ProjectConsts.PROJECTS_FOLDER_NAME}\";
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

        private ObservableCollection<ProjectTemplate> _projectTemplates = new ObservableCollection<ProjectTemplate>();
        public ReadOnlyObservableCollection<ProjectTemplate> ProjectTemplates
        {
            get;
        }

        public CreateProject()
        {
            ProjectTemplates = new ReadOnlyObservableCollection<ProjectTemplate>(_projectTemplates);

            try
            {
                var templates = Directory.GetFiles(_templatesPath, ProjectConsts.TEMPLATE_FILE_NAME, SearchOption.AllDirectories);
                Debug.Assert(templates.Any());

                foreach (var file in templates)
                {
                    var fileDirectoryPath = Path.GetDirectoryName(file);
                    Debug.Assert(fileDirectoryPath != null);

                    var template = Serializer.FromFile<ProjectTemplate>(file);
                    Debug.Assert(template != null, $"Can't read tempalte from file {file}");
                    Debug.Assert(template.File != null, $"Can't get orihect path for template {template.Type} because it's file is null.");

                    template.IconPath = Path.GetFullPath(Path.Combine(fileDirectoryPath, ProjectConsts.ICON_FILE_NAME));
                    template.Icon = File.ReadAllBytes(template.IconPath);

                    template.ScreenshotPath = Path.GetFullPath(Path.Combine(fileDirectoryPath, ProjectConsts.SCREENSHOT_FILE_NAME));
                    template.Screenshot = File.ReadAllBytes(template.ScreenshotPath);

                    template.ProjectPath = Path.GetFullPath(Path.Combine(fileDirectoryPath, template.File));
                    template.Screenshot = File.ReadAllBytes(template.ScreenshotPath);

                    _projectTemplates.Add(template);
                }

                ValidateProjectPath();
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
                Logger.Log(MessageType.Error, $"Failed to read project templates");
                throw;
            }
        }

        public string CreateNewProject(ProjectTemplate template)
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

            var path = $@"{ProjectPath}{ProjectName}\";
            try
            {
                if (!Directory.Exists(path))
                {
                    Directory.CreateDirectory(path);
                }

                Debug.Assert(template.Folders != null, $"Folders can't be null in project template {template.Type}");
                foreach (var folder in template.Folders)
                {
                    string directoryName = Path.GetDirectoryName(path);
                    Debug.Assert(directoryName != null, $"Can't get directory name for path {path}");

                    var combinedPath = Path.Combine(directoryName, folder);
                    var fullPath = Path.GetFullPath(combinedPath);

                    Directory.CreateDirectory(fullPath);
                }

                var dirInfo = new DirectoryInfo(path + @".Paragon\");
                dirInfo.Attributes |= FileAttributes.Hidden;

                Debug.Assert(template.IconPath != null, $"Icon Path can't be null in project template {template.Type}");
                File.Copy(template.IconPath, Path.GetFullPath(Path.Combine(dirInfo.FullName, ProjectConsts.ICON_FILE_NAME)));

                Debug.Assert(template.ScreenshotPath != null, $"Screenshot  Path can't be null in project template {template.Type}");
                File.Copy(template.ScreenshotPath, Path.GetFullPath(Path.Combine(dirInfo.FullName, ProjectConsts.SCREENSHOT_FILE_NAME)));

                Debug.Assert(template.ProjectPath != null, $"Project Path can't be null in project template {template.Type}");
                var projectXml = File.ReadAllText(template.ProjectPath);
                projectXml = string.Format(projectXml, ProjectName, ProjectPath);
                
                var projectPath = Path.GetFullPath(Path.Combine(path, $"{ProjectName}{ProjectConsts.PROJECT_EXTENSION}"));
                
                File.WriteAllText(projectPath, projectXml);

                return path;
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
                Logger.Log(MessageType.Error, $"Failed to create {ProjectName}");
                throw;
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
            else if (Directory.Exists(path) && Directory.EnumerateFileSystemEntries(path).Any())
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