using System.Windows.Controls;

namespace Editor.Utilities
{
    public partial class LoggerView : UserControl
    {
        public LoggerView()
        {
            InitializeComponent();
        }

        private void OnClearButton_Click(object sender, System.Windows.RoutedEventArgs e)
        {
            Logger.Clear();
        }

        private void OnMessageFilterButton_Click(object sender, System.Windows.RoutedEventArgs e)
        {
            var filter = 0x0;
            if (toggleInfo.IsChecked == true)
            {
                filter |= (int)MessageType.Info;
            }

            if (toggleWarning.IsChecked == true)
            {
                filter |= (int)MessageType.Warning;
            }

            if (toggleError.IsChecked == true)
            {
                filter |= (int)MessageType.Error;
            }

            Logger.SetFilter(filter);
        }
    }
}
