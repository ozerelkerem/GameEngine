using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace GameEngine
{
    public static class GameManager
    {

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static IntPtr destroyActor(IntPtr _ptr);

        public static void DestoryActor(Actor a)
        {
            destroyActor(a.actorptr);
        }
    }
}
