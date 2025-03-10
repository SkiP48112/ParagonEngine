using System.Diagnostics;

namespace Editor.Assets
{
    enum AssetType
    {
        Unknown,
        Animation,
        Audio,
        Material,
        Mesh,
        Skeleton,
        Texture
    }

    abstract class Asset : ViewModelBase
    {
        public AssetType Type { get; private set; }
    
        public Asset(AssetType type )
        {
            Debug.Assert(type != AssetType.Unknown, $"Can't create asset with unknown type: {nameof(type)}");
            Type = type;
        }
    }
}
