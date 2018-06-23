package com.example.alexander.speederaser;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.atomic.AtomicInteger;

import static android.content.ContentValues.TAG;

/**
 * Created by Alexander on 2017-10-26.
 */

public class MainThread extends Thread
{
    public boolean running = false;
    public int AverageFPS;

    private static final int INPUT_QUEUE_SIZE = 1000;
    private ArrayBlockingQueue<InputObject> inputQueue = new ArrayBlockingQueue<InputObject>(INPUT_QUEUE_SIZE);
    private Object inputQueueMutex = new Object();

    private int FPS = 30;
    private SurfaceHolder surfaceHolder;
    private GamePanel gamePanel;

    public static Canvas canvas;
    private Context context;

    public MainThread(SurfaceHolder surfaceHolder, GamePanel gamePanel, Context context)
    {
        super();
        this.surfaceHolder = surfaceHolder;
        this.gamePanel = gamePanel;
        this.context = context;
    }

    public void enqueInput(InputObject input)
    {
        synchronized(inputQueueMutex) {
            try {
                inputQueue.put(input);
            } catch (InterruptedException e) {
                Log.e(TAG, e.getMessage(), e);
            }
        }
    }

    public void handleTouch(MotionEvent event)
    {

    }

    private void processInput()
    {
        synchronized(inputQueueMutex)
        {
            //Process input queue
            ArrayBlockingQueue<InputObject> inputQueue = this.inputQueue;
            InputObject input;
            while (!inputQueue.isEmpty())
            {
                try
                {
                    input = inputQueue.take();
                    if (input.eventType == InputObject.EVENT_TYPE_KEY)
                    {
                        processKeyEvent(input);
                    }
                    else if (input.eventType == InputObject.EVENT_TYPE_TOUCH)
                    {
                        processMotionEvent(input);
                    }
                    //Return input to its list of pool queue
                    input.returnToPool();
                } catch (InterruptedException e) {
                    Log.e(TAG, e.getMessage(), e);
                }
            }
        }
    }

    private void processMotionEvent(InputObject input)
    {
        //MainActivity.Log("Processing motion event");
        gamePanel.handleMotionEvent(input);
    }

    private void processKeyEvent(InputObject input)
    {
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
                    //Update graphics according to new values
                    ((MainActivity)gamePanel.context).runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            //Update values of objects
                            gamePanel.update();
                            processInput();
                            gamePanel.draw(canvas);
                        }
                    });
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
                    catch(Exception e) {}
                }
            }
            //Milliseconds it took to update and draw the game once
            timeMillis = (System.nanoTime() - startTime) / 1000000;

            //Wait according to wanted time a iteration should take
            //(basically locks to target FPS)
            waitTime = targetTime - timeMillis;
            if (waitTime >= 0)  //To avoid unnecessary exception due to negative value
            {
                try { this.sleep(waitTime); }
                catch(Exception e) {}
            }

            //Update the total time to be able to decide average FPS
            totalTime+= System.nanoTime() - startTime;

            frameCount++;
            if (frameCount == FPS)
            {
                //Calculate average FPS and reset FPS variables
                //10^9 * frameCount / totalTime (Gives Frames/Second)
                AverageFPS = (int)(1000/((totalTime/frameCount)/1000000));
                frameCount = 0;
                totalTime = 0;
                MainActivity.Log("Average FPS: " + AverageFPS);
            }
        }
    }
}
