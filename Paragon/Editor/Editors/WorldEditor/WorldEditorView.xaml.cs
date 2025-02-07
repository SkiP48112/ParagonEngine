using System.Collections.Specialized;
using System.Windows;
using System.Windows.Controls;

namespace Editor.Editors
{
    public partial class WorldEditorView : UserControl
    {
        public WorldEditorView()
        {
            InitializeComponent();

            Loaded += HandleOnWorldEditorViewLoaded;
        }

        private void HandleOnWorldEditorViewLoaded(object sender, RoutedEventArgs e)
        {
            Loaded -= HandleOnWorldEditorViewLoaded;
            Focus();

            ((INotifyCollectionChanged)GameProject.Project.UndoRedoManager.UndoList).CollectionChanged += (s, e) => Focus();
        }
    }
}
