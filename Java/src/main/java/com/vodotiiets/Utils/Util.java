package com.vodotiiets.Utils;

import com.vodotiiets.controllers.DataController;
import com.vodotiiets.controllers.TaskController;
import com.vodotiiets.directors.Director;
import com.vodotiiets.primitives.Matrix;
import com.vodotiiets.primitives.Vector;

/**
 * Created by Denys Vodotiiets.
 */
public class Util {

    public static void calculate(int startIndex, int endIndex, int a, int b, Matrix MR) {

        for (int i = startIndex; i < endIndex; i++) {
            //MS*MR
            Vector temp = new Vector(Director.N);
            for (int j = 0; j < Director.N; j++) {
                int element = 0;
                for(int k = 0; k < Director.N; k++) {
                    element += Director.MS.get(i, k) * MR.get(k, j);
                }
                temp.set(j, element);
            }


            for (int j = 0; j < Director.N; j++) {
                //b*(MS*MR)
                int sum = temp.get(i) * b;

                //a*MO*b(MS*MR)
                sum += a * Director.MO.get(i, j);
                Director.MA.set(i, j, sum);
            }

        }

    }

    public static void run(TaskController taskController, DataController dataController, int startIndex, int endIndex) {
        taskController.waitInput();

        //Calculate alpha
        int a_i = 0;
        for (int i = startIndex; i < endIndex; i++) {
            a_i += Director.B.get(i) * Director.C.get(i);
        }

        dataController.calculateA(a_i);
        taskController.signalCalculateADone();

        //Calculate beta
        int b_i = Integer.MIN_VALUE;
        for (int i = startIndex; i < endIndex; i++) {
            b_i = b_i < Director.Z.get(i) ? Director.Z.get(i) : b_i;
        }
        dataController.calculateB(b_i);
        taskController.signalCalculateBDone();

        taskController.waitCalculateA();
        taskController.waitCalculateB();

        //Copy data
        int a = dataController.copyA();
        int b = dataController.copyB();
        Matrix MR = dataController.copyMR();

        //Calculate
        Util.calculate(startIndex, endIndex, a, b, MR);
    }

}
