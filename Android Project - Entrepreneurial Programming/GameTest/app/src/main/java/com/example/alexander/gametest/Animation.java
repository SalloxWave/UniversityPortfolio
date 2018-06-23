package com.example.alexander.gametest;

import android.graphics.Bitmap;

/**
 * Created by Alexander on 2017-10-16.
 */

public class Animation
{
    private Bitmap[] frames;
    private int currentFrame;
    private long startTime;
    private long delay;
    private boolean playedOnce;

    public void setFrames(Bitmap[] frames)
    {
        this.frames = frames;
        currentFrame = 0;
        startTime = System.nanoTime();
    }

    public void setDelay(long d) {delay = d;}
    //In case you want to manually set the frame
    public void setFrame(int i) {currentFrame = i;}

    public void update()
    {
        long elapsed = (System.nanoTime() - startTime)/1000000;

        //Check if you want to go to next animation
        if (elapsed > delay)
        {
            currentFrame++;
            startTime = System.nanoTime();
        }

        //When on last frame, reset to first animation frame
        if (currentFrame == frames.length)
        {
            currentFrame = 0;
            playedOnce = true;
        }
    }

    public Bitmap getImage()
    {
        return frames[currentFrame];
    }
    public int getFrame() {return currentFrame;}
    public boolean playedOnce() {return playedOnce;}
}
