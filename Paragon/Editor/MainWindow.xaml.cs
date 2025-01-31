using Editor.GameProject;
using System.ComponentModel;
using System.Windows;

namespace Editor
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            Loaded += OnMainWindowLoaded;
            Closing += OnMainWindowClosing;
        }

        private void OnMainWindowLoaded(object sender, RoutedEventArgs e)
        {
            Loaded -= OnMainWindowLoaded;
            OpenProjectBrowsingDialog();
        }

        private void OnMainWindowClosing(object? sender, CancelEventArgs e)
        {
            Closing -= OnMainWindowClosing;
            UnloadCurrentProject();
        }

        private void UnloadCurrentProject()
        {
            GameProject.GameProject.CurrentGameProject?.Unload();
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