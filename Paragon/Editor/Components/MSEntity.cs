using Editor.Utilities;
using System.Collections.ObjectModel;
using System.Diagnostics;

namespace Editor.Components
{
    abstract class MSEntity : ViewModelBase
    {
        // Enables upadtes to selected entities
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

        public void Refresh()
        {
            _enableUpdates = false;
            UpdateMSGameEntity();
            _enableUpdates = true;
        }

        public static float? GetMixedValue(List<GameEntity> entities, Func<GameEntity, float> getProperty)
        {
            var value = getProperty(entities.First());
            foreach (var entity in entities.Skip(1))
            {
                if (!value.IsEqual(getProperty(entity)))
                {
                    return null;
                }
            }

            return value;
        }

        public static bool? GetMixedValue(List<GameEntity> entities, Func<GameEntity, bool> getProperty)
        {
            var value = getProperty(entities.First());
            foreach (var entity in entities.Skip(1))
            {
                if (value != getProperty(entity))
                {
                    return null;
                }
            }

            return value;
        }

        public static string? GetMixedValue(List<GameEntity> entities, Func<GameEntity, string> getProperty)
        {
            var value = getProperty(entities.First());
            foreach (var entity in entities.Skip(1))
            {
                if (value != getProperty(entity))
                {
                    return null;
                }
            }

            return value;
        }

        protected virtual bool UpdateGameEntities(string? propertyName)
        {
            switch (propertyName)
            {
                case nameof(IsEnabled):
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
