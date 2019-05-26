using System;
using System.Runtime.CompilerServices;

namespace GameEngine
{
    public class Transform
    {
        internal IntPtr _ptr; //internal

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void applyToRigidBody(IntPtr _ptr);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void setLocalPosition(IntPtr _ptr, Vec3<float> a);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static Vec3<float> getLocalPosition(IntPtr _ptr);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void setLocalScale(IntPtr _ptr, Vec3<float> a);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static Vec3<float> getLocalScale(IntPtr _ptr);

        internal Transform(IntPtr e) { _ptr = e; }

        public Vec3<float> LocalPosition
        {
            get { return getLocalPosition(_ptr); }
            set {
                setLocalPosition(_ptr, value);
                applyToRigidBody(_ptr);
            }
        }
        public Vec3<float> LocalScale
        {
            get { return getLocalScale(_ptr); }
            set { setLocalScale(_ptr, value); }
        }


    }
}
