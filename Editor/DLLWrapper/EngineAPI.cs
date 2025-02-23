using Editor.Components;
using Editor.EngineAPIStructs;
using System.Diagnostics;
using System.Numerics;
using System.Runtime.InteropServices;

namespace Editor.EngineAPIStructs
{
    [StructLayout(LayoutKind.Sequential)]
    class TransformComponent
    {
        public Vector3 Position;
        public Vector3 Rotation;
        public Vector3 Scale = new Vector3(1, 1, 1);
    }

    [StructLayout(LayoutKind.Sequential)]
    class GameEntityDesc
    {
        public TransformComponent Transform = new TransformComponent();
    }
}

namespace Editor.DLLWrapper
{
    static class EngineAPI
    {
        private const string _engineDll = "EngineDLL.dll";

        [DllImport(_engineDll, CharSet = CharSet.Ansi)]
        public static extern int LoadGameCodeDll(string dllPath);

        [DllImport(_engineDll)]
        public static extern int UnloadGameCodeDll();

        internal static class EntityAPI
        {
            [DllImport(_engineDll)]
            private static extern int CreateGameEntity(GameEntityDesc desc);
            public static int CreateGameEntity(GameEntity entity)
            {
                GameEntityDesc desc = new GameEntityDesc();

                // transform
                {
                    var component = entity.GetComponent<Transform>();
                    Debug.Assert(component != null, $"Can't get {nameof(Transform)} from entity {entity.ToString()}");

                    desc.Transform.Position = component.Position;
                    desc.Transform.Rotation = component.Rotation;
                    desc.Transform.Scale = component.Scale;
                }

                return CreateGameEntity(desc);
            }

            [DllImport(_engineDll)]
            private static extern int RemoveGameEntity(int id);
            public static void RemoveGameEntity(GameEntity entity)
            {
                RemoveGameEntity(entity.EntityId);
            }
        }
    }
}
