package com.vodotiiets.workers;

import com.vodotiiets.ParallelTasks.MaxTask;
import com.vodotiiets.ParallelTasks.VectorMultipleTask;
import com.vodotiiets.controllers.TaskController;
import com.vodotiiets.directors.Director;
import com.vodotiiets.primitives.Matrix;

import java.util.concurrent.ForkJoinPool;

/**
 * Created by Denys Vodotiiets.
 */
public class TaskWorker implements Runnable {
    private int tid;
    private TaskController monitor;

    private Matrix MRcopy;
    private int aCopy;
    private int sCopy;

    public TaskWorker(int tid, TaskController monitor) {
        this.monitor = monitor;
        this.tid = tid;
    }

    @Override
    public void run() {
        if (tid == 0) {
            monitor.inputMR();
            Director.getMA().generate(Director.getMaxValue());
            Director.getB().generate(Director.getMaxValue());

            monitor.signalInputDone();
        }

        if (tid == Director.getP() - 1) {
            Director.getC().generate(Director.getMaxValue());
            Director.getMO().generate(Director.getMaxValue());
            Director.getMK().generate(Director.getMaxValue());
            Director.getZ().generate(Director.getMaxValue());

            monitor.signalInputDone();
        }

        monitor.waitInput();

        int startIndex = tid * Director.getH();
        int endIndex = (tid != Director.getP() - 1) ? (tid + 1) * Director.getH() : Director.getN();

        MaxTask maxTask = new MaxTask(Director.getZ().getPart(startIndex, endIndex));
        int a = new ForkJoinPool().invoke(maxTask);
        monitor.setA(a);

        VectorMultipleTask vectorMultipleTask = new VectorMultipleTask(Director.getB().getPart(startIndex, endIndex),
                                                                        Director.getC().getPart(startIndex, endIndex));
        int s = new ForkJoinPool(Director.getP()).invoke(vectorMultipleTask);

        monitor.setS(s);

        monitor.signalPreparationDone();
        monitor.waitPreparation();

        MRcopy = monitor.getMR();
        aCopy = monitor.getA();
        sCopy = monitor.getS();

        for (int i = 0; i < Director.getN(); i++) {
            for (int j = startIndex; j < endIndex; j++) {
                int sum = 0;
                for (int k = 0; k < Director.getN(); k++) {
                    sum += MRcopy.get(i, k) * Director.getMK().get(k, j);
                }

                int value = sCopy * Director.getMO().get(i, j) + aCopy * sum;
                Director.getMA().set(i, j, value);
            }
        }

        if (tid == 0) {
            monitor.waitCalculation();
        } else {
            monitor.signalCalculationDone();
        }

        if (tid == 0) {
            Director.setEndTime(System.currentTimeMillis());
            System.out.println("All threads ended calculations. Result time(ms): " + Director.getTime());

            if(Director.getMA().getDimension() < 10) {
                System.out.println("Result MA:\n" + Director.getMA());
            } else {
                System.out.println("Result was calculated. Matrix is too large");
            }
        }

    }
}
