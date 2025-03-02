using Editor.Utilities;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;

namespace Editor.GameProject
{
    class OpenProject
    {
        private static readonly string _applicationDataPath = $@"{Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData)}\ParagonEngine\Editor";
        private static readonly string _projectDataPath = string.Empty;

        private static readonly ObservableCollection<ProjectData> _gameProjects = new ObservableCollection<ProjectData>();
        public static ReadOnlyObservableCollection<ProjectData> GameProjects { get; }

        static OpenProject()
        {
            try
            {
                if (!Directory.Exists(_applicationDataPath))
                {
                    Directory.CreateDirectory(_applicationDataPath);
                }

                _projectDataPath = $@"{_applicationDataPath}ProjectData.xml";
                GameProjects = new ReadOnlyObservableCollection<ProjectData>(_gameProjects);
                ReadProjectData();
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
                Logger.Log(MessageType.Error, $"Failed to read project data");
                throw;
            }
        }

        public static Project Open(ProjectData data)
        {
            ReadProjectData();
            var project = _gameProjects.FirstOrDefault(x => x.FullPath == data.FullPath);
            if (project != null)
            {
                project.Date = DateTime.Now;
            }
            else
            {
                project = data;
                project.Date = DateTime.Now;
                _gameProjects.Add(project);
            }

            WriteProjectData();

            return Project.Load(project.FullPath);
        }

        private static void ReadProjectData()
        {
            if (File.Exists(_projectDataPath))
            {
                var projects = Serializer.FromFile<ProjectDataList>(_projectDataPath)?.Projects?.OrderByDescending(x => x.Date);
                _gameProjects.Clear();

                Debug.Assert(projects != null, $"Error due read from project file {_projectDataPath}");
                foreach (var project in projects)
                {
                    if (File.Exists(project.FullPath))
                    {
                        project.Icon = File.ReadAllBytes($@"{project.Path}\.Paragon\icon.png");
                        project.Screenshot = File.ReadAllBytes($@"{project.Path}\.Paragon\screenshot.png");
                        _gameProjects.Add(project);
                    }
                }
            }
        }

        private static void WriteProjectData()
        {
            var projects = _gameProjects.OrderBy(x => x.Date).ToList();
            Serializer.ToFile(new ProjectDataList() { Projects = projects }, _projectDataPath);
        }
    }
}
