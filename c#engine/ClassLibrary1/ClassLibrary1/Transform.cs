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
        extern static void calcQuatFromEuler(IntPtr _ptr);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void setLocalPosition(IntPtr _ptr, Vec3<float> a);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static Vec3<float> getLocalPosition(IntPtr _ptr);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void setLocalEulerRotation(IntPtr _ptr, Vec3<float> a);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static Vec3<float> getLocalEulerRotation(IntPtr _ptr);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void setLocalScale(IntPtr _ptr, Vec3<float> a);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static Vec3<float> getLocalScale(IntPtr _ptr);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static Vec3<float> getForwardVector(IntPtr _ptr);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static Vec3<float> getUpVector(IntPtr _ptr);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static Vec3<float> getRightVector(IntPtr _ptr);



        internal Transform(IntPtr e) { _ptr = e; }

        public Vec3<float> LocalPosition
        {
            get { return getLocalPosition(_ptr); }
            set {
                setLocalPosition(_ptr, value);
                applyToRigidBody(_ptr);
            }
        }
        public Vec3<float> LocalRotation
        {
            get { return getLocalEulerRotation(_ptr); }
            set
            {
                setLocalEulerRotation(_ptr, value);
                calcQuatFromEuler(_ptr);
                applyToRigidBody(_ptr);
            }
        }
        public Vec3<float> LocalScale
        {
            get { return getLocalScale(_ptr); }
            set { setLocalScale(_ptr, value); }
        }
        public Vec3<float> GetForwardVector
        {
            get { return getForwardVector(_ptr); }
        }
        public Vec3<float> GetRightVector
        {
            get { return getRightVector(_ptr); }
        }
        public Vec3<float> GetUpVector
        {
            get { return getUpVector(_ptr); }
        }

    }
}
