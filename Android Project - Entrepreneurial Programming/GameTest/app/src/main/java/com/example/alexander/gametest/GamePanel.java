package com.example.alexander.gametest;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.Typeface;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Random;

/**
 * Created by Alexander on 2017-10-16.
 */

public class GamePanel extends SurfaceView implements SurfaceHolder.Callback
{
    public static final int WIDTH = 856;
    public static final int HEIGHT = 480;
    public static final int MOVESPEED = -5;

    private long smokeStartTime;
    private long missileStartTime;
    private MainThread thread;
    private Background background;
    private Player player;
    private ArrayList<Smokepuff> smoke;
    private ArrayList<Missile> missiles;
    private ArrayList<TopBorder> topBorder;
    private ArrayList<BotBorder> botBorder;
    private Random random = new Random();
    private int maxBorderHeight;
    private int minBorderHeight;
    private boolean topDown = true;
    private boolean botDown = true;
    private boolean newGameCreated;

    //Increase to slow down difficulty progress, decrease to speed up difficulty progression
    private int progressDenom = 20;

    private Explosion explosion;
    private long startReset;
    private boolean reset;
    private boolean disappear;
    private boolean started;
    private int best;

    public GamePanel(Context context)  //Context: Ultimate base class
    {
        super(context);

        //Add the callback to the surfaceholder to intercept events
        getHolder().addCallback(this);

        //Make gamePanel focusable so it can handle events
        setFocusable(true);
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height)
    {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder)
    {
        //Try to close down main loop until success
        boolean retry = true;
        int counter = 0;
        while(retry && counter < 1000)
        {
            counter++;
            try
            {
                thread.setRunning(false);
                thread.join();
                retry = false;
                thread = null;
            }
            catch(InterruptedException e) { e.printStackTrace(); }
        }
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder)
    {
        //Fetch background image and set vector speed
        background = new Background(BitmapFactory.decodeResource(getResources(), R.drawable.grassbg1));
        player = new Player(BitmapFactory.decodeResource(getResources(), R.drawable.helicopter), 65, 25, 3);
        smoke = new ArrayList<Smokepuff>();
        missiles = new ArrayList<Missile>();
        topBorder = new ArrayList<TopBorder>();
        botBorder = new ArrayList<BotBorder>();
        smokeStartTime = System.nanoTime();
        missileStartTime = System.nanoTime();

        //Create the main thread with surface holder and surface view (this)
        thread = new MainThread(getHolder(), this);

        //We can safely start the game loop
        thread.setRunning(true);
        thread.start();
    }

    @Override
    public boolean onTouchEvent(MotionEvent event)
    {
        if (event.getAction() == MotionEvent.ACTION_DOWN)
        {
            //Player starts playing first when screen is pressed
            if (!player.isPlaying() && newGameCreated && reset)
            {
                player.setPlaying(true);
                player.setUp(true);
            }
            if (player.isPlaying())
            {
                if (!started) started = true;
                reset = false;
                player.setUp(true);
            }
            return true;
        }
        if (event.getAction() == MotionEvent.ACTION_UP)
        {
            player.setUp(false);
            return true;
        }
        return super.onTouchEvent(event);
    }

    public void update()
    {
        if (player.isPlaying())
        {
            if (topBorder.isEmpty() || botBorder.isEmpty())
            {
                player.setPlaying(false);
                return;
            }

            background.update();
            player.update();

            //Calculate the threshold of height the border can have based on the score
            //max and min border heart are updated, and the border switched direction when either max or
            //min is met

            maxBorderHeight = 30 + player.getScore() / progressDenom;
            //Cap max border height so that borders can only take up a total of 1/2 the screen
            if (maxBorderHeight > HEIGHT/4) maxBorderHeight = HEIGHT /4;
            minBorderHeight = 5 + player.getScore() / progressDenom;

            //Check top border collision
            for(int i = 0; i < topBorder.size(); i++)
            {
                if(collision(topBorder.get(i), player))
                {
                    player.setPlaying(false);
                }
            }

            //Check bottom border collision
            for(int i = 0; i < botBorder.size(); i++)
            {
                if(collision(botBorder.get(i), player))
                {
                    player.setPlaying(false);
                }
            }

            //Update top border
            this.updateTopBorder();

            //Update bottom border
            this.updateBotBorder();

            //Add missiles on timer
            long missileElapsed = (System.nanoTime() - missileStartTime)/1000000;
            if (missileElapsed > (2000 - player.getScore()/4))  //Higher scores gives less delay time
            {
                int yPos;
                //First missile always goes down the middle
                if (missiles.size()==0)
                    yPos = HEIGHT/2;
                else
                    yPos = (int) ((random.nextDouble() * (HEIGHT - maxBorderHeight * 2)) + maxBorderHeight);

                missiles.add(new Missile(BitmapFactory.decodeResource(getResources(),
                        R.drawable.missile), WIDTH + 10, yPos, 45, 15, player.getScore(), 13));

                missileStartTime = System.nanoTime();
            }

            //Update current missiles
            for(int i = 0; i < missiles.size(); i++)
            {
                missiles.get(i).update();
                if (collision(missiles.get(i), player))
                {
                    missiles.remove(i);
                    player.setPlaying(false);
                    break;
                }
                //Missile out of screen
                if (missiles.get(i).getX() < -100)
                {
                    missiles.remove(i);
                    break;
                }
            }

            //Add smokepuffs on timer
            long elapsed = (System.nanoTime() - smokeStartTime)/1000000;
            if (elapsed > 120)
            {
                //Add smoke on helicopter's lower part
                smoke.add(new Smokepuff(player.getX(), player.getY() + 10));
                smokeStartTime = System.nanoTime();
            }

            //Update current smokepuffs
            for(int i = 0; i < smoke.size(); i++)
            {
                smoke.get(i).update();
                //Smokepuff is out of the screen
                if(smoke.get(i).getX() < -10)  //r = 5
                {
                    smoke.remove(i);
                }
            }
        }
        else
        {
            player.resetDY();
            if (!reset)
            {
                newGameCreated = false;
                startReset = System.nanoTime();
                reset = true;
                disappear = true;
                explosion = new Explosion(BitmapFactory.decodeResource(getResources(), R.drawable.explosion
                ), player.getX(), player.getY() - 30, 100, 100, 25);
            }

            explosion.update();
            long resetElapsed = (System.nanoTime() - startReset)/1000000;

            if (resetElapsed > 2500 && !newGameCreated)
            {
                newGame();
            }
        }
    }

    public boolean collision(GameObject a, GameObject b)
    {
        return Rect.intersects(a.getRectangle(), b.getRectangle());
    }

    @Override
    public void draw(Canvas canvas)
    {
        super.draw(canvas);

        //Scale bitmaps to screen size
        final float scaleFactorX = (float) getWidth()/WIDTH;
        final float scaleFactorY = (float) getHeight()/HEIGHT;
        if (canvas != null)
        {
            //Save old canvas and scale canvas
            final int savedState = canvas.save();
            canvas.scale(scaleFactorX, scaleFactorY);

            //Draw bitmaps
            background.draw(canvas);
            if (!disappear) player.draw(canvas);
            for(Smokepuff sp: smoke)
            {
                sp.draw(canvas);
            }
            for(Missile m: missiles)
            {
                m.draw(canvas);
            }

            //Draw top border
            for(TopBorder tb: topBorder)
            {
                tb.draw(canvas);
            }

            //Draw bottom border
            for(BotBorder bb: botBorder)
            {
                bb.draw(canvas);
            }

            //Draw explosion
            if (started)
            {
                explosion.draw(canvas);
            }

            drawText(canvas);

            //Set back to old canvas
            canvas.restoreToCount(savedState);
        }
    }

    public void updateTopBorder()
    {
        //Every 50 points, insert randomly placed top blocks that break the pattern
        if (player.getScore() % 50 == 0)
        {
            topBorder.add(new TopBorder(BitmapFactory.decodeResource(getResources(), R.drawable.brick
            ), topBorder.get(topBorder.size() - 1).getX() + 20, 0, (int) ((random.nextDouble() * (maxBorderHeight
            )) + 1)));
        }

        for(int i = 0; i < topBorder.size(); i++)
        {
            topBorder.get(i).update();
            //Border out of bound
            if (topBorder.get(i).getX() < -20)
            {
                //Remove element of arraylist, replace it by adding a new one
                topBorder.remove(i);

                //Calculate topdown which determines the direction the border is moving
                //Last border has reached the max height, therefore go up instead
                if (topBorder.get(topBorder.size() - 1).getHeight() >= maxBorderHeight)
                {
                    topDown = false;
                }
                if (topBorder.get(topBorder.size() - 1).getHeight() <= minBorderHeight)
                {
                    topDown = true;
                }

                //New border added will have larger height
                if(topDown)
                {
                    topBorder.add(new TopBorder(BitmapFactory.decodeResource(getResources(), R.drawable.brick
                    ), topBorder.get(topBorder.size() - 1).getX() + 20, 0,
                        topBorder.get(topBorder.size() - 1).getHeight() + 1));
                }
                //New border added will have smaller height
                else
                {
                    topBorder.add(new TopBorder(BitmapFactory.decodeResource(getResources(), R.drawable.brick
                    ), topBorder.get(topBorder.size() - 1).getX() + 20, 0,
                            topBorder.get(topBorder.size() - 1).getHeight() -1));
                }
            }
        }
    }

    public void updateBotBorder()
    {
        //Every 40 points, insert randomly placed bottom blocks that break pattern
        if (player.getScore() % 40 == 0)
        {
            botBorder.add(new BotBorder(BitmapFactory.decodeResource(getResources(), R.drawable.brick
            ), botBorder.get(botBorder.size() - 1).getX() + 20, (int) ((random.nextDouble()
            * maxBorderHeight) + (HEIGHT-maxBorderHeight))));
        }

        //Update bottom border
        for(int i = 0; i < botBorder.size(); i++)
        {
            botBorder.get(i).update();

            //If border is moving off screen, remove it and add a new corresponding new one
            if (botBorder.get(i).getX() < - 20)
            {
                //Remove element of arraylist, replace it by adding a new one
                botBorder.remove(i);

                //Calculate topdown which determines the direction the border is moving
                //Last border has reached the max height, therefore go up instead
                if (botBorder.get(botBorder.size() - 1).getY() <= HEIGHT - maxBorderHeight)
                {
                    botDown = true;
                }
                if (botBorder.get(botBorder.size() - 1).getY() >= HEIGHT - minBorderHeight)
                {
                    botDown = false;
                }

                //New border added will have larger height
                if(botDown)
                {
                    botBorder.add(new BotBorder(BitmapFactory.decodeResource(getResources(), R.drawable.brick
                    ), botBorder.get(botBorder.size() - 1).getX() + 20, botBorder.get(botBorder.size() - 1).getY() + 1));
                }
                //New border added will have smaller height
                else
                {
                    botBorder.add(new BotBorder(BitmapFactory.decodeResource(getResources(), R.drawable.brick
                    ), botBorder.get(botBorder.size() - 1).getX() + 20, botBorder.get(botBorder.size() - 1).getY() - 1));
                }
            }
        }
    }

    public void newGame()
    {
        disappear = false;

        topBorder.clear();
        botBorder.clear();
        missiles.clear();
        smoke.clear();

        minBorderHeight = 5;
        maxBorderHeight = 30;

        player.resetDY();
        player.setY(HEIGHT / 2);

        if (player.getScore() > best)
        {
            best = player.getScore();
        }
        player.resetScore();

        //Create initial borders
        //Initial top border
        for(int i = 0; i*20 < WIDTH + 40; i++)
        {
            //First top border ever created
            if (i == 0)
            {
                topBorder.add(new TopBorder(BitmapFactory.decodeResource(getResources(), R.drawable.brick
                ), i*20, 0, 10));
            }
            else
            {
                topBorder.add(new TopBorder(BitmapFactory.decodeResource(getResources(), R.drawable.brick
                ), i*20, 0, topBorder.get(i-1).getHeight() + 1));
            }
        }

        //Initial bottom border
        for(int i = 0; i*20 < WIDTH + 40; i++)
        {
            //First bottom border ever created
            if (i == 0)
            {
                botBorder.add(new BotBorder(BitmapFactory.decodeResource(getResources(), R.drawable.brick
                ), i*20, HEIGHT - minBorderHeight));
            }
            //Adding borders until the initial screen if filed
            else
            {
                botBorder.add(new BotBorder(BitmapFactory.decodeResource(getResources(), R.drawable.brick
                ), i*20, botBorder.get(i-1).getY() - 1));
            }
        }

        newGameCreated = true;
    }

    public void drawText(Canvas canvas)
    {
        Paint paint = new Paint();
        paint.setColor(Color.BLACK);
        paint.setTextSize(30);
        paint.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.BOLD));
        canvas.drawText("DISTANCE " + (player.getScore() * 3), 10, HEIGHT - 10, paint);
        canvas.drawText("BEST: " + best, WIDTH - 215, HEIGHT - 10, paint);

        if (!player.isPlaying() && newGameCreated && reset)
        {
            Paint paint1 = new Paint();
            paint1.setTextSize(40);
            paint1.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.BOLD));
            canvas.drawText("PRESS TO START", WIDTH/2 - 50, HEIGHT/2, paint1);

            paint1.setTextSize(20);
            canvas.drawText("PRESS AND HOLD TO GO UP", WIDTH/2 - 50, HEIGHT/2 + 20, paint1);
            canvas.drawText("RELEASE TO GO DOWN", WIDTH/2 - 50, HEIGHT/2 + 40, paint1);
        }
    }
}
