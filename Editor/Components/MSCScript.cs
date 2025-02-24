namespace Editor.Components
{
    sealed class MSCScript : MSComponent<Script>
    {
        private string? _name = string.Empty;
        public string? Name
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

        public MSCScript(MSEntity msEntity) : base(msEntity)
        {
            Refresh();
        }

        protected override bool UpdateComponents(string? propertyName)
        {
            if(propertyName == nameof(Name))
            {
                SelectedComponents.ForEach(component => component.Name = _name);
                return true;
            }

            return false;
        }

        protected override bool UpdateMSComponent()
        {
            Name = MSEntity.GetMixedValue(SelectedComponents, new Func<Script, string>(x => x.Name));
            return true;
        }
    }
}
