package com.vodotiiets.primitives;

import java.util.Formatter;
import java.util.Locale;
import java.util.Random;

/**
 * Created by Denys Vodotiiets.
 */
public class Matrix {

    private int dimension;
    private int[][] array;

    public Matrix(int dimension) {
        this.dimension = dimension;
        array = new int[dimension][dimension];
    }

    public Matrix(Matrix other) {
        this.dimension = other.dimension;
        array = new int [this.dimension][this.dimension];
        for (int i = 0; i < this.dimension; i++) {
            for(int j = 0; j < this.dimension; j++) {
                this.array[i][j] = other.array[i][j];
            }
        }
    }

    public void generate(int maxValue) {
        Random generator = new Random();
        for (int i = 0; i < dimension; i++){
            for (int j = 0; j < dimension; j++){
                array[i][j] = generator.nextInt(maxValue);
            }
        }
    }

    public void fill() {
        for (int i = 0; i < dimension; i++){
            for (int j = 0; j < dimension; j++){
                array[i][j] = 1;
            }
        }
    }

    public int get(int i, int j) {
        return array[i][j];
    }

    public void set(int i, int j, int value) {
        array[i][j] = value;
    }

    public int getDimension() {
        return dimension;
    }

    @Override
    public String toString() {
        StringBuilder stringBuilder = new StringBuilder();
        Formatter formatter = new Formatter(stringBuilder, Locale.US);
        //String result = "";
        for (int i = 0; i < dimension; i++){
            for (int j = 0; j < dimension; j++){
                formatter.format("%5d", array[i][j]); //result += array[i][j] +"\t";
            }
            stringBuilder.append("\n");
            //result += "\n";
        }
        //return result;
        return stringBuilder.toString();
    }

}
