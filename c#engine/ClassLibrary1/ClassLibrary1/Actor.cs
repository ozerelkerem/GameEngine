using System.Runtime.CompilerServices;

using System;

namespace GameEngine
{
  

    public class Actor
    {
       

        internal UIntPtr entityid;

        public Transform transform { get; }

        private Actor(UIntPtr id, UIntPtr trptr)
        {
            this.transform = new Transform(trptr);
        }

    }
    


}
