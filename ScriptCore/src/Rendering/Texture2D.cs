using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Hexy
{
    public class Texture2D
    {
        internal IntPtr instancePtr;

        internal Texture2D(IntPtr instancePtr)
        {
            this.instancePtr = instancePtr;
        }

        public Texture2D(string path)
        {
            instancePtr = Constructor_Path_Internal(path);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern IntPtr Constructor_Path_Internal(string path);
    }
}
