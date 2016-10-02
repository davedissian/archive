using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using IrrlichtLime;
using IrrlichtLime.Core;
using IrrlichtLime.GUI;
using IrrlichtLime.IO;
using IrrlichtLime.Scene;
using IrrlichtLime.Video;

namespace RubiksCube
{
    class CubePiece
    {
        private MeshSceneNode mSceneNode;
        private Vec3 mPosition;
        private Quat mRotation;

        public CubePiece(Vec3 position, Quat rotation, Renderer renderer, int index)
        {
            mSceneNode = renderer.mSceneMgr.AddMeshSceneNode(renderer.mSceneMgr.GetMesh("cube.obj"));
            this.Position = position;
            this.Rotation = rotation;
        }

        public Vec3 Position
        {
            set
            {
                mSceneNode.Position = Vec3.ToIrr(value) * 10.0f;
                mPosition = value;
            }
            get
            {
                return mPosition;
            }
        }

        public Quat Rotation
        {
            set
            {
                mSceneNode.Rotation = Vec3.ToIrr(Quat.ToEuler(value));
                mRotation = value;
            }
            get
            {
                return mRotation;
            }
        }
    }
}