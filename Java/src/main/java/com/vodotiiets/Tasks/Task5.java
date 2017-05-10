package com.vodotiiets.Tasks;

import com.vodotiiets.Utils.Util;
import com.vodotiiets.controllers.DataController;
import com.vodotiiets.controllers.TaskController;
import com.vodotiiets.directors.Director;

/**
 * Created by Denys Vodotiiets.
 */
public class Task5 implements Runnable{

    private int startIndex;
    private int endIndex;

    private DataController dataController;
    private TaskController taskController;

    public Task5(DataController dataController, TaskController taskController) {
        this.dataController = dataController;
        this.taskController = taskController;

        startIndex = Director.H * 4;
        endIndex = Director.H * 5;
    }

    @Override
    public void run() {
        Util.run(taskController, dataController, startIndex, endIndex);
        taskController.signalCalculateDone();
    }

}
