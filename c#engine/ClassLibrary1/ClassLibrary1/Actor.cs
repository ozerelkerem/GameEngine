using System.Runtime.CompilerServices;

using System;

namespace GameEngine
{
  

    public class Actor
    {
       

        internal IntPtr actorptr;//internal

        public Transform transform { get; }

        private Actor(IntPtr id, IntPtr trptr)
        {
            this.actorptr = id;
            this.transform = new Transform(trptr);
        }

        

    }
    


}
