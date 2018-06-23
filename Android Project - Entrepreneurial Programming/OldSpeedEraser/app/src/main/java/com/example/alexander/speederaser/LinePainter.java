package com.example.alexander.speederaser;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.PorterDuff;
import android.graphics.PorterDuffXfermode;
import android.graphics.RectF;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;

import java.util.Random;

/**
 * Created by Alexander on 2017-10-26.
 */

public class LinePainter extends View
{
    private Paint paint;
    private Paint paintErase;
    private Path path;
    private Path pathErase;
    private Line line;
    private boolean finished = false;
    private Canvas canvas;

    public static final int DEFAULT_COLOR = Color.BLACK;
    public static final int DEFAULT_BG_COLOR = Color.WHITE;

    public LinePainter(Context context)
    {
        super(context);
        this.setBackgroundColor(DEFAULT_BG_COLOR);

        paint = new Paint();
        paint.setColor(DEFAULT_COLOR);
        paint.setStyle(Paint.Style.STROKE);
        paint.setStrokeJoin(Paint.Join.ROUND);
        paint.setStrokeCap(Paint.Cap.ROUND);
        paint.setStrokeWidth(10);

        paintErase = new Paint();
        //paintErase.setColor(DEFAULT_BG_COLOR);
        paintErase.setStyle(Paint.Style.STROKE);
        paintErase.setStrokeJoin(Paint.Join.ROUND);
        paintErase.setStrokeCap(Paint.Cap.ROUND);
        paintErase.setStrokeWidth(10);
        paintErase.setXfermode(new PorterDuffXfermode(PorterDuff.Mode.CLEAR));

        path = new Path();
        pathErase = new Path();
        //randomLine();
    }

    private void randomLine()
    {
        Random rnd = new Random();

        int x, y;
        for(int i = 0; i < 10; i++)
        {
            x = (int) rnd.nextDouble() * getWidth();
            y = (int) rnd.nextDouble() * getHeight();
            path.lineTo(x, y);
            canvas.drawPath(path, paint);
        }
    }

    @Override
    protected void onDraw(Canvas canvas)
    {
        super.onDraw(canvas);

        this.canvas = canvas;

        if (!finished)
        {
            MainActivity.Log("Draw paint");
            canvas.drawPath(path, paint);
        }
        else
        {
            MainActivity.Log("Eraser");
            canvas.drawPath(pathErase, paint);
        }
    }


    @Override
    public boolean onTouchEvent(MotionEvent event)
    {
        //MainActivity.Log("Size: " + event.getSize());
        switch (event.getAction())
        {
            case MotionEvent.ACTION_DOWN:
                path.moveTo(event.getX(), event.getY());
                pathErase.moveTo(event.getX(), event.getY());
                break;

            case MotionEvent.ACTION_MOVE:
                path.lineTo(event.getX(), event.getY());
                pathErase.lineTo(event.getX(), event.getY());
                invalidate();
                break;

            case MotionEvent.ACTION_UP:
                //RectF pathBound = new RectF();
                //path.computeBounds(pathBound, false);
                //MainActivity.Log("Height: " + pathBound.height());
                //MainActivity.Log("Width: " + pathBound.width());
                //boolean collision = pathBound.contains(event.getX(), event.getY());
                //MainActivity.Log("Collide: " + collision);
                paint.setXfermode(new PorterDuffXfermode(PorterDuff.Mode.CLEAR));
                finished = true;
                /*finished = true;
                path = new Path();
                line = new Line(Color.BLACK, 10, path);
                canvas.drawColor(0, PorterDuff.Mode.CLEAR);
                canvas.drawPath(line.path, paint);*/
                break;
        }

        return true;
    }
}
