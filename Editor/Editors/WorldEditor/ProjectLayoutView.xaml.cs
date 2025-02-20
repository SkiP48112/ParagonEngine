using Editor.Components;
using Editor.GameProject;
using System.Diagnostics;
using System.Windows;
using System.Windows.Controls;

namespace Editor.Editors
{
    public partial class ProjectLayoutView : UserControl
    {
        public ProjectLayoutView()
        {
            InitializeComponent();
        }

        private void OnAddGameEntityButton_Click(object sender, RoutedEventArgs e)
        {
            var button = sender as Button;
            Debug.Assert(button != null, $"Can't cast {nameof(sender)} to {nameof(Button)}");

            var viewModel = button.DataContext as Scene;
            Debug.Assert(viewModel != null, $"Can't cast {nameof(button.DataContext)} to {nameof(Scene)}");

            viewModel.AddGameEntityCommand?.Execute(new GameEntity(viewModel) { Name = "Empty Game Entity" });
        }

        private void OnGameEntitiesListBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            var listBox = sender as ListBox;
            Debug.Assert(listBox != null);
            var newSelection = listBox.SelectedItems.Cast<GameEntity>().ToList();
            var prevSelection = newSelection.Except(e.AddedItems.Cast<GameEntity>()).Concat(e.RemovedItems.Cast<GameEntity>()).ToList();

            GameProject.Project.AddNewUndoRedoAction("Selection Changed", () => SelectItems(listBox, prevSelection), () => SelectItems(listBox, newSelection));

            MSGameEntity msEntity = null;
            if (newSelection.Any())
            {
                msEntity = new MSGameEntity(newSelection);
            }

            var gameEntityViewInstance = GameEntityView.Instance;
            Debug.Assert(gameEntityViewInstance != null, $"{nameof(GameEntityView)} instance is null");

            gameEntityViewInstance.DataContext = msEntity;
        }

        private void SelectItems(ListBox listBox, List<GameEntity> items)
        {
            listBox.UnselectAll();
            items.ForEach(item => ((ListBoxItem)listBox.ItemContainerGenerator.ContainerFromItem(item)).IsSelected = true);
        }
    }
}
