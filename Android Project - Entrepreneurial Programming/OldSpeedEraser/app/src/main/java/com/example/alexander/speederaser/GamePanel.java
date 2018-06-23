package com.example.alexander.speederaser;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Camera;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Region;
import android.graphics.Typeface;
import android.os.AsyncTask;
import android.os.Environment;
import android.os.RemoteException;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.ImageView;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Random;
import java.util.concurrent.ArrayBlockingQueue;

import static android.content.ContentValues.TAG;

/**
 * Created by Alexander on 2017-10-26.
 */

public class GamePanel extends SurfaceView implements SurfaceHolder.Callback
{
    public Context context;

    private static final int INPUT_QUEUE_SIZE = 1000;
    private ArrayBlockingQueue<InputObject> inputObjectPool;

    private MainThread thread;
    private final int DEFAULT_BG_COLOR = Color.WHITE;

    public int width;
    public int height;

    private Line line;
    private Eraser eraser;
    private Camera camera;

    private long eventTime = 0;

    public GamePanel(Context context)
    {
        super(context);
        this.context = context;
        setDrawingCacheEnabled(true);

        //Get initial screen dimensions (full screen)
        width = context.getResources().getDisplayMetrics().widthPixels;
        height = context.getResources().getDisplayMetrics().heightPixels;

        //Make game panel able to handle events
        getHolder().addCallback(this);
        setFocusable(true);

        setBackgroundColor(DEFAULT_BG_COLOR);

        createInputObjectPool();
    }

    private void createInputObjectPool()
    {
        inputObjectPool = new ArrayBlockingQueue<InputObject>(INPUT_QUEUE_SIZE);
        for (int i = 0; i < INPUT_QUEUE_SIZE; i++)
        {
            inputObjectPool.add(new InputObject(inputObjectPool));
        }
    }

    private Line generateRandomLine(int pathCount, int color, int strokeWidth, int speed)
    {
        Random rnd = new Random();
        Path path = new Path();
        int x = (int) (rnd.nextDouble() * width);
        int y = (int) (rnd.nextDouble() * height);
        path.moveTo(x, y);
        for(int i = 0; i < pathCount; i++)
        {
            x = (int) (rnd.nextDouble() * width);
            y = (int) (rnd.nextDouble() * height);
            path.lineTo(x, y);
        }
        return new Line(path, color, strokeWidth, speed);
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height)
    {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder)
    {
        //Try to close down main loop until success
        boolean retry = true;
        int counter = 0;
        while (retry && counter < 1000)
        {
            counter++;
            try
            {
                thread.running = false;
                thread.join();
                retry = false;
                thread = null;
            }
            catch(InterruptedException e) {}
        }
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder)
    {
        line = generateRandomLine(5, Color.BLUE, 30, 10);

        //Create resized eraser according to scale factor of the line
        Bitmap eraserImg = BitmapFactory.decodeResource(getResources(), R.drawable.rubbergum);
        eraserImg = Bitmap.createScaledBitmap(eraserImg, eraserImg.getWidth() / Line.SCALE_FACTOR,
                eraserImg.getHeight() / Line.SCALE_FACTOR, false);
        eraser = new Eraser(eraserImg, DEFAULT_BG_COLOR, 40);

        camera = new Camera();

        thread = new MainThread(getHolder(), this, context);
        thread.running = true;
        thread.start();
    }

    public void update()
    {
        line.update();
    }

    @Override
    public void draw(Canvas canvas)
    {
        if (canvas != null)
        {
            super.draw(canvas);

            final int savedState = canvas.save();

            //Zoom where the line will be drawn
            float xPos = -line.getPosition().x * Line.SCALE_FACTOR + this.width / 2;
            float yPos = -line.getPosition().y * Line.SCALE_FACTOR + this.height / 2;
            //MainActivity.Log(xPos + ":" + yPos);
            //canvas.translate(line.getCurrentPos().x, line.getCurrentPos().y);

            canvas.scale(Line.SCALE_FACTOR, Line.SCALE_FACTOR, line.getPosition().x, line.getPosition().y);
            line.draw(canvas);
            eraser.draw(canvas);
            //canvas.translate(-line.getPosition().x, -line.getPosition().y);
            canvas.restoreToCount(savedState);

            drawFPS(canvas);
            this.invalidate();

        }
    }

    @Override
    public boolean onTouchEvent(final MotionEvent event)
    {
        // Cap amounts of motion events per second to wanted FPS (60 (16), 30(32))
        if ((System.nanoTime() - eventTime)/1000000 < 33)
        {
            return false;
        }

        //this.event = event;
        eventTime = System.nanoTime();
        Thread t = new Thread()
        {
            public void run()
            {
                try
                {
                    //Process history events if there is any
                    int hist = event.getHistorySize();
                    if (hist > 0)
                    {
                        InputObject input;
                        //Problem: History size is too large
                        MainActivity.Log("History size: " + hist);
                        //Add inputs to thread input queue
                        for (int i = 0; i < hist; i++)
                        {
                            input = inputObjectPool.take();
                            input.useEventHistory(event, i);  //Configure input object based on motion event object
                            thread.enqueInput(input);
                        }
                    }
                    //Process current event
                    InputObject input = inputObjectPool.take();
                    input.useEvent(event);
                    thread.enqueInput(input);
                } catch (InterruptedException e) {}


                //try { Thread.sleep(32); }
                //catch (InterruptedException e) { }
            }
        };
        t.start();

        return true;
    }

    public void handleMotionEvent(InputObject input)
    {
        //MainActivity.Log(input.x + ":" + input.y);
        //As line.y increases (far down), input y needs to increase by difference between line.y and middle

        float xDiff = line.getPosition().x - (this.width / 2f);
        float yDiff = line.getPosition().y - (this.height / 2f);
        //MainActivity.Log(xDiff + ":" + yDiff);

        //line.getCurrentPos().x
        float xPos = input.x;
        float yPos = input.y;
        MainActivity.Log(xPos + ":" + yPos);
        switch (input.action)
        {
            case InputObject.ACTION_TOUCH_DOWN:
                eraser.setPosition(xPos, yPos);
                break;

            case InputObject.ACTION_TOUCH_MOVE:
                eraser.move(xPos, yPos);
                break;

            case InputObject.ACTION_TOUCH_UP:
                //saveCanvasToFile();
                line = generateRandomLine(5, Color.BLUE, 30, 10);
                eraser.reset();
                break;
        }
    }

    private void saveCanvasToFile()
    {
        //invalidate();
        //line.draw(MainThread.canvas);
        ((MainActivity)context).saveToFile(getDrawingCache());
    }

    private void drawFPS(Canvas canvas)
    {
        Paint paint = new Paint();
        paint.setColor(Color.BLACK);
        paint.setTextSize(100);
        paint.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.BOLD));
        canvas.drawText("FPS: " + thread.AverageFPS, 0, paint.getTextSize(), paint);
    }

    /*    private void printPathPoints()
    {
        //paintEraser.setColor(color);
        for(PointF fp : getPathPoints())
        {
            MainActivity.Log(fp.x + ":" + fp.y);
            //updateEraserPath((int)fp.x + 100, (int)fp.y + 100);
        }
        PathMeasure pm = new PathMeasure(pathEraser, false);
        MainActivity.Log("Length: " + pm.getLength());
    }*/

    /*    private PointF[] getPathPoints()
    {
        PointF[] pointArray = new PointF[20];
        PathMeasure pathMeasure = new PathMeasure(pathEraser, false);
        float length = pathMeasure.getLength();
        float distance = 0f;
        float speed = length / 20;
        int counter = 0;
        float[] coordinates = new float[2];

        while ((distance < length) && (counter < 20))
        {
            pathMeasure.getPosTan(distance, coordinates, null);
            pointArray[counter] = new PointF(coordinates[0], coordinates[1]);
            counter++;
            distance+=speed;
        }

        return pointArray;
    }*/

}
