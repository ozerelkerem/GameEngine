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

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void addTorque(IntPtr _component, Vec3<float> a);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void clearForce(IntPtr _component);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void clearTorque(IntPtr _component);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void setAngularDamping(IntPtr _component, float x);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void setAngularVelocity(IntPtr _component, Vec3<float> a);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void setLinearDamping(IntPtr _component, float x);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void setLinearVelocity(IntPtr _component, Vec3<float> a);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void setMass(IntPtr _component, float x);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void setMaxLinearVelocity(IntPtr _component, float x);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void setMaxAngularVelocity(IntPtr _component, float x);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static float getAngularDamping(IntPtr _component);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static Vec3<float> getAngularVelocity(IntPtr _component);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static float getLinearDamping(IntPtr _component);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static Vec3<float> getLinearVelocity(IntPtr _component);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static float getMass(IntPtr _component);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static float getMaxLinearVelocity(IntPtr _component);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static float getMaxAngularVelocity(IntPtr _component);

        public RigidBodyComponent(IntPtr selfptr, IntPtr projectmanager, Actor _actor) : base(_actor)
        {
            _projectmanagerptr = projectmanager;
            _selfptr = selfptr;
        }

        public void AddForce(Vec3<float> force)
        {
            addForce(_selfptr, force);
        }

        public void AddTorque(Vec3<float> torque)
        {
            addTorque(_selfptr, torque);
        }

        public void ClearForce()
        {
            clearForce(_selfptr);
        }

        public void ClearTorque()
        {
            clearTorque(_selfptr);
        }

        
        public void SetAngularDamping(float x)
        {
            setAngularDamping(_selfptr, x);
        }

        
        public void SetAngularVelocity(Vec3<float> a)
        {
            setAngularVelocity(_selfptr, a);
        }

        
        public void SetLinearDamping(float x)
        {
            setLinearDamping(_selfptr, x);
        }

        
        public void SetLinearVelocity(Vec3<float> a)
        {
            setLinearVelocity(_selfptr, a);
        }

        
        public void SetMass(float x)
        {
            setMass(_selfptr, x);
        }

        
        public void SetMaxLinearVelocity(float x)
        {
            setMaxLinearVelocity(_selfptr, x);
        }

        
        public void SetMaxAngularVelocity(float x)
        {
            setMaxAngularVelocity(_selfptr, x);
        }


        
        public float GetAngularDamping()
        {
            return getAngularDamping(_selfptr);
        }

        
        public Vec3<float> GetAngularVelocity(IntPtr _component)
        {
            return getAngularVelocity(_selfptr);
        }

        
        public float GetLinearDamping()
        {
            return getLinearDamping(_selfptr);
        }

        
        public Vec3<float> GetLinearVelocity()
        {
            return getLinearVelocity(_selfptr);
        }

        
        public float GetMass()
        {
            return getMass(_selfptr);
        }

        
        public float GetMaxLinearVelocity()
        {
            return getMaxLinearVelocity(_selfptr);
        }

        
        public float GetMaxAngularVelocity()
        {
            return getMaxAngularVelocity(_selfptr);
        }





    }
}
