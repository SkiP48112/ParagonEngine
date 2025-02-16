using System.Windows;
using System.Windows.Controls;
using System.Windows.Media.Animation;


namespace Editor.GameProject
{
    public partial class ProjectBrowsingDialog : Window
    {
        private readonly CubicEase cubicEase = new CubicEase() { EasingMode = EasingMode.EaseInOut };

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

        private void AnimateToOpenProjectView()
        {
            var highlightAnim = new DoubleAnimation(400, 200, new Duration(TimeSpan.FromSeconds(0.2)));
            highlightAnim.EasingFunction = cubicEase;
            highlightAnim.Completed += (s, e) => 
            {
                var thicknessAnimation = new ThicknessAnimation(new Thickness(-1600, 0, 0, 0), new Thickness(0), new Duration(TimeSpan.FromSeconds(0.5)));
                thicknessAnimation.EasingFunction = cubicEase;
                browserContent.BeginAnimation(MarginProperty, thicknessAnimation);
            };

            highlightRect.BeginAnimation(Canvas.LeftProperty, highlightAnim);
        }

        private void AnimateToCreateProjectView()
        {
            var highlightAnim = new DoubleAnimation(200, 400, new Duration(TimeSpan.FromSeconds(0.2)));
            highlightAnim.EasingFunction = cubicEase;
            highlightAnim.Completed += (s, e) =>
            {
                var thicknessAnimation = new ThicknessAnimation(new Thickness(0), new Thickness(-1600, 0, 0, 0), new Duration(TimeSpan.FromSeconds(0.5)));
                thicknessAnimation.EasingFunction = cubicEase;
                browserContent.BeginAnimation(MarginProperty, thicknessAnimation);
            };

            highlightRect.BeginAnimation(Canvas.LeftProperty, highlightAnim);
        }

        private void OnOpenProjectButton_Click(object sender, RoutedEventArgs e)
        {
            if (createProjectButton.IsChecked == true)
            {
                createProjectButton.IsChecked = false;

                AnimateToOpenProjectView();
                openProjectView.IsEnabled = true;
                createProjectView.IsEnabled = false;
            }

            openProjectButton.IsChecked = true;
        }

        private void OnCreateProjectButton_Click(object sender, RoutedEventArgs e)
        {
            if (openProjectButton.IsChecked == true)
            {
                openProjectButton.IsChecked = false;

                AnimateToCreateProjectView();
                createProjectView.IsEnabled = true;
                openProjectView.IsEnabled = false;
            }

            createProjectButton.IsChecked = true;
        }
    }
}
