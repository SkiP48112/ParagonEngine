using Editor.Components;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Runtime.Serialization;
using System.Windows.Input;

namespace Editor.GameProject
{
    [DataContract]
    class Scene : ViewModelBase
    {
        private string _name = string.Empty;
        [DataMember]
        public string Name
        {
            get => _name;
            set
            {
                if (_name != value)
                {
                    _name = value;
                    OnPropertyChanged(nameof(Name));
                }
            }
        }

        [DataMember]
        public Project Project { get; private set; }

        private bool _isActive;
        [DataMember]
        public bool IsActive
        {
            get => _isActive;
            set
            {
                if (_isActive != value)
                {
                    _isActive = value;
                    OnPropertyChanged(nameof(IsActive));
                }
            }
        }

        public ICommand AddGameEntityCommand { get; private set; }
        public ICommand RemoveGameEntityCommand { get; private set; }

        [DataMember(Name = nameof(GameEntities))]
        private readonly ObservableCollection<GameEntity> _gameEntities = new ObservableCollection<GameEntity>();
        public ReadOnlyObservableCollection<GameEntity> GameEntities { get; private set; }

        public Scene(Project project, string name)
        {
            Debug.Assert(project != null);

            Project = project;
            Name = name;

            OnDeserialized(new StreamingContext());
        }

        private void AddGameEntity(GameEntity entity, int index = -1)
        {
            Debug.Assert(!_gameEntities.Contains(entity));
            entity.IsActive = IsActive;
            if(index == -1)
            {
                _gameEntities.Add(entity);
                return;
            }

            _gameEntities.Insert(index, entity);
        }

        private void RemoveGameEntity(GameEntity entity)
        {
            Debug.Assert(_gameEntities.Contains(entity));
            entity.IsActive = false;
            _gameEntities.Remove(entity);
        }

        [OnDeserialized]
        private void OnDeserialized(StreamingContext context)
        {
            if (_gameEntities != null)
            {
                GameEntities = new ReadOnlyObservableCollection<GameEntity>(_gameEntities);
                OnPropertyChanged(nameof(GameEntities));
            }

            foreach(var entity in _gameEntities)
            {
                entity.IsActive = IsActive;
            }

            AddGameEntityCommand = new RelayCommand<GameEntity>(x =>
            {
                AddGameEntity(x);
                var index = _gameEntities.Count - 1;
                Project.AddNewUndoRedoAction($"Add {x.Name} to {Name}", () => RemoveGameEntity(x), () => AddGameEntity(x, index));
            });

            RemoveGameEntityCommand = new RelayCommand<GameEntity>(x =>
            {
                var index = _gameEntities.IndexOf(x);
                RemoveGameEntity(x);
                Project.AddNewUndoRedoAction($"Remove {x.Name} from {Name}", () => AddGameEntity(x, index), () => RemoveGameEntity(x));
            });
        }
    }
}
