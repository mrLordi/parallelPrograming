package com.vodotiiets.controllers;


import com.vodotiiets.directors.Director;
import com.vodotiiets.primitives.Matrix;


/**
 * Created by Denys Vodotiiets.
 */
public class TaskController {

    private int inputCount;
    private int preparationCount;
    private int calculationCount;

    private int a = Integer.MIN_VALUE;
    private int s;
    private Matrix MR = new Matrix(Director.getN());

    public synchronized void waitInput() {
        while (inputCount < 2) {
            try {
                wait();
            } catch (InterruptedException e) {
                System.out.println("Error has occurred in " + getClass().getSimpleName()
                        + "when running waitInput()!" + e.getMessage());
            }
        }
    }

    public synchronized void waitCalculation() {
        while (calculationCount < Director.getP() - 1) {
            try {
                wait();
            } catch (InterruptedException e) {
                System.out.println("Error has occurred in " + getClass().getSimpleName()
                        + "when running waitCalculation()!" + e.getMessage());
            }
        }
    }

    public synchronized void waitPreparation() {
        while (preparationCount < Director.getP()) {
            try {
                wait();
            } catch (InterruptedException e) {
                System.out.println("Error has occurred in " + getClass().getSimpleName()
                        + "when running waitPreparation()!" + e.getMessage());
            }
        }
    }

    public synchronized void inputMR() {
        MR.generate(Director.getMaxValue());
    }

    public synchronized void setA(int a) {
        if(a > this.a) {
            this.a = a;
        }
    }

    public synchronized void setS(int s) {
        this.s += s;
    }

    public synchronized Matrix getMR() {
        return new Matrix(MR);
    }

    public synchronized int getA() {
        return a;
    }

    public synchronized int getS() {
        return s;
    }

    public synchronized void signalInputDone() {
        inputCount++;
        if(inputCount == 2) {
            notifyAll();
        }
    }

    public synchronized void signalCalculationDone() {
        calculationCount++;
        if (calculationCount == Director.getP() - 1) {
            notifyAll();
        }
    }

    public synchronized void signalPreparationDone() {
        preparationCount++;
        if (preparationCount == Director.getP()) {
            notifyAll();
        }
    }


}
