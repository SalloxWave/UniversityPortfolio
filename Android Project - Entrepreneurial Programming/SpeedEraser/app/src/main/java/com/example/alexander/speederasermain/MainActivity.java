package com.example.alexander.speederasermain;

import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.ProgressBar;

import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;

/**
 * Created by Alexander on 2017-12-20.
 */

public class MainActivity extends AppCompatActivity {
    private final String[] permissions = new String[] {android.Manifest.permission.ACCESS_FINE_LOCATION};
    private final int REQUEST_CODE = 101;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.mainactivity);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE,
                WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE);

        //Make loading progress bar visible
        findViewById(R.id.main_progress_bar).setVisibility(View.VISIBLE);

        int myVersion = Build.VERSION.SDK_INT;

        /*In Android 6.0, API level 23, the users grant permissions while the app is running
        instead of when installed*/
        if (myVersion > Build.VERSION_CODES.LOLLIPOP_MR1) {
            if (!alreadyHasPermission())
                requestPermission();
            else
                switchActivity();
        }
        else {
            switchActivity();
        }
    }

    private boolean alreadyHasPermission() {
        return ContextCompat.checkSelfPermission(this, permissions[0]) == PackageManager.PERMISSION_GRANTED;
    }

    private void requestPermission() {
        ActivityCompat.requestPermissions(this, permissions, REQUEST_CODE);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode,
                                           String permissions[], int[] grantResults) {
        switch (requestCode) {
            case REQUEST_CODE:
                //If request is cancelled, the result arrays are empty.
                if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    switchActivity();
                }
                else {
                    //Quit app
                    finishAffinity();
                }
                break;
            default:
                super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        }
    }

    private void switchActivity() {
        FirebaseUser user = FirebaseAuth.getInstance().getCurrentUser();

        //Go to sign in activity or main menu based on if user is signed in or not
        if (user != null) {
            startActivity(new Intent(MainActivity.this, MainMenu.class));
        }
        else {
            startActivity(new Intent(MainActivity.this, SignInActivity.class));
        }
    }
}
