using Editor.DLLWrapper;
using Editor.GameProject;
using Editor.Utilities;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Runtime.Serialization;

namespace Editor.Components
{
    [DataContract]
    [KnownType(typeof(Transform))]
    [KnownType(typeof(Script))]
    class GameEntity : ViewModelBase
    {
        private bool _isEnabled = true;
        [DataMember]
        public bool IsEnabled
        {
            get => _isEnabled;
            set
            {
                if (_isEnabled != value)
                {
                    _isEnabled = value;
                    OnPropertyChanged(nameof(IsEnabled));
                }
            }
        }

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
        public Scene ParentScene { get; set; }

        [DataMember(Name = nameof(Components))]
        private readonly ObservableCollection<Component> _components = new ObservableCollection<Component>();
        public ReadOnlyObservableCollection<Component> Components { get; private set; }

        private int _entityId = ID.INVALID_ID;
        public int EntityId
        {
            get => _entityId;
            set
            {
                if (_entityId != value)
                {
                    _entityId = value;
                    OnPropertyChanged(nameof(EntityId));
                }
            }
        }

        private bool _isActive;
        public bool IsActive
        {
            get => _isActive;
            set
            {
                if (_isActive != value)
                {
                    _isActive = value;
                    if (_isActive)
                    {
                        EntityId = EngineAPI.EntityAPI.CreateGameEntity(this);
                        Debug.Assert(ID.IsValid(EntityId));
                    }
                    else if(ID.IsValid(EntityId))
                    {
                        EngineAPI.EntityAPI.RemoveGameEntity(this);
                        EntityId = ID.INVALID_ID;
                    }

                    OnPropertyChanged(nameof(IsActive));
                }
            }
        }

        public GameEntity(Scene scene)
        {
            Debug.Assert(scene != null);
            ParentScene = scene;

            _components.Add(new Transform(this));
            OnDeserialized(new StreamingContext());
        }

        public Component GetComponent(Type type) => Components?.FirstOrDefault(x => x.GetType() == type);
        public T GetComponent<T>() where T : Component => GetComponent(typeof(T)) as T;

        public bool AddComponent(Component component)
        {
            Debug.Assert(component != null, "Can't add null component.");
            if(_components.Any(x => x.GetType() == component.GetType()))
            {
                Logger.Log(MessageType.Warning, $"Entity {Name} already has a component of type {component.GetType().Name}");
                return false;
            }

            AddComponentInternal(component);
            return true;
        }

        public void RemoveComponent(Component component)
        {
            Debug.Assert(component != null, "Can't remove null component.");
            if(component is Transform)
            {
                Logger.Log(MessageType.Warning, $"Can't remove transform component from {Name}.");
                return;
            }

            if (!_components.Contains(component))
            {
                Logger.Log(MessageType.Warning, $"{Name} doesn't contains {component.GetType().Name} component.");
                return;
            }

            RemoveComponentInternal(component);
        }

        private void AddComponentInternal(Component component)
        {
            IsActive = false;
            _components.Add(component);
            IsActive = true;
        }

        private void RemoveComponentInternal(Component component)
        {
            IsActive = false;
            _components.Remove(component);
            IsActive = true;
        }

        [OnDeserialized]
        private void OnDeserialized(StreamingContext context)
        {
            if (_components != null)
            {
                Components = new ReadOnlyObservableCollection<Component>(_components);
                OnPropertyChanged(nameof(Components));
            }
        }
    }
}
