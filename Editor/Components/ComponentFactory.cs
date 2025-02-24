using System.Diagnostics;

namespace Editor.Components
{
    enum ComponentType
    {
        Transform,
        Script
    }

    static class ComponentFactory
    {
        private static readonly Func<GameEntity, object, Component>[] _functions =
            new Func<GameEntity, object, Component>[]
            {
                (entity, data) => new Transform(entity),
                (entity, data) => new Script(entity) { Name = (string) data}
            };

        public static Func<GameEntity, object, Component> GetCreationFunction(ComponentType componentType)
        {
            Debug.Assert((int)componentType < _functions.Length);
            return _functions[(int)componentType];
        }
    }
}
