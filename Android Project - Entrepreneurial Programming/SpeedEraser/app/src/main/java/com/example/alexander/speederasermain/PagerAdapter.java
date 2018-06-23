package com.example.alexander.speederasermain;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentStatePagerAdapter;

/**
 * Created by AlexanderJ on 2017-12-05.
 */

public class PagerAdapter extends FragmentStatePagerAdapter {
    int numOfTabs;

    public PagerAdapter(FragmentManager fm, int NumOfTabs) {
        super(fm);
        numOfTabs = NumOfTabs;
    }

    @Override
    public Fragment getItem(int position) {
        //Return fragment based on position of selected tab
        switch (position) {
            case 0:
                FragmentHome fragHome = new FragmentHome();
                return fragHome;
            case 1:
                FragmentSettings fragSettings = new FragmentSettings();
                return fragSettings;
            default:
                return null;
        }
    }

    @Override
    public int getCount() {
        return numOfTabs;
    }
}
