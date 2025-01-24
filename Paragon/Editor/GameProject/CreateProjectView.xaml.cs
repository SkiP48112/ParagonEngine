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
            var viewModel = DataContext as NewGameProjectViewModel;
            if (viewModel == null)
            {
                return;
            }

            var template = templatesListBox.SelectedItem as GameProjectTemplate;
            if (template == null)
            {
                return;
            }

            var projectPath = viewModel.CreateProject(template);
            
            Window window = Window.GetWindow(this);
            window.DialogResult = !string.IsNullOrEmpty(projectPath);
            window.Close();
        }
    }
}
