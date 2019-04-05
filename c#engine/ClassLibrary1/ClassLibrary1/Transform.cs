using System;
using System.Runtime.CompilerServices;

namespace GameEngine
{
    public class Transform
    {
        internal UIntPtr _ptr;

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void setLocalPosition(UIntPtr _ptr, Vec3 a);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static Vec3 getLocalPosition(UIntPtr _ptr);


        internal Transform(UIntPtr e) { _ptr = e; }

        public Vec3 LocalPosition
        {
            get { return getLocalPosition(_ptr); }
            set { setLocalPosition(_ptr, value); }
        }


    }
}
