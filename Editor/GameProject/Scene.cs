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

        public ICommand? AddGameEntityCommand { get; private set; }
        public ICommand? RemoveGameEntityCommand { get; private set; }

        [DataMember(Name = nameof(GameEntities))]
        private readonly ObservableCollection<GameEntity> _gameEntities = new ObservableCollection<GameEntity>();
        public ReadOnlyObservableCollection<GameEntity>? GameEntities { get; private set; }

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

            Debug.Assert(GameEntities != null);
            foreach(var entity in GameEntities)
            {
                entity.IsActive = IsActive;
            }

            AddGameEntityCommand = new RelayCommand<GameEntity>(gameEntity =>
            {
                Debug.Assert(gameEntity != null, "Can't add game entity cause it's null.");

                AddGameEntity(gameEntity);
                var index = GameEntities.Count - 1;
                Project.AddNewUndoRedoAction($"Add {gameEntity.Name} to {Name}", () => RemoveGameEntity(gameEntity), () => AddGameEntity(gameEntity, index));
            });

            RemoveGameEntityCommand = new RelayCommand<GameEntity>(gameEntity =>
            {
                Debug.Assert(gameEntity != null, "Can't remove game entity because it's null");
                var index = GameEntities.IndexOf(gameEntity);

                RemoveGameEntity(gameEntity);
                Project.AddNewUndoRedoAction($"Remove {gameEntity.Name} from {Name}", () => AddGameEntity(gameEntity, index), () => RemoveGameEntity(gameEntity));
            });
        }
    }
}
