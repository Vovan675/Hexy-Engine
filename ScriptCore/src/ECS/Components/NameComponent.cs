using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Hexy
{
    public class NameComponent : Component
    {
        public string Name
        {
            get
            {
                return GetName_Internal(owner.ID);
            }
            set
            {
                SetName_Internal(owner.ID, value);
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern string GetName_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetName_Internal(uint entityID, string name);

    }
}
