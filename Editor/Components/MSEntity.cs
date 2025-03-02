using Editor.Utilities;
using System.Collections.ObjectModel;
using System.Diagnostics;

namespace Editor.Components
{
    abstract class MSEntity : ViewModelBase
    {
        // Enables upadtes to selected objects
        private bool _enableUpdates = true;

        private bool? _isEnabled = true;
        public bool? IsEnabled
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

        private readonly ObservableCollection<IMSComponent> _components = new ObservableCollection<IMSComponent>();
        public ReadOnlyObservableCollection<IMSComponent> Components { get; }

        public List<GameEntity> SelectedEntities { get; }

        public MSEntity(List<GameEntity> entities)
        {
            Debug.Assert(entities?.Any() == true);
            Components = new ReadOnlyObservableCollection<IMSComponent>(_components);
            SelectedEntities = entities;

            PropertyChanged += (s, e) =>
            {
                if (!_enableUpdates)
                {
                    return;
                }

                UpdateGameEntities(e.PropertyName);
            };
        }

        public T GetMSComponent<T>() where T : IMSComponent
        {
            return (T)Components.FirstOrDefault(x => x.GetType() == typeof(T));
        }

        public void Refresh()
        {
            _enableUpdates = false;
            UpdateMSGameEntity();
            MakeComponentList();
            _enableUpdates = true;
        }

        private void MakeComponentList()
        {
            _components.Clear();
            var firstEntity = SelectedEntities.FirstOrDefault();
            if(firstEntity == null || firstEntity.Components == null)
            {
                return;
            }

            foreach(var component in firstEntity.Components)
            {
                var type = component.GetType();
                if(!SelectedEntities.Skip(1).Any(entity => entity.GetComponent(type) == null))
                {
                    Debug.Assert(Components.FirstOrDefault(x => x.GetType() == type) == null);
                    _components.Add(component.GetMultiselectionComponent(this));
                }
            }
        }

        public static float? GetMixedValue<T>(List<T> objects, Func<T, float> getProperty)
        {
            var value = getProperty(objects.First());
            foreach (var obj in objects.Skip(1))
            {
                if (!value.IsEqual(getProperty(obj)))
                {
                    return null;
                }
            }

            return value;
        }

        public static bool? GetMixedValue<T>(List<T> objects, Func<T, bool> getProperty)
        {
            var value = getProperty(objects.First());
            foreach (var obj in objects.Skip(1))
            {
                if (value != getProperty(obj))
                {
                    return null;
                }
            }

            return value;
        }

        public static string GetMixedValue<T>(List<T> objects, Func<T, string> getProperty)
        {
            var value = getProperty(objects.First());
            foreach (var obj in objects.Skip(1))
            {
                if (value != getProperty(obj))
                {
                    return null;
                }
            }

            return value;
        }

        protected virtual bool UpdateGameEntities(string propertyName)
        {
            switch (propertyName)
            {
                case nameof(IsEnabled):
                    Debug.Assert(IsEnabled != null);
                    SelectedEntities.ForEach(x => x.IsEnabled = IsEnabled.Value);
                    return true;
                case nameof(Name):
                    SelectedEntities.ForEach(x => x.Name = Name);
                    return true;
            }

            return false;
        }

        protected virtual bool UpdateMSGameEntity()
        {
            IsEnabled = GetMixedValue(SelectedEntities, new Func<GameEntity, bool>(x => x.IsEnabled));
            Name = GetMixedValue(SelectedEntities, new Func<GameEntity, string>(x => x.Name)) ?? string.Empty;

            return true;
        }
    }
}
