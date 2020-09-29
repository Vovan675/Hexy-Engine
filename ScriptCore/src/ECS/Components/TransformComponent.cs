using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Hexy
{
    public class TransformComponent : Component
    {
        public Vector3 position
        {
            get
            {
                GetPosition_Internal(owner.ID, out Vector3 vec);
                return vec;
            }
            set
            {
                SetPosition_Internal(owner.ID, ref value);
            }
        }


        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetPosition_Internal(uint entityID, out Vector3 vec);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetPosition_Internal(uint entityID, ref Vector3 vec);
    }
}
