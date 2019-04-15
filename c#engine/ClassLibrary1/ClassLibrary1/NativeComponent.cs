using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameEngine
{
    [AttributeUsage(AttributeTargets.All)]
    public class NativeComponent : Attribute
    {
        public string Type
        {
            get;
            set;
        }
    }
}
