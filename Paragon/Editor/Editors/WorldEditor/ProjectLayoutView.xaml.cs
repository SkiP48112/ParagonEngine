﻿using Editor.Components;
using Editor.GameProject;
using Editor.Utilities;
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
            var viewModel = button.DataContext as Scene;
            viewModel.AddGameEntityCommand.Execute(new GameEntity(viewModel) { Name = "Empty Game Entity" });
        }

        private void OnGameEntitiesListBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            GameEntityView.Instance.DataContext = null;
            var listBox = sender as ListBox;
            Debug.Assert(listBox != null);

            if(e.AddedItems.Count > 0)
            {
                GameEntityView.Instance.DataContext = listBox.SelectedItems[0];
            }

            var newSelection = listBox.SelectedItems.Cast<GameEntity>().ToList();
            var prevSelection = newSelection.Except(e.AddedItems.Cast<GameEntity>()).Concat(e.RemovedItems.Cast<GameEntity>()).ToList();

            GameProject.GameProject.AddNewUndoRedoAction("Selection Changed", () => SelectItems(listBox, prevSelection), () => SelectItems(listBox, newSelection));
        }

        private void SelectItems(ListBox listBox, List<GameEntity> items)
        {
            listBox.UnselectAll();
            items.ForEach(item => ((ListBoxItem)listBox.ItemContainerGenerator.ContainerFromItem(item)).IsSelected = true);
        }
    }
}
