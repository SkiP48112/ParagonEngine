using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Input;
using System.Windows.Media.Media3D;


namespace Editor.Editors
{
    public partial class GeometryView : UserControl
    {
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

        private void OnGrid_MouseMove(object sender, System.Windows.Input.MouseEventArgs e)
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
            else if(_isCapturedLeft && _isCapturedRight)
            {
                var vm = DataContext as MeshRenderer;
                var cp = vm.CameraPosition;

                var yOffset = delta.Y * 0.001 * Math.Sqrt(cp.X * cp.X + cp.Z * cp.Z);
                vm.CameraTarget = new Point3D(vm.CameraTarget.X, vm.CameraTarget.Y + yOffset, vm.CameraTarget.Z);
            }

            _clickPosition = pos;
        }

        private void OnGrid_MouseWheel(object sender, System.Windows.Input.MouseWheelEventArgs e)
        {
            MoveCamera(0, 0, Math.Sign(e.Delta));
        }

        private void OnGrid_MouseLeftButtonDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            _clickPosition = e.GetPosition(this);
            _isCapturedLeft = true;

            Mouse.Capture(sender as UIElement);
        }

        private void OnGrid_MouseLeftButtonUp(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            _isCapturedLeft = false;
            if (!_isCapturedRight)
            {
                Mouse.Capture(null);
            }
        }

        private void OnGrid_MouseRightButtonDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            _clickPosition = e.GetPosition(this);
            _isCapturedRight = true;

            Mouse.Capture(sender as UIElement);
        }

        private void OnGrid_MouseRightButtonUp(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            _isCapturedRight = false;
            if (!_isCapturedLeft)
            {
                Mouse.Capture(null);
            }
        }

        private void MoveCamera(double dx, double dy, int dz)
        {
            var vm = DataContext as MeshRenderer;
            var v = new Vector3D(vm.CameraPosition.X, vm.CameraPosition.Y, vm.CameraPosition.Z);

            var r = v.Length;
            var theta = Math.Acos(v.Y / r);
            var phi = Math.Atan2(-v.Z, v.X);

            theta -= dy * 0.01;
            phi -= dx * 0.01;
            r *= 1.0 - 0.1 * dz;

            theta = Math.Clamp(theta, 0.0001, Math.PI - 0.0001);

            v.X = r * Math.Sin(theta) * Math.Cos(phi);
            v.Z = -r * Math.Sin(theta) * Math.Sin(phi);
            v.Y = r * Math.Cos(theta);

            vm.CameraPosition = new Point3D(v.X, v.Y, v.Z);
        }
    }
}
