package com.example.alexander.gametest;

import android.graphics.Canvas;
import android.view.SurfaceHolder;

/**
 * Created by Alexander on 2017-10-16.
 */

public class MainThread extends Thread
{
    private int FPS = 30;
    private double averageFPS;
    private boolean running;
    private SurfaceHolder surfaceHolder;
    private GamePanel gamePanel;

    public static Canvas canvas;

    public MainThread(SurfaceHolder surfaceHolder, GamePanel gamePanel)
    {
        super();
        this.surfaceHolder = surfaceHolder;
        this.gamePanel = gamePanel;
    }

    @Override
    public void run()
    {
        long startTime;
        long timeMillis;
        long waitTime;
        long totalTime = 0;
        long frameCount = 0;
        long targetTime = 1000/FPS;  //Wanted time for one iteration of the gameloop

        while(running)
        {
            startTime = System.nanoTime();
            canvas = null;

            try
            {
                //Try locking the canvas for pixel editing
                canvas = this.surfaceHolder.lockCanvas();

                //Synchronized: Make sure thread in gamepanel and thread here are...
                //...correctly  writing and reading to the same variables
                synchronized (surfaceHolder)
                {
                    //Update values of objects
                    this.gamePanel.update();
                    //Update graphics according to new values
                    this.gamePanel.draw(canvas);
                }
            }
            catch(Exception e){ e.printStackTrace(); }
            finally
            {
                if (canvas != null)
                {
                    try
                    {
                        //Unlock canvas to enable surfaceview to creating, destroying...
                        //...or modifying the surface while it is being drawn
                        surfaceHolder.unlockCanvasAndPost(canvas);
                    }
                    catch(Exception e) { e.printStackTrace(); }
                }
            }
            //Nano seconds it took to update and draw the game once
            timeMillis = (System.nanoTime() - startTime) / 1000000;

            //Wait according to wanted time a iteration should take
            //(basically locks to target FPS)
            waitTime = targetTime - timeMillis;
            if (waitTime >= 0)  //To avoid unnecessary exception due to negative value
            {
                try { this.sleep(waitTime); }
                catch(Exception e) { e.printStackTrace(); }
            }

            //Update the total time to be able to decide average FPS
            totalTime+= System.nanoTime() - startTime;

            frameCount++;
            if (frameCount == FPS)
            {
                //Calculate average FPS and reset FPS variables
                averageFPS = 1000/((totalTime/frameCount)/1000000);
                frameCount = 0;
                totalTime = 0;
                System.out.println("Average FPS: " + averageFPS);
            }
        }
    }

    public void setRunning(boolean running) { this.running = running; }

}