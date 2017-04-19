using System;

namespace ConsoleApplication1
{
    class Matrix
    {
        private Vector[] matr;

        public Matrix(int len)
        {
            matr = new Vector[len];
        }

        public Matrix(Matrix mat)
        {
            matr = new Vector[mat.getLength()];
            for (int i = 0; i < matr.Length; i++)
            {
                matr[i] = new Vector(mat.getString(i));
            }
        }

        public Vector getString(int i)
        {
            return matr[i];
        }

        public int getElement(int i, int j)
        {
            return matr[i].getElement(j);
        }

        public void setElement(int i, int j, int func)
        {
            matr[i].setElement(j, func);
        }

        public void setString(int i, Vector func)
        {
            matr[i] = func;
        }

        public int getLength()
        {
            return matr.Length;
        }

        public void inputMatrix()
        {
            for (int i = 0; i < matr.Length; i++)
            {
                matr[i] = new Vector(matr.Length);
                matr[i].inputVector();
            }
        }

        public String outputMatrix()
        {
            String s = "";
            for (int i = 0; i < matr.Length; i++)
            {
                s = s + matr[i].outputVector() + "\n";
            }
            return s;
        }

    }

}

