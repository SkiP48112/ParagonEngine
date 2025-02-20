using Editor.Utilities;
using System.Numerics;
using System.Runtime.Serialization;

namespace Editor.Components
{
    sealed class MSCTransform : MSComponent<Transform>
    {
        private float? _positionX;
        public float? PositionX
        {
            get => _positionX;
            set
            {
                if (!_positionX.IsEqual(value))
                {
                    _positionX = value;
                    OnPropertyChanged(nameof(PositionX));
                }
            }
        }

        private float? _positionY;
        public float? PositionY
        {
            get => _positionY;
            set
            {
                if (!_positionY.IsEqual(value))
                {
                    _positionY = value;
                    OnPropertyChanged(nameof(PositionY));
                }
            }
        }

        private float? _positionZ;
        public float? PositionZ
        {
            get => _positionZ;
            set
            {
                if (!_positionZ.IsEqual(value))
                {
                    _positionZ = value;
                    OnPropertyChanged(nameof(PositionZ));
                }
            }
        }

        private float? _rotationX;
        public float? RotationX
        {
            get => _rotationX;
            set
            {
                if (!_rotationX.IsEqual(value))
                {
                    _rotationX = value;
                    OnPropertyChanged(nameof(RotationX));
                }
            }
        }

        private float? _rotationY;
        public float? RotationY
        {
            get => _rotationY;
            set
            {
                if (!_rotationY.IsEqual(value))
                {
                    _rotationY = value;
                    OnPropertyChanged(nameof(RotationY));
                }
            }
        }

        private float? _rotationZ;
        public float? RotationZ
        {
            get => _rotationZ;
            set
            {
                if (!_rotationZ.IsEqual(value))
                {
                    _rotationZ = value;
                    OnPropertyChanged(nameof(RotationZ));
                }
            }
        }

        private float? _scaleX;
        public float? ScaleX
        {
            get => _scaleX;
            set
            {
                if (!_scaleX.IsEqual(value))
                {
                    _scaleX = value;
                    OnPropertyChanged(nameof(ScaleX));
                }
            }
        }

        private float? _scaleY;
        public float? ScaleY
        {
            get => _scaleY;
            set
            {
                if (!_scaleY.IsEqual(value))
                {
                    _scaleY = value;
                    OnPropertyChanged(nameof(ScaleY));
                }
            }
        }

        private float? _scaleZ;
        public float? ScaleZ
        {
            get => _scaleZ;
            set
            {
                if (!_scaleZ.IsEqual(value))
                {
                    _scaleZ = value;
                    OnPropertyChanged(nameof(ScaleZ));
                }
            }
        }

        public MSCTransform(MSEntity msEntity) : base(msEntity)
        {
            Refresh();
        }

        protected override bool UpdateComponents(string? propertyName)
        {
            switch (propertyName)
            {
                case nameof(PositionX):
                case nameof(PositionY):
                case nameof(PositionZ):
                    SelectedComponents.ForEach(component => component.Position = new Vector3(_positionX ?? component.Position.X, _positionY ?? component.Position.Y, _positionZ ?? component.Position.Z));
                    return true;

                case nameof(RotationX): 
                case nameof(RotationY):
                case nameof(RotationZ):
                    SelectedComponents.ForEach(component => component.Rotation = new Vector3(_rotationX ?? component.Rotation.X, _rotationY ?? component.Rotation.Y, _rotationZ ?? component.Rotation.Z));
                    return true;

                case nameof(ScaleX):
                case nameof(ScaleY):
                case nameof(ScaleZ):
                    SelectedComponents.ForEach(component => component.Scale = new Vector3(_scaleX ?? component.Scale.X, _scaleY ?? component.Scale.Y, _scaleZ ?? component.Scale.Z));
                    return true;
            }

            return false;
        }

        protected override bool UpdateMSComponent()
        {
            PositionX = MSEntity.GetMixedValue(SelectedComponents, new Func<Transform, float>(x => x.Position.X));
            PositionY = MSEntity.GetMixedValue(SelectedComponents, new Func<Transform, float>(x => x.Position.Y));
            PositionZ = MSEntity.GetMixedValue(SelectedComponents, new Func<Transform, float>(x => x.Position.Z));

            RotationX = MSEntity.GetMixedValue(SelectedComponents, new Func<Transform, float>(x => x.Rotation.X));
            RotationY = MSEntity.GetMixedValue(SelectedComponents, new Func<Transform, float>(x => x.Rotation.Y));
            RotationZ = MSEntity.GetMixedValue(SelectedComponents, new Func<Transform, float>(x => x.Rotation.Z));

            ScaleX = MSEntity.GetMixedValue(SelectedComponents, new Func<Transform, float>(x => x.Scale.X));
            ScaleY = MSEntity.GetMixedValue(SelectedComponents, new Func<Transform, float>(x => x.Scale.Y));
            ScaleZ = MSEntity.GetMixedValue(SelectedComponents, new Func<Transform, float>(x => x.Scale.Z));

            return true;
        }
    }
}
