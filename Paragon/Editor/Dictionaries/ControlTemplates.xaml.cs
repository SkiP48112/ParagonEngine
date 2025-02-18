using System.Diagnostics;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Input;

namespace Editor.Dictionaries
{
    public partial class ControlTemplates : ResourceDictionary
    {
        private void OnTextBox_KeyDown(object sender, System.Windows.Input.KeyEventArgs e)
        {
            var textBox = (TextBox)sender;
            var expression = textBox.GetBindingExpression(TextBox.TextProperty);
            if(expression == null)
            {
                return;
            }

            if(e.Key == Key.Enter)
            {
                HandleOnEnterPressed(textBox, expression);
                Keyboard.ClearFocus();
                e.Handled = true;
            }
            else if (e.Key == Key.Escape)
            {
                HandleOnEscapePressed(expression);
                Keyboard.ClearFocus();
            }
        }

        private void OnTextBoxWithRename_KeyDown(object sender, KeyEventArgs e)
        {
            var textBox = (TextBox)sender;
            var expression = textBox.GetBindingExpression(TextBox.TextProperty);
            if (expression == null)
            {
                return;
            }

            if (e.Key == Key.Enter)
            {
                HandleOnEnterPressed(textBox, expression);
                textBox.Visibility = Visibility.Collapsed;
                e.Handled = true;
            }
            else if (e.Key == Key.Escape)
            {
                HandleOnEscapePressed(expression);
                textBox.Visibility = Visibility.Collapsed;
            }
        }

        private void OnTextBoxWithRename_LostFocus(object sender, RoutedEventArgs e)
        {
            var textBox = (TextBox)sender;
            if (!textBox.IsVisible)
            {
                return;
            }

            var expression = textBox.GetBindingExpression(TextBox.TextProperty);
            if (expression == null)
            {
                return;
            }

            expression.UpdateTarget();
            textBox.Visibility = Visibility.Collapsed;
        }

        private void HandleOnEnterPressed(TextBox textBox, BindingExpression exp)
        {
            if(textBox.Tag is ICommand command && command.CanExecute(textBox.Text))
            {
                command.Execute(textBox.Text);
            }
            else
            {
                exp.UpdateSource();
            }
        }

        private void HandleOnEscapePressed(BindingExpression exp)
        {
            exp.UpdateTarget();
        }

        private void OnCloseButton_Click(object sender, RoutedEventArgs e)
        {
            var frameworkElement = sender as FrameworkElement;
            var window = frameworkElement?.TemplatedParent as Window;
            window?.Close();
        }

        private void OnMaximizeRestoreButton_Click(object sender, RoutedEventArgs e)
        {
            var frameworkElement = sender as FrameworkElement;
            var window = frameworkElement?.TemplatedParent as Window;
            Debug.Assert(window != null);

            if (window.WindowState == WindowState.Normal)
            {
                window.WindowState = WindowState.Maximized;
                return;
            }

            window.WindowState = WindowState.Normal;
        }

        private void OnMinimizeButton_Click(object sender, RoutedEventArgs e)
        {
            var frameworkElement = sender as FrameworkElement;
            var window = frameworkElement?.TemplatedParent as Window;
            Debug.Assert(window != null);

            window.WindowState = WindowState.Minimized;
        }
    }
}
