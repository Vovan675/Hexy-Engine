using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Hexy
{
    public class Mesh
    {
        public IntPtr instancePtr;

        internal Mesh(IntPtr instancePtr)
        {
            this.instancePtr = instancePtr;
        }

        public Material GetMaterial()
        {
            return new Material(GetMaterial_Internal(instancePtr));
        }


        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern IntPtr GetMaterial_Internal(IntPtr meshInstancePtr);
        
    }
}
