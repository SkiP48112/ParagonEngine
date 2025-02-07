using System.Collections.ObjectModel;
using System.Runtime.CompilerServices;
using System.Windows;
using System.Windows.Data;

namespace Editor.Utilities
{
    static class Logger
    {
        private static int _filter = (int)(MessageType.Info | MessageType.Warning | MessageType.Error);
        private readonly static ObservableCollection<LogMessage> _messages = new ObservableCollection<LogMessage>();
        public static ReadOnlyObservableCollection<LogMessage> Messages { get; } = new ReadOnlyObservableCollection<LogMessage>(_messages);
        public static CollectionViewSource FilteredMessages { get; } = new CollectionViewSource() { Source = Messages };

        static Logger()
        {
            FilteredMessages.Filter += (s, e) =>
            {
                int type = (int)((LogMessage)e.Item).Type;
                e.Accepted = (type & _filter) != 0;
            };
        }

        public static async void Log(
            MessageType type,
            string msg,
            [CallerFilePath] string file = "",
            [CallerMemberName] string caler = "",
            [CallerLineNumber] int line = 0)
        {
            // Creates a new task on the UI thread
            await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
            {
                _messages.Add(new LogMessage(type, msg, file, caler, line));
            }));
        }

        public static async void Clear()
        {
            await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
            {
                _messages.Clear();
            }));
        }

        public static void SetFilter(int mask)
        {
            _filter = mask;
            FilteredMessages.View.Refresh();
        }
    }
}
