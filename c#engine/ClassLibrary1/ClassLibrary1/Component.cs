using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameEngine
{
   
    public class Component
    {
        private Actor actor_;
        public Actor actor
        {
            get { return actor_; }
            set
            {
                if (actor_ != null) throw new Exception("Component's actor can not be reset");
                actor_ = value;
               
            }
        }

        public Component(Actor _actor)
        {
            actor_ = _actor;
        }
        public Component() { }
    }
}
