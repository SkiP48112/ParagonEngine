﻿using System.Diagnostics;

namespace Editor.Components
{
    abstract class MSComponent<T> : ViewModelBase, IMSComponent where T : Component
    {
        public List<T> SelectedComponents { get; }
        private bool _enableUpdates = true;

        public MSComponent(MSEntity msEntity)
        {
            Debug.Assert(msEntity.SelectedEntities.Any() == true);
            SelectedComponents = msEntity.SelectedEntities.Select(entity => entity.GetComponent<T>()).ToList()!;
            PropertyChanged += (s, e) =>
            {
                if (_enableUpdates)
                {
                    UpdateComponents(e.PropertyName);
                }
            };
        }

        public void Refresh()
        {
            _enableUpdates = false;
            UpdateMSComponent();
            _enableUpdates = true;
        }

        protected abstract bool UpdateComponents(string propertyName);
        protected abstract bool UpdateMSComponent();
    }
}
