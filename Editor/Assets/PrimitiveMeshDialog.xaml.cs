using Editor.AssetToolsAPIStructs;
using Editor.DLLWrapper;
using Editor.Editors;
using Editor.Utilities.Controls;
using System.Windows;
using System.Windows.Controls;


namespace Editor.Assets
{
    public partial class PrimitiveMeshDialog : Window
    {
        public PrimitiveMeshDialog()
        {
            InitializeComponent();
            Loaded += (s, e) => UpdatePrimitive();
        }

        private void OnPrimitiveTypeComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            UpdatePrimitive();
        }

        private void OnSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            UpdatePrimitive();
        }

        private void OnScalarBox_ValueChanged(object sender, RoutedEventArgs e)
        {
            UpdatePrimitive();
        }

        private float GetScalarBoxValue(ScalarBox scalarBox, float min)
        {
            float.TryParse(scalarBox.Value, out var result);
            return Math.Max(result, min);
        }

        private PrimitiveInitInfo GetPlaneInitInfo()
        {
            var info = new PrimitiveInitInfo() { Type = PrimitiveMeshType.Plane };

            info.SegmentX = (int)xSliderPlane.Value;
            info.SegmentZ = (int)zSliderPlane.Value;

            info.Size.X = GetScalarBoxValue(widthScalarBoxPlane, 0.001f);
            info.Size.Z = GetScalarBoxValue(lengthScalarBoxPlane, 0.001f);

            return info;
        }

        private void UpdatePrimitive()
        {
            if (!IsInitialized)
            {
                return;
            }

            var primitiveType = (PrimitiveMeshType)primTypeComboBox.SelectedItem;
            var info = new PrimitiveInitInfo();

            switch (primitiveType)
            {
                case PrimitiveMeshType.Plane:
                    {
                        info = GetPlaneInitInfo();
                        break;
                    }
                case PrimitiveMeshType.Cube:
                    break;
                case PrimitiveMeshType.UvSphere:
                    break;
                case PrimitiveMeshType.IcoSphere:
                    break;
                case PrimitiveMeshType.Cylinder:
                    break;
                case PrimitiveMeshType.Capsule:
                    break;
                default:
                    break;
            }

            var geometry = new Geometry();
            AssetToolsAPI.CreatePrimitiveMesh(geometry, info);

            var geometryEditor = (GeometryEditor)DataContext;
            geometryEditor.SetAsset(geometry);
        }
    }
}
