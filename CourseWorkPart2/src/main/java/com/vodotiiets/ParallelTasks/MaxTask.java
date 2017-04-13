package com.vodotiiets.ParallelTasks;

import com.vodotiiets.directors.Director;
import com.vodotiiets.primitives.Vector;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.RecursiveTask;

/**
 * Created by Denys Vodotiiets.
 */
public class MaxTask extends RecursiveTask<Integer> {
    private Vector vector;

    public MaxTask(Vector vector) {
        this.vector = vector;
    }

    @Override
    protected Integer compute() {
        if(vector.getDimension() > Director.getH() / Director.getP()) {
            List<MaxTask> subtasks = createSubtasks();

            for(MaxTask subtask : subtasks) {
                subtask.fork();
            }

            int result = Integer.MIN_VALUE;
            for(MaxTask subtask : subtasks) {
                int temp = subtask.join();
                if(temp > result) {
                    result = temp;
                }
            }

            return result;
        } else {
            return vector.getMaxElement(0, vector.getDimension());
        }
    }

    private List<MaxTask> createSubtasks() {
        List<MaxTask> subtasks = new ArrayList<>();

        MaxTask subtask1 = new MaxTask(this.vector.getPart(0, this.vector.getDimension()/2));
        MaxTask subtask2 = new MaxTask(this.vector.getPart(this.vector.getDimension()/2 + 1, this.vector.getDimension()));

        subtasks.add(subtask1);
        subtasks.add(subtask2);

        return subtasks;
    }
}

