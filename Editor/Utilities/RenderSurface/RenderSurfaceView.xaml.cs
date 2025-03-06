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
        private bool _canResize = true;
        private bool _isMoved = false;

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

            var window = this.FindVisualParent<Window>();
            Debug.Assert(window != null, "Can't find any window");

            var helper = new WindowInteropHelper(window);
            if (helper.Handle != IntPtr.Zero)
            {
                HwndSource.FromHwnd(helper.Handle)?.AddHook(HwndMessageHook);
            }
        }

        private IntPtr HostMessageFilter(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            switch ((Win32Msg)msg)
            {
                case Win32Msg.WM_SIZE:
                    if (_canResize)
                    {
                        _host.Resize();
                    }
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

        private nint HwndMessageHook(nint hwnd, int msg, nint wParam, nint lParam, ref bool handled)
        {
            switch ((Win32Msg)msg)
            {
                case Win32Msg.WM_SIZING:
                    _canResize = false;
                    _isMoved = false;
                    break;
                case Win32Msg.WM_ENTERSIZEMOVE:
                    _isMoved = true;
                    break;
                case Win32Msg.WM_EXITSIZEMOVE:
                    _canResize = true;
                    if (!_isMoved)
                    {
                        _host.Resize();
                    }
                    break;
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