package com.example.alexander.speederasermain;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.EditText;
import android.widget.Toast;

import com.google.firebase.analytics.FirebaseAnalytics;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

/**
 * Created by Alexander on 2017-11-22.
 */

public class NicknameActivity extends Activity {
    private FirebaseAnalytics firebaseAnalytics;
    private String nickname;
    private int nickNameTries = 0;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.nicknameactivity);

        firebaseAnalytics = FirebaseAnalytics.getInstance(this);

        findViewById(R.id.btn_continue).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                nickname = ((EditText)findViewById(R.id.et_nickname)).getText().toString();
                if (!nickname.isEmpty()) {
                    findViewById(R.id.nickname_progress).setVisibility(View.VISIBLE);

                    //Update nickname to user database
                    updateNickname();
                }
                else {
                    nickNameTries++;
                    Toast.makeText(NicknameActivity.this, "Nickname can't be empty", Toast.LENGTH_LONG).show();
                }
            }
        });
    }

    @Override
    public void onDestroy() {
        super.onDestroy();

        //Make sure user is signed out when exiting without selecting a nickname
        FirebaseAuth.getInstance().signOut();
    }

    private void updateNickname() {
        final FirebaseDatabase db = FirebaseDatabase.getInstance();

        //Create reference to all users in firebase database
        DatabaseReference usersRef = db.getReference("users");
        ValueEventListener usersListener = new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                boolean exists = false;

                //Go through all users in database
                for (DataSnapshot data: dataSnapshot.getChildren()) {
                    //Chosen nickname already exists
                    if (data.child("nickname").getValue().toString().equals(nickname)) {
                        exists = true;
                        break;
                    }
                }

                if (!exists) {
                    //Get reference to current user's nickname and update value (created if not exists)
                    String uId = FirebaseAuth.getInstance().getCurrentUser().getUid();
                    DatabaseReference nickNameRef = db.getReference("users/" + uId + "/nickname");
                    nickNameRef.setValue(nickname);

                    goToMainMenu();
                }
                else {
                    nickNameTries++;
                    findViewById(R.id.nickname_progress).setVisibility(View.GONE);

                    Toast.makeText(NicknameActivity.this, "Nickname already exists", Toast.LENGTH_LONG).show();
                }
            }

            @Override
            public void onCancelled(DatabaseError databaseError) {
                Log.w(Constants.TAG, "load users: onCancelled", databaseError.toException());
            }
        };
        usersRef.addListenerForSingleValueEvent(usersListener);
    }

    private void goToMainMenu() {
        setLoading(true);

        customEvent();

        Intent intent = new Intent(NicknameActivity.this, MainMenu.class);
        startActivity(intent);
    }

    private void customEvent() {
        //Log amount of tries before choosing a nickname
        Bundle bundle = new Bundle();
        bundle.putInt(getString(R.string.event_nickname_parameter), nickNameTries);
        firebaseAnalytics.logEvent(getString(R.string.event_nickname_choice), bundle);
    }

    private void setLoading(boolean loading) {
        if (loading) {
            getWindow().setFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE,
                    WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE);
            findViewById(R.id.nickname_progress).setVisibility(View.VISIBLE);
        }
        else {
            getWindow().clearFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE);
            findViewById(R.id.nickname_progress).setVisibility(View.GONE);
        }
    }
}
