using Editor.Assets;
using Editor.GameDevelopment;
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
        }

        private void OnCreateScriptButton_Click(object sender, RoutedEventArgs e)
        {
            new CreateScriptDialog().ShowDialog();
        }

        private void OnCreatePrimitiveMeshButton_Click(object sender, RoutedEventArgs e)
        {
            var dialog = new PrimitiveMeshDialog();
            dialog.ShowDialog();
        }
    }
}
