package com.example.alexander.speederaser;

import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.PathMeasure;
import android.graphics.PointF;
import android.graphics.RectF;

import java.util.ArrayList;

/**
 * Created by Alexander on 2017-10-26.
 */

public class Line
{
    public static int SCALE_FACTOR = 2;

    private ArrayList<RectF> lineBounds = new ArrayList<>();

    private PointF startPos;
    private PointF endPos;
    private Path path;  //Current path
    private Path finalPath;  //Path representing the line
    private PathMeasure pathMeasure;  //NOTE: Measures the final path
    private Paint paint;
    private float length;
    private boolean drawed;
    private float speed;  //Basically pixels per second to draw
    private float currentDist;  //Current distance drawed
    private float[] coordinates = new float[2];

    public Line(PointF startPos, PointF endPos, int color, int strokeWidth)
    {
        this.startPos = startPos;
        this.endPos = endPos;

        path = new Path();
        path.moveTo(startPos.x, endPos.y);
        currentDist = 0;

        finalPath = new Path();
        finalPath.moveTo(startPos.x, startPos.y);
        finalPath.lineTo(endPos.x, endPos.y);
        pathMeasure = new PathMeasure(finalPath, false);
        length = pathMeasure.getLength();

        paint = new Paint();
        paint.setDither(true);
        paint.setStyle(Paint.Style.STROKE);
        paint.setStrokeJoin(Paint.Join.ROUND);
        paint.setStrokeCap(Paint.Cap.ROUND);
        paint.setColor(color);
        paint.setStrokeWidth(strokeWidth);
    }

    public Line(Path path, int color, int strokeWidth, int speed)
    {
        //Configure information about the final path
        this.finalPath = path;
        pathMeasure = new PathMeasure(finalPath, false);
        startPos = calculateStartPos();
        endPos = calculateEndPos();
        length = pathMeasure.getLength();

        //Configure information about the current path
        this.path = new Path();
        this.path.moveTo(startPos.x, startPos.y);
        currentDist = 0;
        this.speed = speed;

        //Configure paint for the line
        paint = new Paint();
        paint.setDither(true);
        paint.setStyle(Paint.Style.STROKE);
        paint.setStrokeJoin(Paint.Join.ROUND);
        paint.setStrokeCap(Paint.Cap.ROUND);
        paint.setColor(color);
        paint.setStrokeWidth(strokeWidth);
    }

    public Line(String filePath)
    {
        //Get everything needed from the file and do as above
    }

    private PointF calculateStartPos()
    {
        float[] coordinates = new float[2];
        pathMeasure.getPosTan(pathMeasure.getLength(), coordinates, null);
        return new PointF(coordinates[0], coordinates[1]);
    }

    private PointF calculateEndPos()
    {
        float[] coordinates = new float[2];
        pathMeasure.getPosTan(0, coordinates, null);
        return new PointF(coordinates[0], coordinates[1]);
    }

    public void saveToFile(String filePath)
    {
        //Iterate through getPathInPoints() and store x and y coordinates in a file
    }

    private PointF[] getPathInPoints(int pointCount)
    {
        return new PointF[pointCount];
    }

    public PointF getPosition()
    {
        return new PointF(coordinates[0], coordinates[1]);
    }

    public void update()
    {
        //Don't update if line already drawed
        if (drawed) { return; }

        //Line is finished drawing
        if (currentDist > length)
        {
            drawed = true;
            currentDist = length;
        }

        //Get coordinates of next point to draw to and draw
        //NOTE: The coordinates before those could be used to decide camera position
        coordinates = new float[2];
        pathMeasure.getPosTan(currentDist, coordinates, null);
        path.lineTo(coordinates[0], coordinates[1]);

        //Update next position to draw to according to speed
        currentDist +=speed;
    }

    public void draw(Canvas canvas)
    {
        canvas.drawPath(path, paint);
    }

    private void initLinePixels()
    {
        RectF rectPath = new RectF();
        finalPath.computeBounds(rectPath, true);

        //Get x-steps for every y
        float xStep = rectPath.width() / rectPath.height();
        float left = rectPath.left;
        float right = rectPath.right - rectPath.width() + xStep;
        //Diagonal is leaned to the left
        if ((endPos.y > startPos.y && endPos.x > startPos.x) |
                startPos.y > endPos.y && startPos.x > endPos.x)
        {
            left = rectPath.left + rectPath.width() - xStep;
            right = rectPath.right;
            xStep*=-1;
        }

        RectF rect;
        //MainActivity.Log("Bottom: " + rectPath.bottom);
        //MainActivity.Log("Bottom + height: " + rectPath.bottom + rectPath.height());
        for(float bottom = rectPath.bottom; bottom < rectPath.bottom + rectPath.height(); bottom++)
        {
            rect = new RectF(left, bottom - 1, right, bottom);
            lineBounds.add(rect);

            left+=xStep;
            right+=xStep;
        }
        //for (RectF rect2 : lineBounds)
        //{
            //MainActivity.Log("Left: " + rect2.left);
            //MainActivity.Log("Right: " + rect2.right);
        //}
        //MainActivity.Log("Count: " + lineBounds.size());
    }

    public boolean intersects(Line line)
    {
        //MainActivity.Log("INTERSECT");
        //MainActivity.Log("line count: " + line.lineBounds.size());
        //MainActivity.Log("this line count: " + this.lineBounds.size());
        for (RectF rect : line.lineBounds)
        {
            for(RectF thisRect : this.lineBounds)
            {
                //MainActivity.Log("Rect width: " + rect.width());
                //MainActivity.Log("Rect height: " + rect.height());
                //MainActivity.Log("This rect width: " + thisRect.width());
                //MainActivity.Log("This rect height: " + thisRect.height());

                if (rect.intersect(thisRect))
                {
                    MainActivity.Log("TRUE!");
                    return true;
                }
            }
        }
        //MainActivity.Log("FALSE!");
        return false;
    }
}
