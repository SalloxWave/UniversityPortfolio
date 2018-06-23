package com.example.alexander.speederasermain;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.PathMeasure;
import android.graphics.PointF;
import android.graphics.Rect;
import android.graphics.Region;

/**
 * Created by Alex on 2017-11-02.
 */

public class Eraser {
    private Path path;
    private Paint paint;
    private Bitmap image;
    private PointF position;
    private int strokeWidth;

    public Eraser(Bitmap image, int gameBgColor) {
        this.image = image;
        path = new Path();
        position = new PointF();
        strokeWidth = image.getWidth();

        paint = new Paint();
        paint.setDither(true);
        paint.setColor(gameBgColor);
        paint.setStyle(Paint.Style.STROKE);
        paint.setStrokeJoin(Paint.Join.ROUND);
        paint.setStrokeCap(Paint.Cap.ROUND);
        paint.setStrokeWidth(strokeWidth);
    }

    private PointF calculateStartPos() {
        float[] coordinates = new float[2];
        PathMeasure pathMeasure = new PathMeasure(this.path, false);
        pathMeasure.getPosTan(0, coordinates, null);
        return new PointF(coordinates[0], coordinates[1]);
    }

    public void setColor(int color) {
        paint.setColor(color);
    }

    public void draw(Canvas canvas) {
        canvas.drawPath(path, paint);
        canvas.drawBitmap(image, position.x - image.getWidth() / 2f,
                position.y - image.getHeight() / 2f, null);
    }

    public void setPosition(float x, float y) {
        path.moveTo(x, y);
        position = new PointF(x, y);
    }

    public PointF getPosition()
    {
        return position;
    }

    public void move(float x, float y) {
        path.lineTo(x, y);
        position = new PointF(x, y);
    }

    public void reset() {
        path = new Path();
    }

    public boolean hasErased(Line line, Canvas canvas) {
        //How many parts of the paths you should compare
        float eraseParts = 25f;
        float lineParts = 50f;

        //Eraser initial values
        Path eraserPath;
        PathMeasure pathMeasure = new PathMeasure(this.path, false);
        float eraserLength = pathMeasure.getLength();
        PointF eraserStartPos = calculateStartPos();
        float[] oldEraserCoordinates;
        float[] eraserCoordinates;

        //Line initial values
        Path linePath;
        float lineLength = line.getLength();
        PointF lineStartPos = line.calculateStartPos();
        float[] oldLineCoordinates = new float[] {lineStartPos.x, lineStartPos.y};
        float[] lineCoordinates;

        //Loop through according to amount of line parts
        for (float i = 1; i <= lineParts; i++) {
            //Calculate end of the next line part
            lineCoordinates = new float[2];
            line.getPathMeasure().getPosTan(i * (lineLength / lineParts), lineCoordinates, null);

            int lineWidth = line.getStrokeWidth();
            int x = lineWidth / 2;
            int y = lineWidth / 2;
            float realSlope;
            float slope = 0;

            //Avoid dividing with zero (when line is vertical)
            boolean vertical = lineCoordinates[0] - oldLineCoordinates[0] != 0;
            if (!vertical) {
                //Calculate slope of line
                realSlope = (lineCoordinates[1] - oldLineCoordinates[1]) / (lineCoordinates[0] - oldLineCoordinates[0]);
                slope = Math.abs(realSlope);

                //Y should go upwards if line is leaning to the left
                if (realSlope < 0) y*=-1;
            }

            //Create new part of line
            linePath = new Path();
            linePath.moveTo(oldLineCoordinates[0], oldLineCoordinates[1]);
            linePath.lineTo(lineCoordinates[0], lineCoordinates[1]);
            //Go right, to fill right side of rectangle
            linePath.lineTo(lineCoordinates[0] + x + lineWidth / 20, lineCoordinates[1] + y);

            //Fill other side of rectangle by going either x or y, based on slope
            if (!vertical && slope < 1)
                linePath.lineTo(lineCoordinates[0], lineCoordinates[1] - y*2);
            else
                linePath.lineTo(lineCoordinates[0] - x*2, lineCoordinates[1]);

            oldEraserCoordinates = new float[] { eraserStartPos.x, eraserStartPos.y };
            boolean erased = false;
            //Loop through according to amount of eraser parts
            for (float j = 1; j <= eraseParts; j++) {
                //Calculate end of the next eraser part
                eraserCoordinates = new float[2];
                pathMeasure.getPosTan(j * (eraserLength / eraseParts), eraserCoordinates, null);

                int eraserWidth = this.strokeWidth;
                x = eraserWidth / 2;
                y = eraserWidth / 2;

                //Avoid dividing with zero (when line is vertical)
                vertical = eraserCoordinates[0] - oldEraserCoordinates[0] != 0;
                if (!vertical) {
                    //Calculate slope of line
                    realSlope = (eraserCoordinates[1] - oldEraserCoordinates[1]) / (eraserCoordinates[0] - oldEraserCoordinates[0]);
                    slope = Math.abs(realSlope);

                    //Y should go upwards if line is leaning to the left
                    if (realSlope < 0) y*=-1;
                }

                //Create new part of eraser
                eraserPath = new Path();
                eraserPath.moveTo(oldEraserCoordinates[0], oldEraserCoordinates[1]);
                eraserPath.lineTo(eraserCoordinates[0], eraserCoordinates[1]);
                //Go right, to fill right side of rectangle
                eraserPath.lineTo(eraserCoordinates[0] + x,eraserCoordinates[1] + y);

                //Fill other side of eraser by going either x or y, based on slope
                if (!vertical && slope < 1)  //-1 means line is vertical
                    eraserPath.lineTo(eraserCoordinates[0], eraserCoordinates[1] - y);
                else
                    eraserPath.lineTo(eraserCoordinates[0] - x, eraserCoordinates[1]);

                //Check if part of line intersects with part of eraser
                boolean intersects = intersects(eraserPath, linePath, canvas);
                if (intersects) {
                    erased = true;
                    break;
                }

                oldEraserCoordinates = eraserCoordinates;
            }

            if (!erased) {
                return false;
            }

            oldLineCoordinates = lineCoordinates;
        }

        return true;
    }

    private boolean intersects(Path eraserPath, Path linePath, Canvas canvas) {
        Region clip = new Region(new Rect(0, 0, 1080, 1812));

        Region regionEraser = new Region();
        regionEraser.setPath(eraserPath, clip);

        Region regionLine = new Region();
        regionLine.setPath(linePath, clip);

        //Draw parts of line (for debugging)
        if (canvas != null) {
            Paint newPaint = new Paint();
            newPaint.setStrokeWidth(40);
            newPaint.setColor(Color.CYAN);
            canvas.drawRect(regionEraser.getBounds(), newPaint);

            newPaint.setColor(Color.RED);
            canvas.drawRect(regionLine.getBounds(), newPaint);
        }

        //Return if bounds created from both paths intersects
        return regionEraser.getBounds().intersect(regionLine.getBounds());
    }
}
