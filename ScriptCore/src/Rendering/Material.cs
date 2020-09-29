using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Hexy
{
    public class Material
    {
        public IntPtr instancePtr;

        internal Material(IntPtr instancePtr)
        {
            this.instancePtr = instancePtr;
        }

        public void Set(string uniform, float value)
        {
            SetFloat_Internal(instancePtr, uniform, value);
        }
        public void Set(string uniform, Vector3 vec)
        {
            SetVec3_Internal(instancePtr, uniform, ref vec);
        }

        public void Set(string uniform, Texture2D texture)
        {
            SetTexture2D_Internal(instancePtr, uniform, texture.instancePtr);
        }


        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern IntPtr SetFloat_Internal(IntPtr materialInstancePtr, string uniformName, float val);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern IntPtr SetVec3_Internal(IntPtr materialInstancePtr, string uniformName, ref Vector3 vec);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern IntPtr SetTexture2D_Internal(IntPtr materialInstancePtr, string uniformName, IntPtr image);
    }
}
