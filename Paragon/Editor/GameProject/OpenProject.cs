using Editor.Utilities;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using System.Runtime.Serialization;
using System.Windows.Shapes;

namespace Editor.GameProject
{
    [DataContract]
    public class GameProjectData
    {
        [DataMember]
        public string Name { get; set; }
        [DataMember]
        public string Path { get; set; }
        [DataMember]
        public DateTime Date { get; set; }
        
        public string FullPath { get => $"{Path}{Name}{GameProjectConsts.PROJECT_EXTENSION}"; }
        public byte[] Icon { get; set; }
        public byte[] Screenshot { get; set; }
    }


    [DataContract]
    public class GameProjectDataList
    {
        [DataMember]
        public List<GameProjectData> Projects { get; set; }
    }


    class OpenProject
    {
        private static readonly string _applicationDataPath = $@"{Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData)}\ParagonEngine\Editor";
        private static readonly string _projectDataPath = string.Empty;

        private static readonly ObservableCollection<GameProjectData> _gameProjects = new ObservableCollection<GameProjectData>();
        public static ReadOnlyObservableCollection<GameProjectData> GameProjects { get; }


        static OpenProject()
        {
            try
            {
                if (!Directory.Exists(_applicationDataPath))
                {
                    Directory.CreateDirectory(_applicationDataPath);
                }

                _projectDataPath = $@"{_applicationDataPath}ProjectData.xml";
                GameProjects = new ReadOnlyObservableCollection<GameProjectData>(_gameProjects);
                ReadProjectData();
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
                Logger.Log(MessageType.Error, $"Failed to read project data");
                throw;
            }
        }

        private static void ReadProjectData()
        {
            if (File.Exists(_projectDataPath))
            {
                var projects = Serializer.FromFile<GameProjectDataList>(_projectDataPath).Projects.OrderByDescending(x => x.Date);
                _gameProjects.Clear();

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
            Serializer.ToFile(new GameProjectDataList() { Projects = projects }, _projectDataPath);
        }

        public static GameProject Open(GameProjectData data)
        {
            ReadProjectData();
            var project = _gameProjects.FirstOrDefault(x => x.FullPath == data.FullPath);
            if(project != null)
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

            return GameProject.Load(project.FullPath);
        }
    }
}
