package com.vodotiiets.controllers;

import com.vodotiiets.directors.Director;
import com.vodotiiets.primitives.Matrix;

/**
 * Created by Denys Vodotiiets.
 */
public class DataController {

    private int a;
    private int b;
    private Matrix MR;

    public DataController() {
        a = 0;
        b = Integer.MIN_VALUE;
        MR = new Matrix(Director.N);
    }

    public synchronized void calculateA(int a) {
        this.a += a;
    }

    public int copyA() {
        return a;
    }


    public synchronized void calculateB(int b) {
        this.b = b > this.b ? b : this.b;
    }

    public int copyB() {
        return b;
    }


    public synchronized void inputMR() {
        MR.fill();
    }

    public Matrix copyMR() {
        return new Matrix(MR);
    }


}
