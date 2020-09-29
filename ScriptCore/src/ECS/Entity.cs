using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Hexy
{
    public class Entity
    {
        public uint ID { get; private set; }
        

        protected Entity() { }

        internal Entity(uint id)
        {
            ID = id;
        }


        public T GetComponent<T>() where T : Component, new()
        {
            T comp = new T();
            comp.owner = this;
            return comp;
        }


        public List<Entity> FindEntitiesByName(string name)
        {
            FindEntitiesByName_Internal(name, out uint[] ids);

            List<Entity> entities = new List<Entity>();
            foreach(uint id in ids) {
                entities.Add(new Entity(id));
            }
            return entities;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void FindEntitiesByName_Internal(string name, out uint[] entities);
    }


}
