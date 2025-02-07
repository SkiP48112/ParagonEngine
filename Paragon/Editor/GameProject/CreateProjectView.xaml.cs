using System.Windows;
using System.Windows.Controls;

namespace Editor.GameProject
{
    public partial class CreateProjectView : UserControl
    {
        public CreateProjectView()
        {
            InitializeComponent();
        }

        private void OnCreateButton_Click(object sender, RoutedEventArgs e)
        {
            var viewModel = DataContext as CreateProject;
            if (viewModel == null)
            {
                return;
            }

            var template = templatesListBox.SelectedItem as ProjectTemplate;
            if (template == null)
            {
                return;
            }

            var projectPath = viewModel.CreateNewProject(template);

            Window window = Window.GetWindow(this);
            window.DialogResult = !string.IsNullOrEmpty(projectPath);
            if (window.DialogResult == true)
            {
                var project = OpenProject.Open(new ProjectData() { Name = viewModel.ProjectName, Path = projectPath });
                window.DataContext = project;
            }

            window.Close();
        }
    }
}
