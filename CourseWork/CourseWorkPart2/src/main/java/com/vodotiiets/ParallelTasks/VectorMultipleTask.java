package com.vodotiiets.ParallelTasks;

import com.vodotiiets.directors.Director;
import com.vodotiiets.primitives.Vector;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.RecursiveTask;

/**
 * Created by Denys Vodotiiets.
 */
public class VectorMultipleTask extends RecursiveTask<Integer> {

    private Vector vector1;
    private Vector vector2;
    private int dimension;

    public VectorMultipleTask(Vector vector1, Vector vector2) {
        this.vector1 = vector1;
        this.vector2 = vector2;

        dimension = vector1.getDimension();
    }

    @Override
    protected Integer compute() {
        if(dimension > Director.getH() / Director.getP()) {
            List<VectorMultipleTask> subtasks = createSubtasks();

            for(VectorMultipleTask subtask : subtasks) {
                subtask.fork();
            }

            int result = 0;
            for(VectorMultipleTask subtask : subtasks) {
                result += subtask.join();
            }

            return result;
        } else {
            int result = 0;
            for (int i = 0; i < dimension; i++) {
                result += vector1.get(i) * vector2.get(i);
            }
            return result;
        }
    }

    private List<VectorMultipleTask> createSubtasks() {
        List<VectorMultipleTask> subtasks = new ArrayList<>();

        VectorMultipleTask subtask1 = new VectorMultipleTask(vector1.getPart(0, vector1.getDimension() / 2),
                                        vector2.getPart(0, vector2.getDimension() / 2));
        VectorMultipleTask subtask2 = new VectorMultipleTask(vector1.getPart(vector1.getDimension() / 2 + 1,
                vector1.getDimension()), vector2.getPart(vector2.getDimension() / 2 + 1, vector2.getDimension()));

        subtasks.add(subtask1);
        subtasks.add(subtask2);

        return subtasks;
    }
}
