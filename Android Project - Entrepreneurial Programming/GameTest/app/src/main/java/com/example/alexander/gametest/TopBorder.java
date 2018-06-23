package com.example.alexander.gametest;

import android.graphics.Bitmap;
import android.graphics.Canvas;

/**
 * Created by Alexander on 2017-10-25.
 */

public class TopBorder extends GameObject
{
    private Bitmap image;

    public TopBorder(Bitmap image, int x, int y, int h)
    {
        height = h;
        width = 20;

        this.x = x;
        this.y = y;

        dx = GamePanel.MOVESPEED;
        this.image = Bitmap.createBitmap(image, 0, 0, width, height);
    }

    public void update()
    {
        x+=dx;
    }

    public void draw(Canvas canvas)
    {
        try {canvas.drawBitmap(image, x, y, null);}
        catch(Exception e) {}
    }

}
