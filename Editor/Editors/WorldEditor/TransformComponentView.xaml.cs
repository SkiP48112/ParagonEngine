using Editor.Components;
using Editor.GameProject;
using System.Diagnostics;
using System.Numerics;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace Editor.Editors
{
    public partial class TransformComponentView : UserControl
    {
        private Action _undoAction = null;
        private bool _propertyChanged = false;

        public TransformComponentView()
        {
            InitializeComponent();
            Loaded += OnTransformComponentViewLoaded;
        }

        private void OnTransformComponentViewLoaded(object sender, RoutedEventArgs e)
        {
            Loaded -= OnTransformComponentViewLoaded;
            ((MSCTransform)DataContext).PropertyChanged += (s, e) => _propertyChanged = true;
        }

        private Action GetAction(Func<Transform, (Transform transform, Vector3)> selector, Action<(Transform transform, Vector3)> forEachAction)
        {
            var viewModel = DataContext as MSCTransform;
            if (viewModel == null)
            {
                _propertyChanged = false;
                _undoAction = null;
                return null;
            }

            var selection = viewModel.SelectedComponents.Select(selector).ToList();
            return new Action(() =>
            {
                selection.ForEach(forEachAction);
                (GameEntityView.Instance?.DataContext as MSEntity)?.GetMSComponent<MSCTransform>()?.Refresh();
            });
        }

        private Action GetPositionAction() => GetAction((x) => (x, x.Position), (x) => x.transform.Position = x.Item2);
        private Action GetRotationAction() => GetAction((x) => (x, x.Rotation), (x) => x.transform.Rotation = x.Item2);
        private Action GetScaleAction() => GetAction((x) => (x, x.Scale), (x) => x.transform.Scale = x.Item2);

        private void RecordActions(Action redoAction, string name)
        {
            if (!_propertyChanged)
            {
                return;
            }

            Debug.Assert(_undoAction != null);
            Debug.Assert(redoAction != null);
            _propertyChanged = false;

            Project.AddNewUndoRedoAction(name, _undoAction, redoAction);
        }

        private void OnPositionVectorBox_PreviewMouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            _propertyChanged = false;
            _undoAction = GetPositionAction();
        }

        private void OnPositionVectorBox_PreviewMouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            RecordActions(GetPositionAction(), "PositionChanged");
        }

        private void OnRotationVectorBox_PreviewMouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            _propertyChanged = false;
            _undoAction = GetRotationAction();
        }

        private void OnRotationVectorBox_PreviewMouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            RecordActions(GetRotationAction(), "RotationChanged");
        }

        private void OnScaleVectorBox_PreviewMouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            _propertyChanged = false;
            _undoAction = GetScaleAction();
        }

        private void OnScaleVectorBox_PreviewMouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            RecordActions(GetScaleAction(), "ScaleChanged");
        }

        private void OnPositionVector_LostKeyboardFocus(object sender, KeyboardFocusChangedEventArgs e)
        {
            if(_propertyChanged && _undoAction != null)
            {
                OnPositionVectorBox_PreviewMouseLeftButtonUp(sender, null);
            }
        }

        private void OnRotationVector_LostKeyboardFocus(object sender, KeyboardFocusChangedEventArgs e)
        {
            if (_propertyChanged && _undoAction != null)
            {
                OnRotationVectorBox_PreviewMouseLeftButtonUp(sender, null);
            }
        }

        private void OnScaleVector_LostKeyboardFocus(object sender, KeyboardFocusChangedEventArgs e)
        {
            if (_propertyChanged && _undoAction != null)
            {
                
                OnScaleVectorBox_PreviewMouseLeftButtonUp(sender, null);
            }
        }
    }
}
