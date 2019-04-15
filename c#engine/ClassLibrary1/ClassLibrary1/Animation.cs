using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace GameEngine
{
    public class Animation
    {
        internal IntPtr _ptr; //internal

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static IntPtr getAnimationByName(IntPtr _ptr,string name);

        internal Animation(IntPtr ptr)
        {
            _ptr = ptr;
        }
    }
}
