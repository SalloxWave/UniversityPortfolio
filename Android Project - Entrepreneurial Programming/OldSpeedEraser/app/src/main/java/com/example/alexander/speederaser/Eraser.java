package com.example.alexander.speederaser;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.PointF;
import android.graphics.PorterDuff;
import android.graphics.PorterDuffXfermode;

/**
 * Created by Alex on 2017-11-02.
 */

public class Eraser
{
    private Path path;
    private Paint paint;
    private Bitmap image;
    private PointF position;

    public Eraser(Bitmap image, int gameBgColor, int strokeWidth)
    {
        this.image = image;
        path = new Path();
        position = new PointF();

        paint = new Paint();
        paint.setDither(true);
        paint.setColor(gameBgColor);
        paint.setStyle(Paint.Style.STROKE);
        paint.setStrokeJoin(Paint.Join.ROUND);
        paint.setStrokeCap(Paint.Cap.ROUND);
        paint.setStrokeWidth(image.getWidth());
        //paint.setXfermode(new PorterDuffXfermode(PorterDuff.Mode.CLEAR));
    }

    public void draw(Canvas canvas)
    {
        canvas.drawPath(path, paint);
        canvas.drawBitmap(image, position.x, position.y, null);
    }

    public void setPosition(float x, float y)
    {
        path.moveTo(x, y);
        position = new PointF(x, y);
    }

    public PointF getPosition()
    {
        return position;
    }

    public void move(float x, float y)
    {
        path.lineTo(x, y);
        position = new PointF(x, y);
    }

    public void reset()
    {
        path = new Path();
    }
}
