using System;
using System.Runtime.CompilerServices;

namespace GameEngine
{
    public class Transform
    {
        internal IntPtr _ptr; //internal

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void setLocalPosition(IntPtr _ptr, Vec3 a);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static Vec3 getLocalPosition(IntPtr _ptr);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void setLocalScale(IntPtr _ptr, Vec3 a);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static Vec3 getLocalScale(IntPtr _ptr);
        internal Transform(IntPtr e) { _ptr = e; }

        public Vec3 LocalPosition
        {
            get { return getLocalPosition(_ptr); }
            set { setLocalPosition(_ptr, value); }
        }
        public Vec3 LocalScale
        {
            get { return getLocalScale(_ptr); }
            set { setLocalScale(_ptr, value); }
        }


    }
}
