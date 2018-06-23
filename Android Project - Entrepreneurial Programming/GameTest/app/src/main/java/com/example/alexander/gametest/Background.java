package com.example.alexander.gametest;

import android.graphics.Bitmap;
import android.graphics.Canvas;

/**
 * Created by Alexander on 2017-10-16.
 */

public class Background
{
    private Bitmap image;
    private int x, y, dx;

    public Background(Bitmap image)
    {
        this.image = image;
        dx = GamePanel.MOVESPEED;
    }

    public void update()
    {
        x+=dx;
        //Background is out of screen, reset background position to zero
        if (x < -GamePanel.WIDTH) { x = 0; }
    }

    public void draw(Canvas canvas)
    {
        //Draw image to canvas
        canvas.drawBitmap(image, x, y, null);

        //If out of screen, create new background filling out rest of screen
        if (x < 0)
        {
            canvas.drawBitmap(image, x + GamePanel.WIDTH, y, null);
        }
    }
}

