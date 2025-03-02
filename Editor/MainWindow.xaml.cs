using Editor.GameProject;
using System.ComponentModel;
using System.IO;
using System.Windows;

namespace Editor
{
    public partial class MainWindow : Window
    {
        public static string ParagonPath { get; set; } = string.Empty;

        public MainWindow()
        {
            InitializeComponent();
            Loaded += OnMainWindowLoaded;
            Closing += OnMainWindowClosing;
        }

        private void OnMainWindowLoaded(object sender, RoutedEventArgs e)
        {
            Loaded -= OnMainWindowLoaded;
            GetEnginePath();
            OpenProjectBrowsingDialog();
        }

        private void GetEnginePath()
        {
            var enginePath = Environment.GetEnvironmentVariable("PARAGON_ENGINE", EnvironmentVariableTarget.User);
            if (enginePath == null || !Directory.Exists(Path.Combine(enginePath, @"Engine\engine_api")))
            {
                var dialog = new EnginePathDialog();
                if(dialog.ShowDialog() == true)
                {
                    enginePath = dialog.ParagonPath;
                    Environment.SetEnvironmentVariable("PARAGON_ENGINE", enginePath!.ToUpper(), EnvironmentVariableTarget.User);
                }
                else
                {
                    Application.Current.Shutdown();
                }
            }
            else
            {
                ParagonPath = enginePath;
            }
        }

        private void OnMainWindowClosing(object sender, CancelEventArgs e)
        {
            Closing -= OnMainWindowClosing;
            UnloadCurrentProject();
        }

        private void UnloadCurrentProject()
        {
            Project.CurrentGameProject?.Unload();
        }

        private void OpenProjectBrowsingDialog()
        {
            var projectBrowsingDialog = new ProjectBrowsingDialog();
            if (projectBrowsingDialog.ShowDialog() == false || projectBrowsingDialog.DataContext == null)
            {
                Application.Current.Shutdown();
            }
            else
            {
                UnloadCurrentProject();
                DataContext = projectBrowsingDialog.DataContext;
            }
        }
    }
}