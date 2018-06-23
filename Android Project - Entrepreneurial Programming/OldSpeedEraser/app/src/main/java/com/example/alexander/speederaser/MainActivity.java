package com.example.alexander.speederaser;

import android.Manifest;
import android.app.Activity;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.Path;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.util.Log;
import android.view.Window;
import android.view.WindowManager;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Random;

public class MainActivity extends Activity
{
    public static final String LOG_TAG = "Speed Eraser";

    private final int PERMISSION_WRITE_EXTERNAL_STORAGE = 0;
    private final String[] permissions = new String[] {Manifest.permission.WRITE_EXTERNAL_STORAGE};

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        int myVersion = Build.VERSION.SDK_INT;
        /*In Android 6.0, API level 23, the users grant permissions while the app is running
        instead of when installed*/
        if (myVersion > Build.VERSION_CODES.LOLLIPOP_MR1)
        {
            if (!alreadyHasPermission())
            {
                requestPermission();
            }
            else
            {
                startGame();
            }
        }
    }

    private boolean alreadyHasPermission()
    {
        return ContextCompat.checkSelfPermission(this, permissions[0]) == PackageManager.PERMISSION_GRANTED;
    }

    private void requestPermission()
    {
        ActivityCompat.requestPermissions(this, permissions, 101);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode,
                                           String permissions[], int[] grantResults) {
        switch (requestCode)
        {
            case 101:
                // If request is cancelled, the result arrays are empty.
                if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED)
                {
                    startGame();
                }
                else
                {
                    finish();
                    System.exit(0);
                }
                break;
            default:
                super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        }
    }

    public void saveToFile(Bitmap bitmap)
    {
        String root = Environment.getExternalStorageDirectory().toString() + "/Pictures/";
        String filename = "test.jpg";
        String filepath = root + filename;
        FileOutputStream os = null;
        MainActivity.Log(filepath);

        try {
            os = new FileOutputStream(new File(root, filename));
            bitmap.compress(Bitmap.CompressFormat.JPEG, 100, os);
        }
        catch(IOException e) {e.printStackTrace();}
    }

    private void startGame()
    {
        //Turn off title
        requestWindowFeature(Window.FEATURE_NO_TITLE);

        //Set to fullscreen
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

        //Set activity to render custom view
        setContentView(new GamePanel(this));
    }

    public static void Log(String message)
    {
        Log.d(LOG_TAG, message);
    }


}

