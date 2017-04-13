package com.vodotiiets.directors;

import com.vodotiiets.controllers.TaskController;
import com.vodotiiets.primitives.Matrix;
import com.vodotiiets.primitives.Vector;
import com.vodotiiets.workers.TaskWorker;

/**
 * Created by Denys Vodotiiets.
 */
public class Director {

    private static int N = 2500;
    private static int P = 1;
    private static int H = N / P;

    private static int maxValue = 5;
    private static long startTime;
    private static long endTime;

    private static Matrix MA = new Matrix(N);
    private static Matrix MO = new Matrix(N);
    private static Matrix MK = new Matrix(N);

    private static Vector B = new Vector(N);
    private static Vector C = new Vector(N);
    private static Vector Z = new Vector(N);

    public static void main(String[] args) {

        if(P > N) {
            P = N;
            H = 1;
        }

        Thread[] tasks = new Thread[P];
        TaskController monitor = new TaskController();

        for (int i = 0; i < P; i++) {
            tasks[i] = new Thread(new TaskWorker(i, monitor));
        }

        startTime = System.currentTimeMillis();
        for (int i = 0; i < P; i++) {
            tasks[i].start();
        }
    }

    public static int getN() {
        return N;
    }

    public static int getP() {
        return P;
    }

    public static int getH() {
        return H;
    }

    public static int getMaxValue() {
        return maxValue;
    }

    public static void setEndTime(long endTime) {
        Director.endTime = endTime;
    }

    public static long getTime() {
        return endTime - startTime;
    }

    public static Matrix getMA() {
        return MA;
    }

    public static Matrix getMO() {
        return MO;
    }

    public static Matrix getMK() {
        return MK;
    }

    public static Vector getB() {
        return B;
    }

    public static Vector getC() {
        return C;
    }

    public static Vector getZ() {
        return Z;
    }

}
