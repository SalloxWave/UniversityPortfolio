package com.example.alexander.speederasermain;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Typeface;
import android.support.annotation.NonNull;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;
import com.google.firebase.remoteconfig.FirebaseRemoteConfig;
import com.google.firebase.remoteconfig.FirebaseRemoteConfigSettings;

import java.util.HashMap;
import java.util.Random;
import java.util.concurrent.ArrayBlockingQueue;

/**
 * Created by Alexander on 2017-10-26.
 */

public class GamePanel extends SurfaceView implements SurfaceHolder.Callback {
    public Context context;

    //Firebase variables
    private FirebaseRemoteConfig remoteConfig;
    private FirebaseDatabase firebaseDatabase;

    //Design variables
    private final int DEFAULT_BG_COLOR = Color.WHITE;
    private int width;
    private int height;
    private int bgColor = Color.WHITE;
    private int lineColor = Color.BLACK;

    //Game logic variables
    private Line line;
    private Eraser eraser;
    private boolean gameOver;
    private boolean hasErased;
    private boolean erasing;
    private boolean touchedUp;
    private NanoClock nanoClock;
    private long eraseTimeRecord;

    private static final int INPUT_QUEUE_SIZE = 1000;
    private ArrayBlockingQueue<InputObject> inputObjectPool;
    private GameThread gameThread;

    public GamePanel(Context context) {
        super(context);
        this.context = context;

        firebaseDatabase = FirebaseDatabase.getInstance();

        //Get initial screen dimensions (full screen)
        width = context.getResources().getDisplayMetrics().widthPixels;
        height = context.getResources().getDisplayMetrics().heightPixels;

        //Make game panel able to handle events
        getHolder().addCallback(this);
        setFocusable(true);

        setBackgroundColor(DEFAULT_BG_COLOR);
        initRemoteConfig();
        loadRecord();

        createInputObjectPool();
    }

    private void initRemoteConfig() {
        remoteConfig = FirebaseRemoteConfig.getInstance();
        remoteConfig.setConfigSettings(new FirebaseRemoteConfigSettings.Builder()
                .setDeveloperModeEnabled(true)
                .build());

        //Set default values for remote config parameters
        HashMap<String, Object> defaults = new HashMap<>();
        defaults.put("game_bg_color", "#ffffff");
        defaults.put("game_line_color", "#000000");
        remoteConfig.setDefaults(defaults);

        //Fetch values from the cloud
        final Task<Void> fetch = remoteConfig.fetch(0);  //0: cache time before requesting new values

        fetch.addOnCompleteListener(new OnCompleteListener<Void>() {
            @Override
            public void onComplete(@NonNull Task<Void> task) {
                if (task.isSuccessful()) {
                    remoteConfig.activateFetched();
                }
                else {
                    Log.w(Constants.TAG, "Failed to fetch remote config");
                }

                //Set background color
                String color = remoteConfig.getString("game_bg_color");
                bgColor = Color.parseColor(color);
                setBackgroundColor(bgColor);

                //Set eraser as same color as background
                eraser.setColor(bgColor);

                //Set line color
                lineColor = Color.parseColor(remoteConfig.getString("game_line_color"));
                line.setColor(lineColor);
            }
        });
    }

    private void loadRecord() {
        //Get reference to logged in user's record
        String uId = FirebaseAuth.getInstance().getCurrentUser().getUid();
        final DatabaseReference recordRef = firebaseDatabase.getReference("users/" + uId + "/record");

        ValueEventListener recordListener = new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                //User has a record
                if (dataSnapshot.exists()) {
                    eraseTimeRecord = dataSnapshot.getValue(Long.class);
                } else {
                    eraseTimeRecord = 0;
                }
            }
            @Override
            public void onCancelled(DatabaseError databaseError) {
                Log.w(Constants.TAG, "load user record: onCancelled", databaseError.toException());
            }
        };
        //Start retrieving record from firebase database
        recordRef.addListenerForSingleValueEvent(recordListener);
    }

    private void createInputObjectPool() {
        inputObjectPool = new ArrayBlockingQueue<>(INPUT_QUEUE_SIZE);
        for (int i = 0; i < INPUT_QUEUE_SIZE; i++) {
            inputObjectPool.add(new InputObject(inputObjectPool));
        }
    }

    private Line generateRandomLine(int lineCount, int color, int strokeWidth, int speed, int linePadding) {
        Random rnd = new Random();
        Path path = new Path();

        //Randomize starting point of line
        int x = (int) (rnd.nextDouble() * width);
        int y = (int) (rnd.nextDouble() * height);
        path.moveTo(x, y);

        for (int i = 0; i < lineCount; i++) {
            //Randomize position of next path
            x = (int) (rnd.nextDouble() * width);
            y = (int) (rnd.nextDouble() * height);

            //Adjust according to padding
            if (x < linePadding) x = linePadding;
            else if (x > width - linePadding) x = width - linePadding;
            if (y < linePadding) y = linePadding;
            else if (y > height - linePadding) y = height - linePadding;

            path.lineTo(x, y);
        }
        return new Line(path, color, strokeWidth, speed);
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        //Try to close down game thread until success
        boolean retry = true;
        int counter = 0;
        while (retry && counter < 1000) {
            counter++;
            try {
                gameThread.setRunning(false);
                gameThread.join();
                retry = false;
                gameThread = null;
            }
            catch(InterruptedException e) { e.printStackTrace(); }
        }
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        gameOver = false;
        hasErased = false;
        erasing = false;
        touchedUp = false;
        nanoClock = new NanoClock();

        //Initialize random line
        line = generateRandomLine(4, lineColor, 60, 0, 100);

        //Initialize the eraser
        Bitmap eraserImg = BitmapFactory.decodeResource(getResources(), R.drawable.rubbergum);
        eraser = new Eraser(eraserImg, DEFAULT_BG_COLOR);

        //Start the gamethread in which the game loop runs inside
        gameThread = new GameThread(getHolder(), this);
        gameThread.setRunning(true);
        gameThread.start();
    }

    public void update() {
        //Update clock if a user is currently erasing
        if (erasing) {
            nanoClock.update();
        }
        line.update();
    }

    @Override
    public void draw(Canvas canvas) {
        if (canvas == null) {
            return;
        }
        super.draw(canvas);

        //Save old canvas
        final int savedState = canvas.save();

        line.draw(canvas);
        eraser.draw(canvas);
        drawTime(canvas);
        drawRecord(canvas);
        drawFPS(canvas);

        //Show game instruction when user is not erasing
        if (!erasing) {
            drawHelp(canvas);
        }

        //Draw result when game is over
        if (gameOver) {
            drawResult(canvas);
        }

        canvas.restoreToCount(savedState);
        this.invalidate();
    }

    private void drawRecord(Canvas canvas) {
        Paint paint = new Paint();
        paint.setColor(Color.WHITE);
        paint.setTextSize(75);
        paint.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.BOLD));

        String recordText = "Record: " + NanoClock.toString(eraseTimeRecord, false);
        canvas.drawText(recordText, width - (paint.getTextSize() * 7), paint.getTextSize(), paint);
    }

    private void drawTime(Canvas canvas) {
        Paint paint = new Paint();
        paint.setColor(Color.WHITE);
        paint.setTextSize(75);
        paint.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.BOLD));

        String timeText = "Time: " + nanoClock.toString(true);
        canvas.drawText(timeText, width - (paint.getTextSize() * 6), paint.getTextSize() * 2, paint);
    }

    private void drawHelp(Canvas canvas) {
        Paint paint = new Paint();
        paint.setColor(Color.WHITE);
        paint.setTextSize(50);
        paint.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.BOLD));
        canvas.drawText("Touch to start erasing, release when done", 0, height / 2, paint);
    }

    private void drawResult(Canvas canvas) {
        Paint paint = new Paint();
        paint.setTextSize(75);
        paint.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.BOLD));

        float yPos = paint.getTextSize() * 3f;
        if (hasErased) {
            paint.setColor(Color.GREEN);
            canvas.drawText("LINE ERASED!", 0, yPos, paint);
        }
        else {
            paint.setColor(Color.RED);
            canvas.drawText("LINE IS NOT FULLY ERASED!", 0, yPos, paint);
        }
    }

    private void drawFPS(Canvas canvas) {
        Paint paint = new Paint();
        paint.setColor(Color.BLACK);
        paint.setTextSize(75);
        paint.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.BOLD));

        //Avoid crashing when trying to read FPS from a thread shutting down
        if (gameThread != null) {
            canvas.drawText("FPS: " + gameThread.getAverageFPS(), width - (paint.getTextSize() * 4), height - paint.getTextSize(), paint);
        }
    }

    @Override
    public boolean onTouchEvent(final MotionEvent event) {
        try {
            //Process history events if there is any
            int hist = event.getHistorySize();
            if (hist > 0) {
                InputObject input;
                //Add inputs to gameThread input queue
                for (int i = 0; i < hist; i++) {
                    input = inputObjectPool.take();
                    input.useEventHistory(event, i);  //Configure input object based on motion event object
                    gameThread.enqueInput(input);
                }
            }

            //Process current event
            InputObject input = inputObjectPool.take();
            input.useEvent(event);
            gameThread.enqueInput(input);
        } catch (InterruptedException e) { e.printStackTrace(); }

        return true;
    }

    public void handleMotionEvent(InputObject input) {
        switch (input.action) {
            case InputObject.ACTION_TOUCH_DOWN:
                startErase();
                eraser.setPosition(input.x, input.y);
                break;

            case InputObject.ACTION_TOUCH_MOVE:
                if (!erasing) {  //In case touch down doesn't work
                    startErase();
                }
                eraser.move(input.x, input.y);
                break;

            case InputObject.ACTION_TOUCH_UP:
                //Avoid multiple up events in a row, which saves performance
                if (touchedUp) {
                    return;
                }
                touchedUp = true;
                gameOver = true;
                erasing = false;
                hasErased = eraser.hasErased(line, null);

                //Erase time record 0 means no record has yet been set
                if (hasErased) {
                    if (eraseTimeRecord == 0 || nanoClock.getNano() < eraseTimeRecord) {
                        uploadNewRecord();

                        //Reload the new record
                        loadRecord();
                    }
                }

                //Generate new random line for player to erase
                line = generateRandomLine(4, lineColor, 60, 0, 100);

                //remove old eraser to not interfere with new line
                eraser.reset();
                break;
        }
    }

    private void startErase() {
        resetGame();
        nanoClock.restart();
        erasing = true;
    }

    private void resetGame() {
        gameOver = false;
        hasErased = false;
        erasing = false;
        touchedUp = false;
        eraser.reset();
    }

    private void uploadNewRecord() {
        //To avoid hackers or bugs (not that there would be any bugs)
        if (nanoClock.getNano() == 0) {
            return;
        }

        //Get reference to currently signed in user and set new record
        String uId = FirebaseAuth.getInstance().getCurrentUser().getUid();
        DatabaseReference userRef = firebaseDatabase.getReference("users/" + uId + "/record");
        userRef.setValue(nanoClock.getNano());
    }
}
