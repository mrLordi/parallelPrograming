using System;
using System.Threading;

/////////////////////////////////////////////////////////////
/////   Lab 2. C#                                       /////
/////   A = sort(d*B + e*C*(MO*MK))                     /////
/////   Denys Vodotiiets                                /////
/////   IP-42                                           /////
/////////////////////////////////////////////////////////////

namespace ConsoleApplication1
{
    class Program
    {
        public static int N = 6;
        public static int P = 4;
        public static int H = N / P;

        public static Matrix MO, MK;
        public static Vector A, B, C;
        public static int d, e;

        public static Semaphore S1, S2, S3, S4;
        public static Mutex MUT = new Mutex(false);
        public static Object CS1 = new Object();
        public static EventWaitHandle E_InputT1;
        public static EventWaitHandle E_InputT3;
        public static EventWaitHandle E_InputT4;

        static void Main(string[] args)
        {
            /*if (N % P == 0)
            {
                H = N / P;
            }
            else
            {
                H = (N / P)+1;
            } 
            Console.WriteLine(">>H = {0}", H);*/

            A = new Vector(N);

            E_InputT1 = new ManualResetEvent(false);
            E_InputT3 = new ManualResetEvent(false);
            E_InputT4 = new ManualResetEvent(false);

            S1 = new Semaphore(0, 1);
            S2 = new Semaphore(0, 1);
            S3 = new Semaphore(0, 1);
            S4 = new Semaphore(0, 1);

            Thread t1 = new Thread(T1);
            Thread t2 = new Thread(T2);
            Thread t3 = new Thread(T3);
            Thread t4 = new Thread(T4);

            t1.Start();
            t2.Start();
            t3.Start();
            t4.Start();


            Console.ReadKey();
        }

        static void T1()
        {
            int k = 1;

            int d1, e1;
            Vector C1;
            Matrix MO1;

            Console.WriteLine(">>T1: Started");

            //Input data
            C = new Vector(N);
            C.inputVector();
            MO = new Matrix(N);
            MO.inputMatrix();
            //End input data

            //Signal: end entering data
            E_InputT1.Set();

            //Wait input data from T3 and T4
            E_InputT3.WaitOne();
            E_InputT4.WaitOne();

            //Copy data
            lock (CS1)
            {
                d1 = d;
                e1 = e;
            }
            MUT.WaitOne();
            C1 = new Vector(C);
            MO1 = new Matrix(MO);
            MUT.ReleaseMutex();
            //End copy

            //Calculating
            Vector Temp = new Vector(N);
            for (int i = (k - 1) * H; i < k * H; i++)
            {
                //MO1*MK
                for (int j = 0; j < N; j++)
                {
                    int element = 0;
                    for (int l = 0; l < N; l++)
                    {
                        element += MK.getElement(i, l) * MO1.getElement(l, j);
                    }
                    Temp.setElement(j, element);
                }

                //C1*(MO1*MK)
                int sum = 0;
                for (int j = 0; j < N; j++)
                {
                    sum += Temp.getElement(j) * C1.getElement(j);
                }

                //d1*B + e1*C1*(MO1*MK)
                sum = e1 * sum + B.getElement(i) * d1;

                A.setElement(i, sum);
            }
            A.sort((k - 1) * H, H);
            //End calculating

            //Wait signal from T2
            S1.WaitOne();

            //Merge A2H
            Vector tv = new Vector(2 * H);
            int point1 = (k - 1) * H;
            int point2 = k * H;

            for (int i = 0; i < tv.getLength(); i++)
            {
                if ((point2 >= (k + 1) * H) || ((A.getElement(point1) <= A.getElement(point2)) && (point1 < k * H)))
                {
                    tv.setElement(i, A.getElement(point1));
                    point1++;
                }
                else
                {
                    tv.setElement(i, A.getElement(point2));
                    point2++;
                }
            }
            for (int i = 0; i < tv.getLength(); i++)
            {
                A.setElement((k - 1) * H + i, tv.getElement(i));
            }
            //End merge

            //Wait signal from T3
            S3.WaitOne();

            //Merge A
            tv = new Vector(N);
            point1 = 0;
            point2 = 2 * H;

            for (int i = 0; i < tv.getLength(); i++)
            {
                if ((point2 >= N) || ((A.getElement(point1) <= A.getElement(point2)) && (point1 < 2 * H)))
                {
                    tv.setElement(i, A.getElement(point1));
                    point1++;
                }
                else
                {
                    tv.setElement(i, A.getElement(point2));
                    point2++;
                }
            }
            for (int i = 0; i < tv.getLength(); i++)
            {
                A.setElement((k - 1) * H + i, tv.getElement(i));
            }
            //End merge

            //Send signal T4
            S4.Release();

            Console.WriteLine(">>T1: Finished");
        }

        static void T2()
        {
            int k = 2;

            int d2, e2;
            Vector C2;
            Matrix MO2;

            Console.WriteLine(">>T2: Started");

            //Wait input data from T1, T3, T4
            E_InputT1.WaitOne();
            E_InputT3.WaitOne();
            E_InputT4.WaitOne();

            //Copy data
            lock (CS1)
            {
                e2 = e;
                d2 = d;
            }
            MUT.WaitOne();
            C2 = new Vector(C);
            MO2 = new Matrix(MO);
            MUT.ReleaseMutex();
            //End copy

            //Calculating
            Vector Temp = new Vector(N);
            for (int i = (k - 1) * H; i < k * H; i++)
            {
                //MO2*MK
                for (int j = 0; j < N; j++)
                {
                    int element = 0;
                    for (int l = 0; l < N; l++)
                    {
                        element += MK.getElement(i, l) * MO2.getElement(l, j);
                    }
                    Temp.setElement(j, element);
                }

                //C2*(MO2*MK)
                int sum = 0;
                for (int j = 0; j < N; j++)
                {
                    sum += Temp.getElement(j) * C2.getElement(j);
                }

                //d2*B + e2*C2*(MO2*MK)
                sum = e2 * sum + B.getElement(i) * d2;

                A.setElement(i, sum);
            }
            A.sort((k - 1) * H, H);
            //End calculating

            //Send signal T1
            S1.Release();

            Console.WriteLine(">>T2: Finished");
        }

        static void T3()
        {
            int k = 3;

            int d3, e3;
            Vector C3;
            Matrix MO3;

            Console.WriteLine(">>T3: Started");

            //Input data
            e = d = 1;
            //End input data

            //Signal: end entering data
            E_InputT3.Set();

            //Wait input data from T1 and T4
            E_InputT1.WaitOne();
            E_InputT4.WaitOne();

            //Copy data
            lock (CS1)
            {
                e3 = e;
                d3 = d;
            }
            MUT.WaitOne();
            C3 = new Vector(C);
            MO3 = new Matrix(MO);
            MUT.ReleaseMutex();
            //End copy

            //Calculating
            Vector Temp = new Vector(N);
            for (int i = (k - 1) * H; i < k * H; i++)
            {
                //MO3*MK
                for (int j = 0; j < N; j++)
                {
                    int element = 0;
                    for (int l = 0; l < N; l++)
                    {
                        element += MK.getElement(i, l) * MO3.getElement(l, j);
                    }
                    Temp.setElement(j, element);
                }

                //C3*(MO3*MK)
                int sum = 0;
                for (int j = 0; j < N; j++)
                {
                    sum += Temp.getElement(j) * C3.getElement(j);
                }

                //d3*B + e3*C3*(MO3*MK)
                sum = e3 * sum + B.getElement(i) * d3;

                A.setElement(i, sum);
            }
            A.sort((k - 1) * H, H);
            //End calculating

            //Wait signal from T4
            S2.WaitOne();

            //Merge A2H
            Vector tv = new Vector(2 * H);
            int point1 = (k - 1) * H;
            int point2 = k * H;

            for (int i = 0; i < tv.getLength(); i++)
            {
                if ((point2 >= (k + 1) * H) || ((A.getElement(point1) <= A.getElement(point2)) && (point1 < k * H)))
                {
                    tv.setElement(i, A.getElement(point1));
                    point1++;
                }
                else
                {
                    tv.setElement(i, A.getElement(point2));
                    point2++;
                }
            }
            for (int i = 0; i < tv.getLength(); i++)
            {
                A.setElement((k - 1) * H + i, tv.getElement(i));
            }
            //End merge

            //Send signal T1
            S3.Release();

            Console.WriteLine(">>T3: Finished");
        }

        static void T4()
        {
            int k = 4;

            int d4, e4;
            Vector C4;
            Matrix MO4;

            Console.WriteLine(">>T4: Started");

            //Input data
            B = new Vector(N);
            B.inputVector();
            MK = new Matrix(N);
            MK.inputMatrix();
            //End input data

            //Signal: end entering data
            E_InputT4.Set();

            //Wait signal from T1 and T3
            E_InputT1.WaitOne();
            E_InputT3.WaitOne();

            //Copy data
            lock (CS1)
            {
                e4 = e;
                d4 = d;
            }
            MUT.WaitOne();
            C4 = new Vector(C);
            MO4 = new Matrix(MO);
            MUT.ReleaseMutex();
            //End copy

            //Calculating
            Vector Temp = new Vector(N);
            for (int i = (k - 1) * H; i < N; i++)
            {
                //MO4*MK
                for (int j = 0; j < N; j++)
                {
                    int element = 0;
                    for (int l = 0; l < N; l++)
                    {
                        element += MK.getElement(l, i) * MO4.getElement(j, l);
                    }
                    Temp.setElement(j, element);
                }

                //C4*(MO4*MK)
                int sum = 0;
                for (int j = 0; j < N; j++)
                {
                    sum += Temp.getElement(j) * C4.getElement(j);
                }

                //d4*B + e4*C4*(MO4*MK)
                sum = e4 * sum + B.getElement(i) * d4;

                A.setElement(i, sum);
            }
            A.sort((k - 1) * H, N - (k - 1) * H);
            //End calculating

            //Send signal T3
            S2.Release();

            //Wait signal from T1
            S4.WaitOne();

            Console.WriteLine("Calculation end.\nResult:");
            if (N <= 100)
            {
                Console.WriteLine(A.outputVector());
            }
            else
            {
                Console.WriteLine("Vector is too large.");
            }

            Console.WriteLine(">>T4: Finished");
        }
    }
}
