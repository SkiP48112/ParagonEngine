using System.IO;
using System.Windows;

namespace Editor
{
    public partial class EnginePathDialog : Window
    {
        public string ParagonPath { get; private set; }

        public EnginePathDialog()
        {
            InitializeComponent();
            Owner = Application.Current.MainWindow;
        }

        private void OnOKButton_Click(object sender, RoutedEventArgs e)
        {
            var path = pathTextBox.Text.Trim();
            messageTextBlock.Text = string.Empty;

            if (string.IsNullOrEmpty(path))
            {
                messageTextBlock.Text = "Invalid path.";
            }
            else if (path.IndexOfAny(Path.GetInvalidPathChars()) != -1)
            {
                messageTextBlock.Text = "Invalid character(s) used in path.";
            }
            else if (!Directory.Exists(Path.Combine(path, @"Engine\engine_api\")))
            {
                messageTextBlock.Text = "Unable to find the engine at the specified location.";
            }

            if (string.IsNullOrEmpty(messageTextBlock.Text))
            {
                if (!Path.EndsInDirectorySeparator(path))
                {
                    path += @"\";
                }

                ParagonPath = path;
                DialogResult = true;
                Close();
            }
        }
    }
}
