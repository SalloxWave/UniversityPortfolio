package com.example.alexander.speederasermain;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.appinvite.AppInviteInvitation;
import com.google.firebase.analytics.FirebaseAnalytics;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;

import static android.app.Activity.RESULT_OK;

/**
 * Created by AlexanderJ on 2017-12-05.
 */

public class FragmentSettings extends Fragment implements View.OnClickListener {
    private final int REQUEST_INVITE = 38;

    private FirebaseAnalytics firebaseAnalytics;
    private FirebaseUser user;

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        user = FirebaseAuth.getInstance().getCurrentUser();
    }

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
        firebaseAnalytics = FirebaseAnalytics.getInstance(getActivity());
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_settings, container, false);

        //Only enable link button if user is anonymous
        if (user.isAnonymous())
            view.findViewById(R.id.btn_link).setOnClickListener(this);
        else
            view.findViewById(R.id.btn_link).setVisibility(View.GONE);

        view.findViewById(R.id.btn_logout).setOnClickListener(this);
        view.findViewById(R.id.btn_invite).setOnClickListener(this);
        ((TextView)view.findViewById(R.id.tv_signin_method)).setText(getSignInMethodString());

        return view;
    }

    private String getSignInMethodString() {
        if (user.isAnonymous()) {
            return getString(R.string.tv_signin_method_anonymous);
        }
        String signInMethod = user.getProviders().get(0);

        return getString(R.string.tv_signin_method_header) + ": " + signInMethod;
    }

    @Override
    public void onClick(View view) {
        switch(view.getId()) {
            case R.id.btn_logout:
                logout();
                break;
            case R.id.btn_link:
                goToSignInActivity(true);
                break;
            case R.id.btn_invite:
                sendInvite();
                break;
        }
    }

    private void logout() {
        FirebaseAuth.getInstance().signOut();
        goToSignInActivity(false);
    }

    private void goToSignInActivity(boolean linkLogin) {
        Intent intent = new Intent(getActivity(), SignInActivity.class);
        intent.putExtra("linkLogin", linkLogin);
        startActivity(intent);
    }

    private void sendInvite() {
        Intent intent = new AppInviteInvitation.IntentBuilder(getString(R.string.invitation_title))
                .setMessage(getString(R.string.invitation_message))
                .setEmailHtmlContent(getString(R.string.invitation_html_content))
                .setEmailSubject(getString(R.string.invitation_email_subject))
                .build();
        startActivityForResult(intent, REQUEST_INVITE);
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (requestCode == REQUEST_INVITE) {
            if (resultCode == RESULT_OK) {
                // Get the invitation IDs of all sent messages
                String[] ids = AppInviteInvitation.getInvitationIds(resultCode, data);

                String toastMessage = "Successfully sent invite to " + ids.length + " people";
                Toast.makeText(getActivity(), toastMessage, Toast.LENGTH_LONG).show();
                for (String id : ids) {
                    Log.d(Constants.TAG, "Sent invitation to " + id);
                }

                customEventFriendInvite(ids.length);
            }
            else {
                Log.w(Constants.TAG, "Sending invite failed or was cancelled: " + resultCode);
                Toast.makeText(getActivity(), "Sending invite was cancelled or failed", Toast.LENGTH_LONG).show();
            }
        }
    }

    private void customEventFriendInvite(int invAmount) {
        Bundle bundle = new Bundle();
        bundle.putInt(getString(R.string.event_friend_invite_parameter), invAmount);
        firebaseAnalytics.logEvent(getString(R.string.event_friend_invite), bundle);
    }
}
