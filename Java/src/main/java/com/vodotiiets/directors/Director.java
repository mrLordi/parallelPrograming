package com.vodotiiets.directors;

import com.vodotiiets.Tasks.*;
import com.vodotiiets.controllers.DataController;
import com.vodotiiets.controllers.TaskController;
import com.vodotiiets.primitives.Matrix;
import com.vodotiiets.primitives.Vector;

/**
 * Created by Denys Vodotiiets.
 */
public class Director {
    public static final int N = 10;
    public static final int P = 6;
    public static final int H = N / P;

    public static int inputCount = 3;

    public static Vector B = new Vector(N);
    public static Vector C = new Vector(N);
    public static Vector Z = new Vector(N);
    public static Matrix MO = new Matrix(N);
    public static Matrix MS = new Matrix(N);
    public static Matrix MR = new Matrix(N);
    public static Matrix MA = new Matrix(N);

    public static void main(String[] args) {
        Thread[] tasks = new Thread[P];
        DataController dm = new DataController();
        TaskController tm = new TaskController();

        tasks[0] = new Thread(new Task1(dm, tm));
        tasks[1] = new Thread(new Task2(dm, tm));
        tasks[2] = new Thread(new Task3(dm, tm));
        tasks[3] = new Thread(new Task4(dm, tm));
        tasks[4] = new Thread(new Task5(dm, tm));
        tasks[5] = new Thread(new Task6(dm, tm));

        for (int i = 0; i < P; i++) {
            tasks[i].start();
        }
    }
}

