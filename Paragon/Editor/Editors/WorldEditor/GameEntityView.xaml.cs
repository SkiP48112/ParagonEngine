using Editor.Components;
using Editor.GameProject;
using System.Diagnostics;
using System.Windows.Controls;


namespace Editor.Editors
{
    public partial class GameEntityView : UserControl
    {
        private Action _undoAction;
        private string? _propertyName;

        public static GameEntityView Instance { get; private set; }

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
            var selection = viewModel.SelectedEntities.Select(entity => (entity, entity.IsEnabled)).ToList();
            return new Action(() =>
            {
                selection.ForEach(item => item.entity.IsEnabled = item.IsEnabled);
                viewModel.Refresh();
            });
        }

        private void OnNameTextBox_GotKeyboardFocus(object sender, System.Windows.Input.KeyboardFocusChangedEventArgs e)
        {
            _undoAction = GetRenameAction();
        }

        private void OnNameTextBox_LostKeyboardFocus(object sender, System.Windows.Input.KeyboardFocusChangedEventArgs e)
        {
            if(_propertyName == nameof(MSEntity.Name) && _undoAction != null)
            {
                var redoAction = GetRenameAction();
                GameProject.GameProject.AddNewUndoRedoAction("Rename game entity", _undoAction, redoAction);
                _propertyName = null;
            }

            _undoAction = null;
        }

        private void OnEnabledCheckBox_Click(object sender, System.Windows.RoutedEventArgs e)
        {
            var undoAction = GetIsEnabledAction();
            var viewModel = DataContext as MSEntity;
            Debug.Assert(viewModel != null);

            viewModel.IsEnabled = ((CheckBox)sender).IsChecked == true;

            var redoAction = GetIsEnabledAction();
            GameProject.GameProject.AddNewUndoRedoAction(viewModel.IsEnabled == true ? "Enable game entity" : "Disable game entity", undoAction, redoAction);
        }
    }
}
