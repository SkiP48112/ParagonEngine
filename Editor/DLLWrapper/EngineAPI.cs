using Editor.Components;
using Editor.EngineAPIStructs;
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
        private const string _dllName = "EngineDLL.dll";

        [DllImport(_dllName)]
        private static extern int CreateGameEntity(GameEntityDesc desc);
        public static int CreateGameEntity(GameEntity entity)
        {
            GameEntityDesc desc = new GameEntityDesc();

            // transform
            {
                var component = entity.GetComponent<Transform>();
                desc.Transform.Position = component.Position;
                desc.Transform.Rotation = component.Rotation;
                desc.Transform.Scale = component.Scale;
            }

            return CreateGameEntity(desc);
        }

        [DllImport(_dllName)]
        private static extern int RemoveGameEntity(int id);
        public static void RemoveGameEntity(GameEntity entity)
        {
            RemoveGameEntity(entity.EntityId);
        }
    }
}
