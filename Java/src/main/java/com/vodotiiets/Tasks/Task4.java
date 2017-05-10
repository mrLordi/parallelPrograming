package com.vodotiiets.Tasks;

import com.vodotiiets.Utils.Util;
import com.vodotiiets.controllers.DataController;
import com.vodotiiets.controllers.TaskController;
import com.vodotiiets.directors.Director;
import com.vodotiiets.primitives.Matrix;

/**
 * Created by Denys Vodotiiets.
 */
public class Task4 implements Runnable {

    private int startIndex;
    private int endIndex;

    private DataController dataController;
    private TaskController taskController;

    public Task4(DataController dataController, TaskController taskController) {
        this.dataController = dataController;
        this.taskController = taskController;

        startIndex = Director.H * 3;
        endIndex = Director.H * 4;
    }

    @Override
    public void run() {
        Util.run(taskController, dataController, startIndex, endIndex);
        taskController.signalCalculateDone();
    }

}

