using System.Windows;
using System.Windows.Media;

namespace Editor
{
    static class VisualExtensions
    {
        public static T FindVisualParent<T>(this DependencyObject dependencyObject) where T : DependencyObject
        {
            if(!(dependencyObject is Visual))
            {
                return default(T);
            }

            var parent = VisualTreeHelper.GetParent(dependencyObject);
            while(parent != null)
            {
                if(parent is T type)
                {
                    return type;
                }

                parent = VisualTreeHelper.GetParent(parent);
            }

            return default(T);
        }
    }
}
