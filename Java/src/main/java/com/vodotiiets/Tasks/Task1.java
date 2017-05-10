package com.vodotiiets.Tasks;

import com.vodotiiets.Utils.Util;
import com.vodotiiets.controllers.DataController;
import com.vodotiiets.controllers.TaskController;
import com.vodotiiets.directors.Director;
import com.vodotiiets.primitives.Matrix;

/**
 * Created by Denys Vodotiiets.
 */
public class Task1 implements Runnable {

    private int startIndex;
    private int endIndex;

    private DataController dataController;
    private TaskController taskController;

    public Task1(DataController dataController, TaskController taskController) {
        this.dataController = dataController;
        this.taskController = taskController;

        startIndex = 0;
        endIndex = Director.H;
    }

    @Override
    public void run() {
        //input
        dataController.inputMR();
        Director.C.fill();
        Director.Z.fill();

        taskController.signalInputDone();
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
        for(int i = startIndex; i < endIndex; i++) {
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

        taskController.signalCalculateDone();

    }

}

