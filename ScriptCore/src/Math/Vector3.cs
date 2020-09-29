using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Hexy
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector3
    {
        public float x;
        public float y;
        public float z;


        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public Vector3(float v)
        {
            x = v;
            y = v;
            z = v;
        }


        public static float Magnitude(Vector3 vec)
        {
            return (float)Math.Sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
        }
        public float magnitude
        {
            get { return (float)Math.Sqrt(x*x + y*y + z*z); }
        }


        public static Vector3 Normalize(Vector3 vec)
        {
            float mag = Magnitude(vec);
            if (mag > 0)
            {
                return vec / mag;
            }
            else
            {
                return new Vector3(0);
            }
        }

        public Vector3 normalized
        {
            get { return Normalize(this); }
        }


        public static float Dot(Vector3 a, Vector3 b)
        {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }


        public static float Distance(Vector3 a, Vector3 b)
        {
            Vector3 diff = a - b;
            return (float)Math.Sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
        }


        public static bool operator ==(Vector3 left, Vector3 right)
        {
            return left.x == right.x && left.y == right.y && left.z == right.z;
        }

        public static bool operator !=(Vector3 left, Vector3 right)
        {
            return !(left == right);
        }

        public static Vector3 operator +(Vector3 left, Vector3 right)
        {
            return new Vector3(left.x + right.x, left.y + right.y, left.z + right.z);
        }

        public static Vector3 operator +(Vector3 vec, float scalar)
        {
            return new Vector3(vec.x + scalar, vec.y + scalar, vec.z + scalar);
        }

        public static Vector3 operator +(float scalar, Vector3 vec)
        {
            return new Vector3(vec.x + scalar, vec.y + scalar, vec.z + scalar);
        }


        public static Vector3 operator -(Vector3 left, Vector3 right)
        {
            return new Vector3(left.x - right.x, left.y - right.y, left.z - right.z);
        }

        public static Vector3 operator -(Vector3 vec, float scalar)
        {
            return new Vector3(vec.x - scalar, vec.y - scalar, vec.z - scalar);
        }

        public static Vector3 operator -(Vector3 vec)
        {
            return new Vector3(-vec.x, -vec.y, -vec.z);
        }



        public static Vector3 operator *(Vector3 vec, float scalar)
        {
            return new Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
        }

        public static Vector3 operator *(float scalar, Vector3 vec)
        {
            return new Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
        }

        public static Vector3 operator /(Vector3 vec, float scalar)
        {
            return new Vector3(vec.x / scalar, vec.y / scalar, vec.z / scalar);
        }


        public override string ToString()
        {
            return String.Format("({0}, {1}, {2})", x, y, z);
        }

    }
}
