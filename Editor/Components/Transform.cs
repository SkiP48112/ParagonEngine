﻿using System.IO;
using System.Numerics;
using System.Runtime.Serialization;

namespace Editor.Components
{
    [DataContract]
    class Transform : Component
    {
        private Vector3 _position;
        [DataMember]
        public Vector3 Position
        {
            get => _position;
            set
            {
                if (_position != value)
                {
                    _position = value;
                    OnPropertyChanged(nameof(Position));
                }
            }
        }

        private Vector3 _rotation;
        [DataMember]
        public Vector3 Rotation
        {
            get => _rotation;
            set
            {
                if (_rotation != value)
                {
                    _rotation = value;
                    OnPropertyChanged(nameof(Rotation));
                }
            }
        }

        private Vector3 _scale;
        [DataMember]
        public Vector3 Scale
        {
            get => _scale;
            set
            {
                if (_scale != value)
                {
                    _scale = value;
                    OnPropertyChanged(nameof(Scale));
                }
            }
        }

        public Transform(GameEntity owner) : base(owner)
        {
        }

        public override IMSComponent GetMultiselectionComponent(MSEntity msEntity)
        {
            return new MSCTransform(msEntity);
        }

        public override void WriteToBinary(BinaryWriter bw)
        {
            bw.Write(_position.X);
            bw.Write(_position.Y);
            bw.Write(_position.Z);

            bw.Write(_rotation.X);
            bw.Write(_rotation.Y);
            bw.Write(_rotation.Z);

            bw.Write(_scale.X);
            bw.Write(_scale.Y);
            bw.Write(_scale.Z);
        }
    }
}
