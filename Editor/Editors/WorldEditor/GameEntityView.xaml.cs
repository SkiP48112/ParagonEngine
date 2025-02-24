using Editor.Components;
using Editor.GameProject;
using System.Diagnostics;
using System.Globalization;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;

namespace Editor.Editors
{
    public class NullableBoolToBoolConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return value is bool b && b == true;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return value is bool b && b == true;
        }
    }

    public partial class GameEntityView : UserControl
    {
        private Action? _undoAction;
        private string? _propertyName;

        public static GameEntityView? Instance { get; private set; }

        public GameEntityView()
        {
            InitializeComponent();
            DataContext = null;
            Instance = this;

            DataContextChanged += (_, _) =>
            {
                if (DataContext != null)
                {
                    ((MSEntity)DataContext).PropertyChanged += (s, e) => _propertyName = e.PropertyName;
                }
            };
        }

        private Action GetRenameAction()
        {
            var viewModel = DataContext as MSEntity;
            Debug.Assert(viewModel != null, $"Can't cast {nameof(DataContext)} to {nameof(MSEntity)}");

            var selection = viewModel.SelectedEntities.Select(entity => (entity, entity.Name)).ToList();
            return new Action(() =>
            {
                selection.ForEach(item => item.entity.Name = item.Name);
                viewModel.Refresh();
            });
        }

        private Action GetIsEnabledAction()
        {
            var viewModel = DataContext as MSEntity;
            Debug.Assert(viewModel != null, $"Can't cast {nameof(DataContext)} to {nameof(MSEntity)}");

            var selection = viewModel.SelectedEntities.Select(entity => (entity, entity.IsEnabled)).ToList();
            return new Action(() =>
            {
                selection.ForEach(item => item.entity.IsEnabled = item.IsEnabled);
                viewModel.Refresh();
            });
        }

        private void OnNameTextBox_GotKeyboardFocus(object sender, System.Windows.Input.KeyboardFocusChangedEventArgs e)
        {
            _propertyName = string.Empty;
            _undoAction = GetRenameAction();
        }

        private void OnNameTextBox_LostKeyboardFocus(object sender, System.Windows.Input.KeyboardFocusChangedEventArgs e)
        {
            if (_propertyName == nameof(MSEntity.Name) && _undoAction != null)
            {
                var redoAction = GetRenameAction();
                GameProject.Project.AddNewUndoRedoAction("Rename game entity", _undoAction, redoAction);
                _propertyName = null;
            }

            _undoAction = null;
        }

        private void OnEnabledCheckBox_Click(object sender, System.Windows.RoutedEventArgs e)
        {
            var undoAction = GetIsEnabledAction();
            var viewModel = DataContext as MSEntity;
            Debug.Assert(viewModel != null, $"Can't cast {nameof(DataContext)} to {nameof(MSEntity)}");

            viewModel.IsEnabled = ((CheckBox)sender).IsChecked == true;

            var redoAction = GetIsEnabledAction();
            GameProject.Project.AddNewUndoRedoAction(viewModel.IsEnabled == true ? "Enable game entity" : "Disable game entity", undoAction, redoAction);
        }

        private void OnAddComponentButton_PreviewBouseLeftButtonDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            var menu = FindResource("addComponentMenu") as ContextMenu;
            Debug.Assert(menu != null);

            var button = sender as ToggleButton;
            Debug.Assert(button != null);

            button.IsChecked = true;

            menu.Placement = PlacementMode.Bottom;
            menu.PlacementTarget = button;
            menu.MinWidth = button.ActualWidth;
            menu.IsOpen = true;
        }

        private void OnAddScriptComponent(object sender, System.Windows.RoutedEventArgs e)
        {
            AddComponent(ComponentType.Script, ((MenuItem)sender).Header.ToString()!);
        }

        private void AddComponent(ComponentType type, object data)
        {
            var creationFunction = ComponentFactory.GetCreationFunction(type);
            var changedEntities = new List<(GameEntity entity, Component component)>();
            var viewModel = (MSEntity)DataContext;

            foreach(var entity in viewModel.SelectedEntities)
            {
                var component = creationFunction(entity, data);
                if (entity.AddComponent(component))
                {
                    changedEntities.Add((entity, component));
                }
            }

            if (changedEntities.Any())
            {
                viewModel.Refresh();

                Project.AddNewUndoRedoAction
                (
                    $"Add {type} component",
                    () =>
                    {
                        changedEntities.ForEach(x => x.entity.RemoveComponent(x.component));
                        ((MSEntity)DataContext).Refresh();
                    },
                    () =>
                    {
                        changedEntities.ForEach(x => x.entity.AddComponent(x.component));
                        ((MSEntity)DataContext).Refresh();
                    }
                );
            }
        }
    }
}
