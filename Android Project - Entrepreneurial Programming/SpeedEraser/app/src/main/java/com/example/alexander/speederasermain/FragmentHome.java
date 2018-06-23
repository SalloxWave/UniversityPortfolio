package com.example.alexander.speederasermain;

import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.analytics.FirebaseAnalytics;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;
import com.google.firebase.perf.FirebasePerformance;
import com.google.firebase.perf.metrics.Trace;
import com.google.firebase.remoteconfig.FirebaseRemoteConfig;
import com.google.firebase.remoteconfig.FirebaseRemoteConfigSettings;

import java.util.HashMap;

/**
 * Created by AlexanderJ on 2017-12-05.
 */

public class FragmentHome extends Fragment implements View.OnClickListener {
    private FirebaseUser user;
    private FirebaseRemoteConfig remoteConfig;
    private FirebaseAnalytics firebaseAnalytics;
    private FirebaseDatabase firebaseDatabase;
    private Trace nicknameTrace;
    private Trace recordTrace;
    private String nickname;
    private long eraseTimeRecord;

    private View view;

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
        user = FirebaseAuth.getInstance().getCurrentUser();
        firebaseAnalytics = FirebaseAnalytics.getInstance(getActivity());
        firebaseDatabase = FirebaseDatabase.getInstance();
    }

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        view = inflater.inflate(R.layout.fragment_home, container, false);
        view.findViewById(R.id.btn_play).setOnClickListener(this);
        return view;
    }

    @Override
    public void onResume() {
        super.onResume();
        nicknameTrace = FirebasePerformance.getInstance().newTrace("loading_nickname");
        recordTrace = FirebasePerformance.getInstance().newTrace("loading_record");
        loadNickname();
        loadRecord();
        loadRemoteConfig();
    }

    @Override
    public void onClick(View view) {
        switch(view.getId()) {
            case R.id.btn_play:
                //Log that play was pressed
                logCustomEvent(getString(R.string.event_pressed_play), new Bundle());

                //Switch to game activity to start game
                Intent intent = new Intent(getActivity(), GameActivity.class);
                startActivity(intent);
                break;
        }
    }

    private void loadNickname() {
        nicknameTrace.start();

        DatabaseReference nicknameRef = firebaseDatabase.getReference("users/" + user.getUid() + "/nickname");
        ValueEventListener nicknameListener = new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                nicknameTrace.stop();

                nickname = dataSnapshot.getValue(String.class);
                ((TextView)view.findViewById(R.id.tv_nickname)).setText(nickname);
            }
            @Override
            public void onCancelled(DatabaseError databaseError) {
                Log.w(Constants.TAG, "load nickname: onCancelled", databaseError.toException());
            }
        };
        nicknameRef.addListenerForSingleValueEvent(nicknameListener);
    }

    private void loadRecord() {
        recordTrace.start();

        DatabaseReference recordRef = firebaseDatabase.getReference("users/" + user.getUid() + "/record");
        ValueEventListener recordListener = new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                recordTrace.stop();

                //User has a record
                if (dataSnapshot.exists()) {
                    eraseTimeRecord = dataSnapshot.getValue(Long.class);
                } else {
                    eraseTimeRecord = 0;
                }

                String recordText = getString(R.string.home_record_header) + ": " + NanoClock.toString(eraseTimeRecord, false);
                ((TextView)view.findViewById(R.id.tv_record)).setText(recordText);
            }
            @Override
            public void onCancelled(DatabaseError databaseError) {
                Log.w(Constants.TAG, "load user record: onCancelled", databaseError.toException());
            }
        };
        recordRef.addListenerForSingleValueEvent(recordListener);
    }

    private void loadRemoteConfig() {
        remoteConfig = FirebaseRemoteConfig.getInstance();
        remoteConfig.setConfigSettings(new FirebaseRemoteConfigSettings.Builder()
                .setDeveloperModeEnabled(true)
                .build());

        //Set default values for remove config parameterss
        HashMap<String, Object> defaults = new HashMap<>();
        defaults.put("nickname_color", "#ffffff");
        remoteConfig.setDefaults(defaults);

        //Fetch values from the cloud
        final Task<Void> fetch = remoteConfig.fetch(0);  //0: cache time before requesting new values
        fetch.addOnCompleteListener(new OnCompleteListener<Void>() {
            @Override
            public void onComplete(@NonNull Task<Void> task) {
                if (task.isSuccessful()) {
                    Log.d(Constants.TAG, "Successfully fetched remote config in fragment home");
                    remoteConfig.activateFetched();
                }
                else {
                    Log.w(Constants.TAG, "Failed to fetch remote config");
                }

                setNicknameColor();
            }
        });
    }

    private void setNicknameColor() {
        String color = remoteConfig.getString("nickname_color");
        view.findViewById(R.id.tv_nickname).setBackgroundColor(Color.parseColor(color));
    }

    private void logCustomEvent(String logName, Bundle bundle) {
        firebaseAnalytics.logEvent(logName, bundle);
    }
}
