

namespace GameEngine
{
    public struct Vec3<T>
    {
        public T x, y, z;
        public Vec3(T a,T b,T c)
        {
            x = a; y = b; z = c;
        }
    };

    public struct Vec2<T>
    {
        public T x, y;
        public Vec2(T a, T b)
        {
            x = a; y = b;
        }
    };
}


