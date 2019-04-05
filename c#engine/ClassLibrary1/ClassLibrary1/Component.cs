using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameEngine
{
   
    public class Component
    {
        protected Actor entity_;
        public Actor entity
        {
            get { return entity_; }
            set
            {
                if (entity_ != null) throw new Exception("Component's entity can not be reset");
                entity_ = value;
               
            }
        }

        public Component(Actor _entity)
        {
            entity_ = _entity;
        }
    }
}
