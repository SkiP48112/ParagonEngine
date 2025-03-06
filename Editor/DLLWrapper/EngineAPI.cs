using Editor.Components;
using Editor.EngineAPIStructs;
using Editor.GameProject;
using Editor.Utilities;
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
    class ScriptComponent
    {
        public IntPtr ScriptCreator;
    }

    [StructLayout(LayoutKind.Sequential)]
    class GameEntityDesc
    {
        public TransformComponent Transform = new TransformComponent();
        public ScriptComponent Script = new ScriptComponent();
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

        [DllImport(_engineDll)]
        public static extern IntPtr GetScriptCreator(string name);

        [DllImport(_engineDll)]
        [return: MarshalAs(UnmanagedType.SafeArray)]
        public static extern string[] GetScriptNames();

        [DllImport(_engineDll)]
        public static extern int CreateRenderSurface(IntPtr host, int width, int height);

        [DllImport(_engineDll)]
        public static extern void RemoveRenderSurface(int idx);

        [DllImport(_engineDll)]
        public static extern IntPtr GetWindowHandle(int idx);

        [DllImport(_engineDll)]
        public static extern IntPtr ResizeRenderSurface(int idx);

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

                //script
                {
                    // NOTE: here we also check if currnt project is not null, so we can tell whether the game code dll
                    //       has been loaded or not.
                    var scriptComponent = entity.GetComponent<Script>();
                    if(scriptComponent != null && Project.CurrentGameProject != null)
                    {
                        if (Project.CurrentGameProject.AvailableScripts!.Contains(scriptComponent.Name))
                        {
                            desc.Script.ScriptCreator = GetScriptCreator(scriptComponent.Name!);
                        }
                        else
                        {
                            Logger.Log(MessageType.Error, $"Unable to find script with name {scriptComponent.Name}. Game entity will be created without script component.");
                        }
                    }
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
