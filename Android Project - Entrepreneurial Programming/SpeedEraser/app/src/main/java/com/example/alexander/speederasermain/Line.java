package com.example.alexander.speederasermain;

import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.PathMeasure;
import android.graphics.PointF;
import android.graphics.PorterDuff;
import android.graphics.PorterDuffXfermode;
import android.graphics.RectF;

import java.util.ArrayList;

/**
 * Created by Alexander on 2017-10-26.
 */

public class Line {
    private Path path;  //Current path
    private Path finalPath;  //Path representing the line
    private PathMeasure pathMeasure;  //NOTE: Measures the final path
    private Paint paint;
    private int strokeWidth;
    private float length;
    private boolean drawed;
    private float speed;  //Basically pixels per second to draw
    private float currentDist;  //Current distance drawed
    private float[] coordinates = new float[2];

    public Line(Path path, int color, int strokeWidth, int speed) {
        this.strokeWidth = strokeWidth;
        this.speed = speed;
        currentDist = 0;

        //Configure information about the final path
        this.finalPath = path;
        pathMeasure = new PathMeasure(finalPath, false);
        length = pathMeasure.getLength();

        //No speed means line should be painted out instantly...
        if (speed == 0) {
            this.path = finalPath;
        }
        //...otherwise move to start position
        else {
            this.path = new Path();
            PointF startPos = calculateStartPos();
            this.path.moveTo(startPos.x, startPos.y);
        }

        //Configure paint for the line
        paint = new Paint();
        paint.setDither(true);
        paint.setStyle(Paint.Style.STROKE);
        paint.setStrokeJoin(Paint.Join.ROUND);
        paint.setStrokeCap(Paint.Cap.ROUND);
        paint.setColor(color);
        paint.setStrokeWidth(strokeWidth);
    }

    public int getStrokeWidth() {
        return strokeWidth;
    }

    public float getLength() {
        return pathMeasure.getLength();
    }

    public PathMeasure getPathMeasure() {
        return pathMeasure;
    }

    public void setColor(int color) {
        paint.setColor(color);
    }

    public PointF calculateStartPos() {
        float[] coordinates = new float[2];
        pathMeasure.getPosTan(0, coordinates, null);
        return new PointF(coordinates[0], coordinates[1]);
    }

    public PointF calculateEndPos() {
        float[] coordinates = new float[2];
        pathMeasure.getPosTan(pathMeasure.getLength(), coordinates, null);
        return new PointF(coordinates[0], coordinates[1]);
    }

    public PointF getPosition()
    {
        return new PointF(coordinates[0], coordinates[1]);
    }

    public void update() {
        //Don't update if line already drawed
        if (drawed) { return; }

        //Line is finished drawing
        if (currentDist > length || speed == 0) {
            drawed = true;
            currentDist = length;
        }

        //Get coordinates of next point to draw to and draw
        coordinates = new float[2];
        pathMeasure.getPosTan(currentDist, coordinates, null);
        path.lineTo(coordinates[0], coordinates[1]);

        //Update next position to draw to according to speed
        currentDist +=speed;
    }

    public void draw(Canvas canvas) {
        canvas.drawPath(path, paint);
    }
}
