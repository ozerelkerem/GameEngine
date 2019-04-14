using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace GameEngine
{
    public static class InputManager
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static byte getMouseButtonState(int Button);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static byte getKeyState(UInt16 Key);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static Vec2<double> getMouseDeltaPosition();
    }
}
