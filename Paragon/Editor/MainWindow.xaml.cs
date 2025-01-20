using Editor.GameProject;
using System.Windows;

namespace Editor
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            Loaded += OnMainWindowLoaded;
        }

        private void OnMainWindowLoaded(object sender, RoutedEventArgs e)
        {
            Loaded -= OnMainWindowLoaded;
            OpenProjectBrowsingDialog();
        }

        private void OpenProjectBrowsingDialog()
        {
            var projectBrowsingDialog = new ProjectBrowsingDialog();
            if (projectBrowsingDialog.ShowDialog() == false){
                Application.Current.Shutdown();
            }
            else
            {

            }
        }
    }
}