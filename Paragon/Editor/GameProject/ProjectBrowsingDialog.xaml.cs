using System.Windows;


namespace Editor.GameProject
{
    public partial class ProjectBrowsingDialog : Window
    {
        public ProjectBrowsingDialog()
        {
            InitializeComponent();
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
