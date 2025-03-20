using System.Globalization;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace Editor.Utilities.Controls
{
    [TemplatePart(Name = "PART_textBlock", Type = typeof(TextBlock))]
    [TemplatePart(Name = "PART_textBox", Type = typeof(TextBox))]
    class NumberBox : Control
    {
        private double _originalValue;
        private double _mouseXStart;
        private double _multiplier;

        private bool _isCaptured = false;
        private bool _isValueChanged = false;

        public event RoutedEventHandler ValueChanged
        {
            add { AddHandler(ValueChangedEvent, value); }
            remove { RemoveHandler(ValueChangedEvent, value); }
        }

        public static readonly RoutedEvent ValueChangedEvent =
            EventManager.RegisterRoutedEvent(nameof(ValueChanged), RoutingStrategy.Bubble,
                typeof(RoutedEventHandler), typeof(NumberBox));

        public string Value
        {
            get => (string)GetValue(ValueProperty);
            set => SetValue(ValueProperty, value);
        }

        public static readonly DependencyProperty ValueProperty = 
            DependencyProperty.Register(nameof(Value), typeof(string), typeof(NumberBox),
                new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault,
                new PropertyChangedCallback(OnValueChanged)));

        private static void OnValueChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            var numberBox = d as NumberBox;
            numberBox.RaiseEvent(new RoutedEventArgs(ValueChangedEvent));
        }

        public double Multiplier
        {
            get => (double)GetValue(MultiplierProperty);
            set => SetValue(MultiplierProperty, value);
        }

        public static readonly DependencyProperty MultiplierProperty =
            DependencyProperty.Register(nameof(Multiplier), typeof(double), typeof(NumberBox),
                new PropertyMetadata(1.0));

        static NumberBox()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(NumberBox), new FrameworkPropertyMetadata(typeof(NumberBox)));
        }

        public override void OnApplyTemplate()
        {
            base.OnApplyTemplate();

            if(GetTemplateChild("PART_textBlock") is TextBlock textBlock)
            {
                textBlock.MouseLeftButtonDown += OnTextBlockMouseLeftButtonDown;
                textBlock.MouseLeftButtonUp += OnTextBlockMouseLeftButtonUp;
                textBlock.MouseMove += OnTextBlockMouseMove;
            }
        }

        private void OnTextBlockMouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            double.TryParse(Value, out _originalValue);

            Mouse.Capture(sender as UIElement);
            _isCaptured = true;
            _isValueChanged = false;
            
            e.Handled = true;
            _mouseXStart = e.GetPosition(this).X;
            Focus();
        }

        private void OnTextBlockMouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            if (!_isCaptured)
            {
                return;
            }

            Mouse.Capture(null);
            _isCaptured = false;
            e.Handled = true;

            if(!_isValueChanged && GetTemplateChild("PART_textBox") is TextBox textBox)
            {
                textBox.Visibility = Visibility.Visible;
                textBox.Focus();
                textBox.SelectAll();
            }
        }

        private void OnTextBlockMouseMove(object sender, MouseEventArgs e)
        {
            if(!_isCaptured)
            {
                return;
            }

            var mouseX = e.GetPosition(this).X;
            var delta = mouseX - _mouseXStart;

            if(Math.Abs(delta) <= SystemParameters.MinimumHorizontalDragDistance)
            {
                return;
            }

            UpdateMultiplier();

            var newValue = _originalValue + (delta * _multiplier * Multiplier);
            Value = newValue.ToString("G5", CultureInfo.InvariantCulture);

            _isValueChanged = true;
        }

        private void UpdateMultiplier()
        {
            if (Keyboard.Modifiers.HasFlag(ModifierKeys.Control))
            {
                _multiplier = 0.001;
            }
            else if (Keyboard.Modifiers.HasFlag(ModifierKeys.Shift))
            {
                _multiplier = 0.1;
            }
            else
            {
                _multiplier = 0.01;
            }
        }
    }
}
