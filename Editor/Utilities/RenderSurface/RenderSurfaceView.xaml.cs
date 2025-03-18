using System.Diagnostics;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Interop;

namespace Editor.Utilities
{
    public enum Win32Msg
    {
        WM_SIZE = 0x0005,
        WM_SIZING = 0x0214,
        WM_ENTERSIZEMOVE = 0x0231,
        WM_EXITSIZEMOVE = 0x0232
    }

    public partial class RenderSurfaceView : UserControl, IDisposable
    {
        private RenderSurfaceHost _host = null;
        private bool _disposedValue;

        public RenderSurfaceView()
        {
            InitializeComponent();
            Loaded += OnRenderSurfaceViewLoaded;
        }

        private void OnRenderSurfaceViewLoaded(object sender, RoutedEventArgs e)
        {
            Loaded -= OnRenderSurfaceViewLoaded;

            _host = new RenderSurfaceHost(ActualWidth, ActualHeight);
            _host.MessageHook += new HwndSourceHook(HostMessageFilter);
            Content = _host;
        }

        private IntPtr HostMessageFilter(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            switch ((Win32Msg)msg)
            {
                case Win32Msg.WM_SIZE:
                    break;
                case Win32Msg.WM_SIZING:
                    throw new Exception();
                case Win32Msg.WM_ENTERSIZEMOVE:
                    throw new Exception();
                case Win32Msg.WM_EXITSIZEMOVE:
                    throw new Exception();

                default:
                    break;
            }

            return IntPtr.Zero;
        }

        public void Dispose()
        {
            Dispose(disposing: true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!_disposedValue)
            {
                if (disposing)
                {
                    _host.Dispose();
                }

                _disposedValue = true;
            }
        }
    }
}