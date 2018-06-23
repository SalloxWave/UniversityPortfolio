package com.example.alexander.speederaser;

import android.util.EventLog;
import android.view.MotionEvent;

/**
 * Created by Alex on 2017-11-07.
 */

public class EventHelpThread implements Runnable {

    private MotionEvent event;
    public EventHelpThread(MotionEvent event)
    {
        this.event = event;
    }
    @Override
    public void run()
    {

    }
}
