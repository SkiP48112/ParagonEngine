using Editor.DLLWrapper;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Windows.Interop;

namespace Editor.Utilities
{
    class RenderSurfaceHost : HwndHost
    {
        public int SurfaceID { get; private set; } = ID.INVALID_ID;

        private IntPtr _renderWindowHandle = IntPtr.Zero;
        private readonly int _width = 800;
        private readonly int _height = 800;
        private readonly int VK_LBUTTON = 0x01;

        private DelayEventTimer _resizeDelayTimer;

        [DllImport("user32.dll")]
        private static extern short GetAsyncKeyState(int vKey);

        public RenderSurfaceHost(double width, double height)
        {
            _width = (int)width;
            _height = (int)height;
            _resizeDelayTimer = new DelayEventTimer(TimeSpan.FromMilliseconds(250.0));
            _resizeDelayTimer.Triggered += Resize;

            SizeChanged += (s, e) => _resizeDelayTimer.Trigger();
        }

        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            SurfaceID = EngineAPI.CreateRenderSurface(hwndParent.Handle, _width, _height);
            Debug.Assert(ID.IsValid(SurfaceID), "Can't create render surface for a reason.");

            _renderWindowHandle = EngineAPI.GetWindowHandle(SurfaceID);
            Debug.Assert(_renderWindowHandle != IntPtr.Zero, $"Can't get window handle by surface id = {SurfaceID}");

            return new HandleRef(this, _renderWindowHandle);
        }

        protected override void DestroyWindowCore(HandleRef hwnd)
        {
            EngineAPI.RemoveRenderSurface(SurfaceID);

            SurfaceID = ID.INVALID_ID;
            _renderWindowHandle = IntPtr.Zero;
        }

        private void Resize(object sender, DelayEventTimerArgs e)
        {
            e.RepeatEvent = GetAsyncKeyState(VK_LBUTTON) < 0;
            if (!e.RepeatEvent)
            {
                EngineAPI.ResizeRenderSurface(SurfaceID);
            }
        }
    }
}