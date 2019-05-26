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

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static Actor spawnActor(IntPtr _projectmangr, string name, IntPtr _targetactor);

        static internal IntPtr _projectmanagerptr;

        public static void DestoryActor(Actor a)
        {
            destroyActor(a.actorptr);
        }
        
        public static Actor SpawnActor(string name, Actor target)
        {
           return spawnActor(_projectmanagerptr, name, target.actorptr);
        }
        public static Actor SpawnActor(string name)
        {
           return spawnActor(_projectmanagerptr, name, IntPtr.Zero);
        }
    }
}
