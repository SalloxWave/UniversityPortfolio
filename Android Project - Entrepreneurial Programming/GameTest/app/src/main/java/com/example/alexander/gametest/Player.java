package com.example.alexander.gametest;

import android.graphics.Bitmap;
import android.graphics.Canvas;

/**
 * Created by Alexander on 2017-10-16.
 */

public class Player extends GameObject
{
    private Bitmap spritesheet;
    private int score;
    private boolean up;
    private boolean playing;
    private Animation animation = new Animation();
    private long startTime;  //Calculate and incrementing the score

    public Player(Bitmap image, int w, int h, int numFrames)
    {
        x = 100;
        y = GamePanel.HEIGHT / 2;
        dy = 0;
        score = 0;
        spritesheet = image;
        height = h;
        width = w;

        //List of animation images
        Bitmap[] images = new Bitmap[numFrames];

        //Fill animation images
        for(int i = 0; i < images.length; i++)
        {
            images[i] = Bitmap.createBitmap(spritesheet, i*width, 0, width, height);
        }

        //Animate the images
        animation.setFrames(images);
        animation.setDelay(10);

        startTime = System.nanoTime();
    }

    public void setUp(boolean up)
    {
        this.up = up;
    }

    public void update()
    {
        long elapsed = (System.nanoTime() - startTime)/1000000;
        //Update score after every 100 milliseconds
        if (elapsed > 100)
        {
            score++;
            startTime = System.nanoTime();
        }
        animation.update();

        //Update acceleration based on direction
        //Pressing down (move helicopter up)
        if (up)
        {
            dy-=1;
        }
        else
        {
            dy+=1;
        }

        //Set limit to acceleration
        if (dy>14) dy = 14;
        if (dy<-14) dy = -14;

        //Update height based on speed
        y+= dy*2;
    }

    public void draw(Canvas canvas)
    {
        canvas.drawBitmap(animation.getImage(), x, y, null);
    }

    public int getScore() { return score; }
    public boolean isPlaying() { return playing; }
    public void setPlaying(boolean playing) { this.playing = playing; }
    public void resetDY() { dy = 0; }
    public void resetScore() { score = 0; }
}
