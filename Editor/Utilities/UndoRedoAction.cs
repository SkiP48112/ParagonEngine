using System.Diagnostics;

namespace Editor.Utilities
{
    public class UndoRedoAction : IUndoRedo
    {
        public string Name { get; }
        private Action? _undoAction;
        private Action? _redoAction;

        public UndoRedoAction(string name)
        {
            Name = name;
        }

        public UndoRedoAction(Action undo, Action redo, string name) : this(name)
        {
            Debug.Assert(undo != null && redo != null);
            _undoAction = undo;
            _redoAction = redo;
        }

        public UndoRedoAction(string property, object instance, object undoValue, object redoValue, string name) :
            this(
                () => SetValue(property, instance, undoValue),
                () => SetValue(property, instance, redoValue),
                name)
        {
        }

        public void Redo()
        {
            Debug.Assert(_redoAction != null, $"Can't execute {Name} redo action because it is null.");
            _redoAction();
        }

        public void Undo() 
        {
            Debug.Assert(_undoAction != null, $"Can't execute {Name} undo action because it is null.");
            _undoAction(); 
        }

        private static void SetValue(string propertyName, object instance, object actionValue)
        {
            var type = instance.GetType();
            var property = type.GetProperty(propertyName);

            property?.SetValue(instance, actionValue);
        }
    }
}
