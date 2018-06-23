package com.example.alexander.gametest;

import android.graphics.Bitmap;
import android.graphics.Canvas;

import java.util.Random;

/**
 * Created by Alexander on 2017-10-17.
 */

public class Missile extends GameObject
{
    private int score;
    private int speed;
    private Random random = new Random();
    private Animation animation = new Animation();
    private Bitmap spritesheet;

    public Missile(Bitmap image, int x, int y, int w, int h, int s, int numFrames)
    {
        spritesheet = image;
        super.x = x;
        super.y = y;

        width = w;
        height = h;
        score = s;

        //Speed uses base speed together with random speed based on current score
        speed = 7 + (int) (random.nextDouble()*score/30);  //nextDouble generates 0.0-1.0

        //Limit missile speed
        if (speed >= 40) speed = 40;

        //Setup animation images
        Bitmap[] images = new Bitmap[numFrames];
        for(int i = 0; i < images.length; i++)
        {
            images[i] = Bitmap.createBitmap(spritesheet, 0, i*height, width, height);
        }

        animation.setFrames(images);
        animation.setDelay(100-speed);  //Faster animation for faster missiles
    }

    public void update()
    {
        x-=speed;
        animation.update();
    }

    public void draw(Canvas canvas)
    {
        try
        {
            canvas.drawBitmap(animation.getImage(), x, y, null);
        }
        catch(Exception e){ e.printStackTrace(); }
    }

    @Override
    public int getWidth()
    {
        //Offset slightly for more realistic collision detection
        return width-10;
    }

}
