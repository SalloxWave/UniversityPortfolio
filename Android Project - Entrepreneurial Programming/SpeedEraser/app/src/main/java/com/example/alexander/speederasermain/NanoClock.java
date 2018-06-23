package com.example.alexander.speederasermain;

/**
 * Created by Alex on 2018-01-05.
 */

public class NanoClock {
    private long startTime;
    private long elapsed;

    public NanoClock() {
        startTime = 0;
        elapsed = 0;
    }

    public void update() {
        elapsed = System.nanoTime() - startTime;
    }

    public void restart() {
        startTime = System.nanoTime();
        elapsed = 0;
    }

    public void setElapsed(long elapsed) {
        this.elapsed = elapsed;
    }

    public long getNano() {
        return elapsed;
    }

    public long getMilli() {
        return (elapsed / 1000000) - (getSeconds() * 1000);
    }

    public long getSeconds() {
        return elapsed / 1000000000;
    }

    public String toString(boolean displayZero) {
        if (!displayZero && elapsed == 0) {
            return "None";
        }

        long milli = getMilli();
        String milliText = String.valueOf(milli);

        //Fill milliseconds with zeros if too small
        if (milli < 100) {
            milliText+="0";
            if (milli < 10) {
                milliText+="0";
            }
        }

        return getSeconds() + ":" + milliText;
    }

    public static String toString(long nanoSeconds, boolean displayZero) {
        NanoClock nanoClock = new NanoClock();
        nanoClock.setElapsed(nanoSeconds);
        return nanoClock.toString(displayZero);
    }
}