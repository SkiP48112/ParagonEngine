using System.Windows;


namespace Editor.GameProject
{
    public partial class ProjectBrowsingDialog : Window
    {
        public ProjectBrowsingDialog()
        {
            InitializeComponent();
            Loaded += HandleOnProjectBrowserDialogLoaded;
        }

        private void HandleOnProjectBrowserDialogLoaded(object sender, RoutedEventArgs e)
        {
            Loaded -= HandleOnProjectBrowserDialogLoaded;
            if (!OpenProject.GameProjects.Any())
            {
                openProjectButton.IsEnabled = false;
                openProjectView.Visibility = Visibility.Hidden;
                OnCreateProjectButton_Click(createProjectButton, new RoutedEventArgs());
            }
        }

        private void OnOpenProjectButton_Click(object sender, RoutedEventArgs e)
        {
            if (createProjectButton.IsChecked == true)
            {
                createProjectButton.IsChecked = false;
                browserContent.Margin = new Thickness(0);
            }

            openProjectButton.IsChecked = true;
        }

        private void OnCreateProjectButton_Click(object sender, RoutedEventArgs e)
        {
            if (openProjectButton.IsChecked == true)
            {
                openProjectButton.IsChecked = false;
                browserContent.Margin = new Thickness(-800, 0, 0, 0);
            }

            createProjectButton.IsChecked = true;
        }

    }
}
