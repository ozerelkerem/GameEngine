using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace GameEngine
{
    [NativeComponent(Type = "animatorcomponent")]
    public class AnimatorComponent : Component
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void stop(IntPtr _component);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static IntPtr playLoop(IntPtr _component, IntPtr _animation);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static IntPtr playOnce(IntPtr _component, IntPtr _animation);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool getState(IntPtr _component);

        public Animation GetAnimationByName(string name)
        {
            IntPtr val = Animation.getAnimationByName(_projectmanagerptr, name);
            if (val != IntPtr.Zero)
                return new Animation(val);
            return null;
        }
        public AnimatorComponent(IntPtr selfptr,IntPtr projectmanager,Actor _actor) : base(_actor)
        {
            _projectmanagerptr = projectmanager;
            _selfptr = selfptr;
        }

        public void PlayLoop(Animation animation)
        {
            playLoop(_selfptr,animation._ptr);
        }
        public void PlayOnce(Animation animation)
        {
            playOnce(_selfptr,animation._ptr);
        }
        public void Stop()
        {
            stop(_selfptr);
        }
        public bool GetState()
        {
            return getState(_selfptr);
        }

    }
}
