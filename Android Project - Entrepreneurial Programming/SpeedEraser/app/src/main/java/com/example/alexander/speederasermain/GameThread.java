package com.example.alexander.speederasermain;
import android.graphics.Canvas;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

import java.util.concurrent.ArrayBlockingQueue;

import static android.content.ContentValues.TAG;

/**
 * Created by Alexander on 2017-10-26.
 */

public class GameThread extends Thread {
    private boolean running = false;
    private int averageFPS;
    private final int FPS = 30;

    private SurfaceHolder surfaceHolder;
    private GamePanel gamePanel;
    private static Canvas canvas;

    private static final int INPUT_QUEUE_SIZE = 1000;
    private ArrayBlockingQueue<InputObject> inputQueue = new ArrayBlockingQueue<>(INPUT_QUEUE_SIZE);
    private final Object inputQueueMutex = new Object();

    public GameThread(SurfaceHolder surfaceHolder, GamePanel gamePanel) {
        super();
        this.surfaceHolder = surfaceHolder;
        this.gamePanel = gamePanel;
    }

    public void setRunning(boolean running) {
        this.running = running;
    }

    public int getAverageFPS() {
        return averageFPS;
    }

    public void enqueInput(InputObject input) {
        //Add input object in game loop queue
        synchronized(inputQueueMutex) {
            try {
                inputQueue.put(input);
            } catch (InterruptedException e) {
                Log.e(TAG, e.getMessage(), e);
            }
        }
    }

    private void processInput() {
        synchronized(inputQueueMutex) {
            ArrayBlockingQueue<InputObject> inputQueue = this.inputQueue;
            InputObject input;

            //Go through input queue and process input objects
            while (!inputQueue.isEmpty()) {
                try {
                    input = inputQueue.take();
                    if (input.eventType == InputObject.EVENT_TYPE_KEY) {
                        processKeyEvent(input);
                    }
                    else if (input.eventType == InputObject.EVENT_TYPE_TOUCH) {
                        processMotionEvent(input);
                    }
                    input.returnToPool();
                } catch (InterruptedException e) {
                    Log.e(TAG, e.getMessage(), e);
                }
            }
        }
    }

    private void processMotionEvent(InputObject input) {
        //Let the gamepanel update according to input object
        gamePanel.handleMotionEvent(input);
    }

    private void processKeyEvent(InputObject input) {
    }

    @Override
    public void run() {
        long startTime;
        long timeMillis;
        long waitTime;
        long totalTime = 0;
        long frameCount = 0;
        long targetTime = 1000/FPS;  //Wanted time for one iteration of the gameloop

        while(running) {
            startTime = System.nanoTime();

            try {
                //Try locking the canvas for pixel editing
                canvas = this.surfaceHolder.lockCanvas();

                //Synchronized: Make sure thread in gamepanel and thread here are...
                //...correctly writing and reading to the same variables
                synchronized (surfaceHolder) {
                    //Make sure you are drawing on UI thread
                    ((GameActivity)gamePanel.context).runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            gamePanel.update();     //Update values of objects
                            processInput();         //Process event inputs
                            gamePanel.draw(canvas); //Redraw gamepanel with updated values
                        }
                    });
                }
            }
            catch(Exception e){ e.printStackTrace(); }
            finally {
                if (canvas != null) {
                    try {
                        //Unlock canvas to enable surfaceview to creating, destroying...
                        //...or modifying the surface while it is being drawn
                        surfaceHolder.unlockCanvasAndPost(canvas);
                    }
                    catch(Exception e) { e.printStackTrace(); }
                }
            }

            //Milliseconds it took to update and draw the game once
            timeMillis = (System.nanoTime() - startTime) / 1000000;

            //Wait according to wanted time a iteration should take to lock to target FPS
            waitTime = targetTime - timeMillis;
            if (waitTime >= 0) {  //To avoid unnecessary exception due to negative value
                try { this.sleep(waitTime); }
                catch(Exception e) { e.printStackTrace(); }
            }

            //Update the total time to be able to decide average FPS
            totalTime+= System.nanoTime() - startTime;

            frameCount++;
            if (frameCount == FPS) {
                //Calculate average FPS and reset FPS variables
                //10^9 * frameCount / totalTime (Gives Frames/Second)
                averageFPS = (int)(1000/((totalTime/frameCount)/1000000));
                frameCount = 0;
                totalTime = 0;
            }
        }
    }
}
