package com.example.alexander.gametest;

import android.graphics.Bitmap;
import android.graphics.Canvas;

/**
 * Created by Alexander on 2017-10-25.
 */

public class BotBorder extends GameObject
{
    private Bitmap image;

    public BotBorder(Bitmap image, int x, int y)
    {
        //Extend off the screen so height doesn't really matter, 200 is safe
        height = 200;
        width = 20;

        this.x = x;
        this.y = y;
        dx = GamePanel.MOVESPEED;

        this.image = Bitmap.createBitmap(image, 0, 0, width, height);
    }

    public void update()
    {
        x += dx;
    }

    public void draw(Canvas canvas)
    {
        canvas.drawBitmap(image, x, y, null);
    }

}
