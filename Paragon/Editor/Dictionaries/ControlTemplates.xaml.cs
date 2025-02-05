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
            var textBox = sender as TextBox;
            Debug.Assert(textBox != null);

            var expression = textBox.GetBindingExpression(TextBox.TextProperty);
            if(expression == null)
            {
                return;
            }

            if(e.Key == Key.Enter)
            {
                HandleOnEnterPressed(textBox, expression);
                e.Handled = true;
            }
            else if (e.Key == Key.Escape)
            {
                HandleOnEscapePressed(expression);
            }
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

            Keyboard.ClearFocus();
        }

        private void HandleOnEscapePressed(BindingExpression exp)
        {
            exp.UpdateTarget();
            Keyboard.ClearFocus();
        }
    }
}
