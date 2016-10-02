using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RubiksCube
{
    class Vec3
    {
        public float x;
        public float y;
        public float z;

        public Vec3()
        {
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
        }

        public Vec3(float _x, float _y, float _z)
        {
            x = _x;
            y = _y;
            z = _z;
        }

        public static Vec3 Add(Vec3 a, Vec3 b)
        {
            return new Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
        }

        public static Vec3 Sub(Vec3 a, Vec3 b)
        {
            return new Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
        }

        public static Vec3 Mul(Vec3 v, float s)
        {
            return new Vec3(v.x * s, v.y * s, v.z * s);
        }

        public static Vec3 Transform(Vec3 v, Quat q)
        {
            Quat vQuat = new Quat(0.0f, v);
            Quat o = Quat.Mul(Quat.Mul(q, vQuat), Quat.Conjugate(q));
            return o.v;
        }

        public static float Dot(Vec3 a, Vec3 b)
        {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }

        public static Vec3 Cross(Vec3 a, Vec3 b)
        {
            Vec3 result = new Vec3();
            result.x = a.y * b.z - a.z * b.y;
            result.y = a.z * b.x - a.x * b.z;
            result.z = a.x * b.y - a.y * b.x;
            return result;
        }

        public static IrrlichtLime.Core.Vector3Df ToIrr(Vec3 v)
        {
            return new IrrlichtLime.Core.Vector3Df(v.x, v.y, v.z);
        }

        public static Vec3 FromIrr(IrrlichtLime.Core.Vector3Df v)
        {
            return new Vec3(v.X, v.Y, v.Z);
        }
    }
}