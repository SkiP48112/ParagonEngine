using System.Windows;
using System.Windows.Controls;

namespace Editor.GameProject
{
    public partial class OpenProjectView : UserControl
    {
        public OpenProjectView()
        {
            InitializeComponent();

            Loaded += (s, e) =>
            {
                var item = gameProjectsListBox.ItemContainerGenerator.ContainerFromIndex(gameProjectsListBox.SelectedIndex) as ListBoxItem;
                item?.Focus();
            };
        }

        private void OnOpenButton_Click(object sender, System.Windows.RoutedEventArgs e)
        {
            OpenSelectedProject();
        }

        private void OnListBoxItem_MouseDoubleClick(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            OpenSelectedProject();
        }

        private void OpenSelectedProject()
        {
            var template = gameProjectsListBox.SelectedItem as GameProjectData;
            if (template == null)
            {
                return;
            }

            var project = OpenProject.Open(template);
            Window window = Window.GetWindow(this);
            if(project != null)
            {
                window.DialogResult = true;
                window.DataContext = project;
            }

            window.Close();
        }
    }
}
