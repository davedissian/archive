using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RubiksCube
{
    class Quat
    {
        public float s;
        public Vec3 v;

        public Quat()
        {
            s = 1.0f;
            v = new Vec3(0.0f, 0.0f, 0.0f);
        }

        public Quat(float _s, Vec3 _v)
        {
            s = _s;
            v = _v;
        }

        public static Quat Rotate(float angle, Vec3 axis)
        {
            return new Quat((float)Math.Cos(angle * 0.5f), Vec3.Mul(axis, (float)Math.Sin(angle * 0.5f)));
        }

        public static Quat Mul(Quat a, Quat b)
        {
            Quat c = new Quat();
            c.s = (b.s * a.s) - (b.v.x * a.v.x) - (b.v.y * a.v.y) - (b.v.z * a.v.z);
            c.v.x = (b.s * a.v.x) + (b.v.x * a.s) + (b.v.y * a.v.z) - (b.v.z * a.v.y);
            c.v.y = (b.s * a.v.y) + (b.v.y * a.s) + (b.v.z * a.v.x) - (b.v.x * a.v.z);
            c.v.z = (b.s * a.v.z) + (b.v.z * a.s) + (b.v.x * a.v.y) - (b.v.y * a.v.x);
            return c;
        }

        public static Quat Conjugate(Quat q)
        {
            return new Quat(q.s, new Vec3(-q.v.x, -q.v.y, -q.v.z));
        }

        public static float Dot(Quat a, Quat b)
        {
            return a.s * b.s + a.v.x * b.v.x + a.v.y * b.v.y + a.v.z * b.v.z;
        }

        public static Quat Normalise(Quat q)
        {
            float norm = (float)Math.Sqrt(q.s * q.s + q.v.x * q.v.x + q.v.y * q.v.y + q.v.z * q.v.z);
            q.s /= norm;
            q.v = Vec3.Mul(q.v, 1.0f / norm);
            return q;
        }

        public static Vec3 ToEuler(Quat q)
        {
            // http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
            // http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/
            // Irrlicht requires rotations to be specified with euler angles
            Vec3 angles = new Vec3();
            float sqW = q.s * q.s;
            float sqX = q.v.x * q.v.x;
            float sqY = q.v.y * q.v.y;
            float sqZ = q.v.z * q.v.z;
            float test = 2.0f * (q.v.y * q.s - q.v.x * q.v.z);

            if (test == 1.0f)
            {
                angles.z = -2.0f * (float)Math.Atan2(q.v.x, q.s);
                angles.x = 0.0f;
                angles.y = (float)Math.PI / 2.0f;
            }
            else if (test == -1.0f)
            {
                angles.z = 2.0f * (float)Math.Atan2(q.v.x, q.s);
                angles.x = 0;
                angles.y = (float)Math.PI / -2.0f;
            }
            else
            {
                angles.z = (float)Math.Atan2(2.0f * (q.v.x * q.v.y + q.v.z * q.s), (sqX - sqY - sqZ + sqW));
                angles.x = (float)Math.Atan2(2.0f * (q.v.y * q.v.z + q.v.x * q.s), (-sqX - sqY + sqZ + sqW));
                angles.y = (float)Math.Asin(Clamp(test, -1.0f, 1.0f));
            }

            return Vec3.Mul(angles, 180.0f / (float)Math.PI);
        }

        private static float Clamp(float value, float min, float max)
        {
            return (value < min) ? min : (value > max) ? max : value;
        }

        // Constants
        public static Quat Identity = new Quat();
    }
}
