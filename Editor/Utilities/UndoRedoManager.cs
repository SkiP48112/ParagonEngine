using System.Collections.ObjectModel;

namespace Editor.Utilities
{
    public class UndoRedoManager
    {
        private bool _isAddEnabled = true;

        private readonly ObservableCollection<IUndoRedo> _redoList = new ObservableCollection<IUndoRedo>();
        public ReadOnlyObservableCollection<IUndoRedo> RedoList { get; }

        private readonly ObservableCollection<IUndoRedo> _undoList = new ObservableCollection<IUndoRedo>();
        public ReadOnlyObservableCollection<IUndoRedo> UndoList { get; }

        public UndoRedoManager()
        {
            RedoList = new ReadOnlyObservableCollection<IUndoRedo>(_redoList);
            UndoList = new ReadOnlyObservableCollection<IUndoRedo>(_undoList);
        }

        public void Reset()
        {
            _redoList.Clear();
            _undoList.Clear();
        }

        public void Add(IUndoRedo cmd)
        {
            if (!_isAddEnabled)
            {
                return;
            }

            _undoList.Add(cmd);
            _redoList.Clear();
        }

        public void Undo()
        {
            if (_undoList.Any())
            {
                var cmd = _undoList.Last();
                _undoList.RemoveAt(_undoList.Count - 1);

                _isAddEnabled = false;
                cmd.Undo();
                _isAddEnabled = true;

                _redoList.Insert(0, cmd);
            }
        }

        public void Redo()
        {
            if (_redoList.Any())
            {
                var cmd = _redoList.First();
                _redoList.RemoveAt(0);

                _isAddEnabled = false;
                cmd.Redo();
                _isAddEnabled = false;


                _undoList.Add(cmd);
            }
        }

    }
}
