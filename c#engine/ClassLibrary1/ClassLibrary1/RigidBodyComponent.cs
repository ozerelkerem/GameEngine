using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace GameEngine
{
    [NativeComponent(Type = "rigidbodycomponent")]
    public class RigidBodyComponent : Component
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void addForce(IntPtr _component, Vec3<float> a);


        public RigidBodyComponent(IntPtr selfptr, IntPtr projectmanager, Actor _actor) : base(_actor)
        {
            _projectmanagerptr = projectmanager;
            _selfptr = selfptr;
        }

        public void AddForce(Vec3<float> force)
        {
            addForce(_selfptr, force);
        }
    

    }
}
