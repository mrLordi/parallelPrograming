using System;

namespace ConsoleApplication1
{
    class Vector
    {
        private int[] vect;

        public Vector(int length)
        {
            vect = new int[length];
        }

        public Vector(Vector vec)
        {
            vect = new int[vec.getLength()];
            for (int i = 0; i < vec.getLength(); i++)
            {
                vect[i] = vec.getElement(i);
            }

        }

        public int getLength()
        {
            return vect.Length;
        }

        public int getElement(int i)
        {
            return vect[i];
        }

        public void setElement(int i, int func)
        {
            vect[i] = func;
        }


        public void sort(int a, int b)
        {
            Array.Sort(vect, a, b);
        }

        public void inputVector()
        {
            for (int i = 0; i < vect.Length; i++)
            {
                vect[i] = 1;
            }
        }

        public String outputVector()
        {
            String s = "";
            for (int i = 0; i < vect.Length; i++)
            {
                s = s + " " + vect[i];
            }
            return s;
        }
    }
}
