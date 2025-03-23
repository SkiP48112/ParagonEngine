using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Media.Media3D;


namespace Editor.Editors
{
    public partial class GeometryView : UserControl
    {
        private static readonly GeometryView _geometryView = 
            new GeometryView() { Background = (Brush)Application.Current.FindResource("Editor.Window.GrayBrush4") };
        
        private Point _clickPosition;
        private bool _isCapturedLeft;
        private bool _isCapturedRight;

        public GeometryView()
        {
            InitializeComponent();
            DataContextChanged += (s, e) => SetGeometry();
        }

        public void SetGeometry(int index = -1)
        {
            if(!(DataContext is MeshRenderer vm))
            {
                return;
            }

            // NOTE: if we already have a geom in our viewport we want to remove it
            //       at 0 position lies the light
            if(vm.Meshes.Any() && viewport.Children.Count == 2)
            {
                viewport.Children.RemoveAt(1);
            }

            var meshIndex = 0;
            var modelGroup = new Model3DGroup();
            foreach(var mesh in vm.Meshes)
            {
                // Skip over meshes that we don't want to display
                if(index != -1 && meshIndex != index)
                {
                    ++meshIndex;
                    continue;
                }

                var mesh3D = new MeshGeometry3D()
                {
                    Positions = mesh.Positions,
                    Normals = mesh.Normals,
                    TriangleIndices = mesh.Indices,
                    TextureCoordinates = mesh.UVs
                };

                var diffuse = new DiffuseMaterial(mesh.Diffuse);
                var specular = new SpecularMaterial(mesh.Specular, 50);
                var matGroup = new MaterialGroup();

                matGroup.Children.Add(diffuse);
                matGroup.Children.Add(specular);

                var model = new GeometryModel3D(mesh3D, matGroup);
                modelGroup.Children.Add(model);

                var binding = new Binding(nameof(mesh.Diffuse)) { Source = mesh};
                BindingOperations.SetBinding(diffuse, DiffuseMaterial.BrushProperty, binding);
            
                if(meshIndex == index)
                {
                    break;
                }
            }

            var visual = new ModelVisual3D { Content = modelGroup };
            viewport.Children.Add(visual);
        }

        private void OnGrid_MouseMove(object sender,MouseEventArgs e)
        {
            if(!_isCapturedLeft && !_isCapturedRight)
            {
                return;
            }

            var pos = e.GetPosition(this);
            var delta = pos - _clickPosition;

            if(_isCapturedLeft && !_isCapturedRight)
            {
                MoveCamera(delta.X, delta.Y, 0);
            }
            else if(!_isCapturedLeft && _isCapturedRight)
            {
                var vm = DataContext as MeshRenderer;
                var cp = vm.CameraPosition;

                var yOffset = delta.Y * 0.001 * Math.Sqrt(cp.X * cp.X + cp.Z * cp.Z);
                vm.CameraTarget = new Point3D(vm.CameraTarget.X, vm.CameraTarget.Y + yOffset, vm.CameraTarget.Z);
            }

            _clickPosition = pos;
        }

        private void OnGrid_MouseWheel(object sender, MouseWheelEventArgs e)
        {
            MoveCamera(0, 0, Math.Sign(e.Delta));
        }

        private void OnGrid_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            _clickPosition = e.GetPosition(this);
            _isCapturedLeft = true;

            Mouse.Capture(sender as UIElement);
        }

        private void OnGrid_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            _isCapturedLeft = false;
            if (!_isCapturedRight)
            {
                Mouse.Capture(null);
            }
        }

        private void OnGrid_MouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {
            _clickPosition = e.GetPosition(this);
            _isCapturedRight = true;

            Mouse.Capture(sender as UIElement);
        }

        private void OnGrid_MouseRightButtonUp(object sender, MouseButtonEventArgs e)
        {
            _isCapturedRight = false;
            if (!_isCapturedLeft)
            {
                Mouse.Capture(null);
            }
        }

        private void MoveCamera(double dx, double dy, int dz)
        {
            var viewModel = DataContext as MeshRenderer;
            var cameraPosition = new Vector3D(viewModel.CameraPosition.X, viewModel.CameraPosition.Y, viewModel.CameraPosition.Z);

            var radialDistance = cameraPosition.Length;
            var polarAngle = Math.Acos(cameraPosition.Y / radialDistance);
            var azimuthalAngle = Math.Atan2(-cameraPosition.Z, cameraPosition.X);

            polarAngle -= dy * 0.01;
            azimuthalAngle -= dx * 0.01;
            radialDistance *= 1.0 - 0.1 * dz;

            polarAngle = Math.Clamp(polarAngle, 0.0001, Math.PI - 0.0001);

            cameraPosition.X = radialDistance * Math.Sin(polarAngle) * Math.Cos(azimuthalAngle);
            cameraPosition.Z = -radialDistance * Math.Sin(polarAngle) * Math.Sin(azimuthalAngle);
            cameraPosition.Y = radialDistance * Math.Cos(polarAngle);

            viewModel.CameraPosition = new Point3D(cameraPosition.X, cameraPosition.Y, cameraPosition.Z);
        }

        internal static BitmapSource RenderToBitmap(MeshRenderer mesh, int width, int height)
        {
            var bmp = new RenderTargetBitmap(width, height, 96, 96, PixelFormats.Default);

            _geometryView.DataContext = mesh;
            _geometryView.Width = width;
            _geometryView.Height = height;
            _geometryView.Measure(new Size(width, height));
            _geometryView.Arrange(new Rect(0, 0, width, height));
            _geometryView.UpdateLayout();

            bmp.Render(_geometryView);
            return bmp;
        }
    }
}
