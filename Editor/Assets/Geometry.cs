﻿
using Editor.Common.Helpers;
using Editor.Editors;
using Editor.Utilities;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using System.Text;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace Editor.Assets
{
    enum PrimitiveMeshType
    {
        Plane,
        Cube,
        UvSphere,
        IcoSphere,
        Cylinder,
        Capsule
    }

    class Mesh : ViewModelBase
    {
        private int _vertexSize;
        public int VertexSize
        {
            get => _vertexSize;
            set
            {
                if (_vertexSize != value)
                {
                    _vertexSize = value;
                    OnPropertyChanged(nameof(VertexSize));
                }
            }
        }

        private int _vertexCount;
        public int VertexCount
        {
            get => _vertexCount;
            set
            {
                if (_vertexCount != value)
                {
                    _vertexCount = value;
                    OnPropertyChanged(nameof(VertexCount));
                }
            }
        }

        private int _indexSize;
        public int IndexSize
        {
            get => _indexSize;
            set
            {
                if (_indexSize != value)
                {
                    _indexSize = value;
                    OnPropertyChanged(nameof(IndexSize));
                }
            }
        }

        private int _indexCount;
        public int IndexCount
        {
            get => _indexCount;
            set
            {
                if (_indexCount != value)
                {
                    _indexCount = value;
                    OnPropertyChanged(nameof(IndexCount));
                }
            }
        }

        public byte[] Vertices { get; set; }
        public byte[] Indices { get; set; }
    }

    class MeshLOD : ViewModelBase
    {
        private string _name = string.Empty;
        public string Name
        {
            get => _name;
            set
            {
                if (_name != value)
                {
                    _name = value;
                    OnPropertyChanged(nameof(Name));
                }
            }
        }

        private float _lodThreshold;
        public float LodThreshold
        {
            get => _lodThreshold;
            set
            {
                if (_lodThreshold != value)
                {
                    _lodThreshold = value;
                    OnPropertyChanged(nameof(LodThreshold));
                }
            }
        }

        public ObservableCollection<Mesh> Meshes { get; } = new ObservableCollection<Mesh>();
    }

    class LODGroup : ViewModelBase
    {
        private string _name = string.Empty;
        public string Name
        {
            get => _name;
            set
            {
                if (_name != value)
                {
                    _name = value;
                    OnPropertyChanged(nameof(Name));
                }
            }
        }

        public ObservableCollection<MeshLOD> LODs { get; } = new ObservableCollection<MeshLOD>();
    }

    class GeometryImportSettings : ViewModelBase
    {
        private float _smoothingAngle;
        public float SmoothingAngle
        {
            get => _smoothingAngle;
            set
            {
                if (_smoothingAngle != value)
                {
                    _smoothingAngle = value;
                    OnPropertyChanged(nameof(SmoothingAngle));
                }
            }
        }

        private bool _calculateNormals;
        public bool CalculateNormals
        {
            get => _calculateNormals;
            set
            {
                if(_calculateNormals != value)
                {
                    _calculateNormals = value;
                    OnPropertyChanged(nameof(CalculateNormals));
                }
            }
        }

        private bool _calculateTangents;
        public bool CalculateTangents
        {
            get => _calculateTangents;
            set
            {
                if (_calculateTangents != value)
                {
                    _calculateTangents = value;
                    OnPropertyChanged(nameof(CalculateTangents));
                }
            }
        }

        private bool _reverseHandedness;
        public bool ReverseHandedness
        {
            get => _reverseHandedness;
            set
            {
                if (_reverseHandedness != value)
                {
                    _reverseHandedness = value;
                    OnPropertyChanged(nameof(ReverseHandedness));
                }
            }
        }

        private bool _importEmbadedTextures;
        public bool ImportEmbadedTextures
        {
            get => _importEmbadedTextures;
            set
            {
                if (_importEmbadedTextures != value)
                {
                    _importEmbadedTextures = value;
                    OnPropertyChanged(nameof(ImportEmbadedTextures));
                }
            }
        }

        private bool _importAnimations;
        public bool ImportAnimations
        {
            get => _importAnimations;
            set
            {
                if (_importAnimations != value)
                {
                    _importAnimations = value;
                    OnPropertyChanged(nameof(ImportAnimations));
                }
            }
        }

        public GeometryImportSettings()
        {
            _smoothingAngle = 178.0f;
            _calculateNormals = false;
            _calculateTangents = false;
            _reverseHandedness = false;
            _importEmbadedTextures = true;
            _importAnimations = true;
        }

        public void ToBinary(BinaryWriter writer)
        {
            writer.Write(SmoothingAngle);
            writer.Write(CalculateNormals);
            writer.Write(CalculateTangents);
            writer.Write(ReverseHandedness);
            writer.Write(ImportEmbadedTextures);
            writer.Write(ImportAnimations);
        }
    }

    class Geometry : Asset
    {
        private readonly List<LODGroup> _lodGroups = new List<LODGroup>();
        public GeometryImportSettings ImportSettings { get; } = new GeometryImportSettings();

        public LODGroup GetLODGroup(int lodGroup = 0)
        {
            Debug.Assert(lodGroup >= 0 && lodGroup < _lodGroups.Count, $"Can't find lod group with index = {lodGroup}.");
            return _lodGroups.Any() ? _lodGroups[lodGroup] : null;
        }

        public Geometry() : base(AssetType.Mesh)
        {
        }

        public void FromRawData(byte[] data)
        {
            Debug.Assert(data?.Length > 0, "Can't read from geometry data due it's length is less or equals zero.");
            _lodGroups.Clear();

            using var reader = new BinaryReader(new MemoryStream(data));
            
            // skip scene name string
            var s = reader.ReadInt32();
            reader.BaseStream.Position += s;

            // get number of LODs
            var numLODGroups = reader.ReadInt32();
            Debug.Assert(numLODGroups > 0, "Can't read amount of LODs");

            for (int i = 0; i < numLODGroups; ++i)
            {
                // get LOD group's name
                s = reader.ReadInt32();
                string lodGroupName = string.Empty;

                if (s > 0)
                {
                    var nameBytes = reader.ReadBytes(s);
                    lodGroupName = Encoding.UTF8.GetString(nameBytes);
                }
                else
                {
                    lodGroupName = $"lod_{ AssetHelper.GetRandomString() }";
                }

                // get number of meshes in this LOD group
                var numMeshes = reader.ReadInt32();
                Debug.Assert(numMeshes > 0, "Can't read amount of meshes from asset data.");

                var lods = ReadMeshLODs(numMeshes, reader);
                var lodGroup = new LODGroup() { Name = lodGroupName };
                lods.ForEach(l => lodGroup.LODs.Add(l));

                _lodGroups.Add(lodGroup);
            }
        }

        private static List<MeshLOD> ReadMeshLODs(int numMeshes, BinaryReader reader)
        {
            var lodIds = new List<int>();
            var lodList = new List<MeshLOD>();

            for (int i = 0; i < numMeshes; ++i)
            {
                ReadMeshes(reader, lodIds, lodList);
            }

            return lodList;
        }

        private static void ReadMeshes(BinaryReader reader, List<int> lodIds, List<MeshLOD> lodList)
        {
            // get mesh name
            var s = reader.ReadInt32();
            string meshName = string.Empty;

            if (s > 0)
            {
                var nameBytes = reader.ReadBytes(s);
                meshName = Encoding.UTF8.GetString(nameBytes);
            }
            else
            {
                meshName = $"mesh_{AssetHelper.GetRandomString()}";
            }

            var mesh = new Mesh();

            var lodId = reader.ReadInt32();

            mesh.VertexSize = reader.ReadInt32();
            mesh.VertexCount = reader.ReadInt32();

            mesh.IndexSize = reader.ReadInt32();
            mesh.IndexCount = reader.ReadInt32();

            var lodThreshold = reader.ReadSingle();
            var vertexBufferSize = mesh.VertexSize * mesh.VertexCount;
            var indexBufferSize = mesh.IndexSize * mesh.IndexCount;

            mesh.Vertices = reader.ReadBytes(vertexBufferSize);
            mesh.Indices = reader.ReadBytes(indexBufferSize);

            MeshLOD lod;
            if(ID.IsValid(lodId) && lodIds.Contains(lodId))
            {
                lod = lodList[lodIds.IndexOf(lodId)];
                Debug.Assert(lod != null);
            }
            else
            {
                lodIds.Add(lodId);
                lod = new MeshLOD() { Name = meshName, LodThreshold = lodThreshold };
                lodList.Add(lod);
            }

            lod.Meshes.Add(mesh);
        }

        public override IEnumerable<string> Save(string file)
        {
            Debug.Assert(_lodGroups.Any(), "Can't save geometry without any lod groups.");
            
            var savedFiles = new List<string>();
            if (!_lodGroups.Any())
            {
                return savedFiles;
            }

            var path = Path.GetDirectoryName(file) + Path.DirectorySeparatorChar;
            var fileName = Path.GetFileNameWithoutExtension(file);

            try
            {
                foreach(var lodGroup in _lodGroups)
                {
                    Debug.Assert(lodGroup.LODs.Any(), "Can't save lod group with no LODs in it.");

                    var meshFileName = AssetHelper.SanitizeFileName(path + fileName + "_" + lodGroup.LODs[0].Name + ASSET_FILE_EXTENSION);
                    Guid = Guid.NewGuid();

                    byte[] data = null;
                    using (var writer = new BinaryWriter(new MemoryStream()))
                    {
                        writer.Write(lodGroup.Name);
                        writer.Write(lodGroup.LODs.Count);

                        var hashes = new List<byte>();
                        foreach(var lod in lodGroup.LODs)
                        {
                            LODToBinary(lod, writer, out var hash);
                            hashes.AddRange(hash);
                        }

                        Hash = AssetHelper.ComputeHash(hashes.ToArray());
                        data = (writer.BaseStream as MemoryStream).ToArray();
                        Icon = GenerateIcon(lodGroup.LODs[0]);
                    }

                    Debug.Assert(data?.Length > 0);
                    using (var writer = new BinaryWriter(File.Open(meshFileName, FileMode.Create, FileAccess.Write)))
                    {
                        WriteAssetFileHeader(writer);
                        ImportSettings.ToBinary(writer);
                        
                        writer.Write(data.Length);
                        writer.Write(data);
                    }

                    savedFiles.Add(meshFileName);
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
                Logger.Log(MessageType.Error, $"Failed to save geometry to {file}");
            }

            return savedFiles;
        }

        private byte[] GenerateIcon(MeshLOD lod)
        {
            var width = 90 * 4;
            BitmapSource bmp = null;


            // NOTE: It's not good practice to use a WPF control in the ViewModel.
            //       But we need to make an exception for this case, for as long as we don't
            //       have a graphics renderer that we can use for screenshot.
            Application.Current.Dispatcher.Invoke(() =>
            {
                bmp = GeometryView.RenderToBitmap(new MeshRenderer(lod, null), width, width);
                bmp = new TransformedBitmap(bmp, new ScaleTransform(0.25, 0.25, 0.5, 0.5));
            });

            using var memStream = new MemoryStream();
            memStream.SetLength(0);

            var encoder = new PngBitmapEncoder();
            encoder.Frames.Add(BitmapFrame.Create(bmp));
            encoder.Save(memStream);

            return memStream.ToArray();
        }

        private void LODToBinary(MeshLOD lod, BinaryWriter writer, out byte[] hash)
        {
            writer.Write(lod.Name);
            writer.Write(lod.LodThreshold);
            writer.Write(lod.Meshes.Count);

            var meshDataBegin = writer.BaseStream.Position;
            foreach (var mesh in lod.Meshes)
            {
                writer.Write(mesh.VertexSize);
                writer.Write(mesh.VertexCount);
                writer.Write(mesh.IndexSize);
                writer.Write(mesh.IndexCount);
                writer.Write(mesh.Vertices);
                writer.Write(mesh.Indices);
            }

            var meshDataSize = writer.BaseStream.Position - meshDataBegin;
            Debug.Assert(meshDataSize > 0);

            var buffer = (writer.BaseStream as MemoryStream).ToArray();
            hash = AssetHelper.ComputeHash(buffer, (int)meshDataBegin, (int)meshDataSize);
        }
    }
}
