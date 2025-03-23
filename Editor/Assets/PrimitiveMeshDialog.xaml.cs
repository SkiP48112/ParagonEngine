using Editor.AssetToolsAPIStructs;
using Editor.DLLWrapper;
using Editor.Editors;
using Editor.GameProject;
using Editor.Utilities.Controls;
using Microsoft.Win32;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Imaging;


namespace Editor.Assets
{
    public partial class PrimitiveMeshDialog : Window
    {
        static PrimitiveMeshDialog()
        {
            LoadTextures();
        }

        public PrimitiveMeshDialog()
        {
            InitializeComponent();
            Loaded += (s, e) => UpdatePrimitive();
        }

        private static readonly List<ImageBrush> _textures = new List<ImageBrush>();

        private static void LoadTextures()
        {
            var uris = new List<Uri>
            {
                new Uri("pack://application:,,,/Resources/PrimitiveMeshView/uvs_texture_8.png"),
                new Uri("pack://application:,,,/Resources/PrimitiveMeshView/uvs_texture_6.png"),
                new Uri("pack://application:,,,/Resources/PrimitiveMeshView/uvs_texture_combined.png")
            };

            _textures.Clear();
            foreach(var uri in uris)
            {
                var resource = Application.GetResourceStream(uri);
                using var reader = new BinaryReader(resource.Stream);
                var data = reader.ReadBytes((int)resource.Stream.Length);

                var imageSource = (BitmapSource)new ImageSourceConverter().ConvertFrom(data);
                imageSource.Freeze();

                var brush = new ImageBrush(imageSource);
                brush.Transform = new ScaleTransform(1, -1, 0.5, 0.5);
                brush.ViewportUnits = BrushMappingMode.Absolute;
                brush.Freeze();

                _textures.Add(brush);
            }

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
            float.TryParse(scalarBox.Value, CultureInfo.InvariantCulture, out var result);
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

        private PrimitiveInitInfo GetUvSphereInitInfo()
        {
            var info = new PrimitiveInitInfo() { Type = PrimitiveMeshType.UvSphere };

            info.SegmentX = (int)xSliderUvSphere.Value;
            info.SegmentY = (int)ySliderUvSphere.Value;

            info.Size.X = GetScalarBoxValue(xScalarBoxUvSphere, 0.001f);
            info.Size.Y = GetScalarBoxValue(yScalarBoxUvSphere, 0.001f);
            info.Size.Z = GetScalarBoxValue(zScalarBoxUvSphere, 0.001f);

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
            var smoothingAngle = 0;

            switch (primitiveType)
            {
                case PrimitiveMeshType.Plane:
                     info = GetPlaneInitInfo();
                     break;
                case PrimitiveMeshType.Cube:
                    return;
                case PrimitiveMeshType.UvSphere:
                    info = GetUvSphereInitInfo();
                    smoothingAngle = (int)angleSliderUvSphere.Value;
                    break;
                case PrimitiveMeshType.IcoSphere:
                    return;
                case PrimitiveMeshType.Cylinder:
                    return;
                case PrimitiveMeshType.Capsule:
                    return;
                default:
                    break;
            }

            var geometry = new Geometry();
            geometry.ImportSettings.SmoothingAngle = smoothingAngle;

            AssetToolsAPI.CreatePrimitiveMesh(geometry, info);

            var geometryEditor = (GeometryEditor)DataContext;
            geometryEditor.SetAsset(geometry);

            OnTextureComboBox_Click(textureCheckBox, null);
        }

        private void OnTextureComboBox_Click(object sender, RoutedEventArgs e)
        {
            Brush brush = Brushes.White;
            if((sender as CheckBox).IsChecked == true)
            {
                brush = _textures[(int)primTypeComboBox.SelectedItem];
            }

            var vm = DataContext as GeometryEditor;
            foreach(var mesh in vm.MeshRenderer.Meshes)
            {
                mesh.Diffuse = brush;
            }
        }

        private void OnSaveButton_Click(object sender, RoutedEventArgs e)
        {
            var saveDialog = new SaveFileDialog()
            {
                InitialDirectory = Project.CurrentGameProject.ContentPath,
                Filter = "Asset file (*.asset)|*.asset"
            };

            if(saveDialog.ShowDialog() == true)
            {
                Debug.Assert(string.IsNullOrEmpty(saveDialog.FileName), "Can't save asset file with invalid name.");

                var asset = (DataContext as IAssetEditor).Asset;
                Debug.Assert(asset != null, "Can't find asset in asset editor.");

                asset.Save(saveDialog.FileName);
            }
        }
    }
}
