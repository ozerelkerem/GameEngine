using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace GameEngine
{
    
    public class Component
    {
 

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static IntPtr hasComponent(IntPtr _ptr, string name);


        internal IntPtr _projectmanagerptr;
        internal IntPtr _selfptr;

        private Actor actor_;
        public Actor actor
        {
            get { return actor_; }
            set
            {
                if (actor_ != null) throw new Exception("Component's actor can not be reset");
                actor_ = value;
               
            }
        }

        public Component(Actor _actor)
        {
            actor_ = _actor;
        }
        public Component() { }

        

        public T GetComponent<T>() where T : Component
        {
           foreach(var x in System.Attribute.GetCustomAttributes(typeof(T)))
            {
               if(x is NativeComponent)
                {
                    IntPtr val = hasComponent(actor.actorptr, (x as NativeComponent).Type);
                    if (val!=IntPtr.Zero)
                        return (T)System.Activator.CreateInstance(typeof(T), new object[] {val,_projectmanagerptr,actor});
                }
            }
            return null;
 
        }
    }
}
