using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Hexy
{
    public class MeshComponent : Component
    {
        public Mesh mesh
        {
            get
            {
                Mesh mesh = new Mesh(GetMesh_Internal(owner.ID));
                return mesh;
            }
            set
            {
                
            }
        }


        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern IntPtr GetMesh_Internal(uint entityID);

    }
}
