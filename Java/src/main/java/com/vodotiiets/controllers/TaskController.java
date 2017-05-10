package com.vodotiiets.controllers;

import com.vodotiiets.directors.Director;

/**
 * Created by Denys Vodotiiets.
 */
public class TaskController {

    private int inputCount;
    private int aCount;
    private int bCount;
    private int calculateCount;

    public synchronized void signalInputDone() {
        if(++inputCount == Director.inputCount) {
            notifyAll();
        }
    }

    public synchronized void waitInput() {
        while (inputCount < Director.inputCount) {
            try {
                wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }


    public synchronized void signalCalculateADone() {
        if(++aCount == Director.P) {
            notifyAll();
        }
    }

    public synchronized void waitCalculateA() {
        while (aCount < Director.P){
            try {
                wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }


    public synchronized void signalCalculateBDone() {
        if (++bCount == Director.P) {
            notifyAll();
        }
    }

    public synchronized void waitCalculateB() {
        while (bCount < Director.P) {
            try {
                wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }


    public synchronized void signalCalculateDone() {
        if (++calculateCount == Director.P - 1) {
            notifyAll();
        }
    }

    public synchronized void waitCalculate() {
        while (calculateCount < Director.P - 1) {
            try {
                wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
