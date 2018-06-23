package com.example.alexander.speederasermain;

import android.content.Intent;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.ProgressBar;
import android.widget.Toast;

import com.google.android.gms.auth.api.signin.GoogleSignIn;
import com.google.android.gms.auth.api.signin.GoogleSignInAccount;
import com.google.android.gms.auth.api.signin.GoogleSignInClient;
import com.google.android.gms.auth.api.signin.GoogleSignInOptions;
import com.google.android.gms.common.api.ApiException;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.analytics.FirebaseAnalytics;
import com.google.firebase.auth.AuthCredential;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseAuthUserCollisionException;
import com.google.firebase.auth.GoogleAuthProvider;
import com.google.firebase.auth.TwitterAuthProvider;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;
import com.twitter.sdk.android.core.Callback;
import com.twitter.sdk.android.core.Result;
import com.twitter.sdk.android.core.Twitter;
import com.twitter.sdk.android.core.TwitterException;
import com.twitter.sdk.android.core.TwitterSession;
import com.twitter.sdk.android.core.identity.TwitterLoginButton;

public class SignInActivity extends AppCompatActivity implements View.OnClickListener {
    private final int REQUESTCODE_GOOGLE = 1;

    private GoogleSignInClient googleSignInClient;
    private TwitterLoginButton twitterSignInButton;

    private FirebaseAuth firebaseAuth;
    private FirebaseAnalytics firebaseAnalytics;

    private boolean linkLogin;
    private String signInChoice;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Twitter.initialize(this);
        setContentView(R.layout.activity_signin);

        firebaseAuth = FirebaseAuth.getInstance();
        firebaseAnalytics = FirebaseAnalytics.getInstance(this);

        linkLogin = getIntent().getBooleanExtra("linkLogin", false);

        //Setup graphic according if session is linked login
        if (linkLogin) {
            findViewById(R.id.anonymous_signin_button).setVisibility(View.GONE);
        }
        else {
            findViewById(R.id.tv_link_header).setVisibility(View.GONE);
        }

        //Already signed in
        if (firebaseAuth.getCurrentUser() != null) {
            //Don't proceed to next activity when it's a link session
            if (!linkLogin) {
                switchActivity();
                return;
            }
        }

        //Setup sign in methods
        setUpGoogle();
        setUpTwitter();
        setUpAnonymous();
    }

    @Override
    public void onBackPressed() {
        super.onBackPressed();

        //Quit app
        finishAffinity();
    }

    private void setUpGoogle() {
        //Configure sign-in to request the user's ID, email address, and basic
        //profile. ID and basic profile are included in DEFAULT_SIGN_IN.
        GoogleSignInOptions gso = new GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_SIGN_IN)
                .requestIdToken(getString(R.string.default_web_client_id))
                .requestEmail()
                .build();

        //Build a GoogleSignInClient with the options specified by gso.
        googleSignInClient = GoogleSignIn.getClient(this, gso);

        //Redirect google login button clicks to this activity
        findViewById(R.id.google_signin_button).setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        switch(v.getId()) {
            case R.id.google_signin_button:
                signInGoogle();
                break;
            case R.id.anonymous_signin_button:
                signInAnonymously();
                break;
        }
    }

    private void signInGoogle() {
        setLoading(true);

        //Start Google sign in activity
        Intent signInIntent = googleSignInClient.getSignInIntent();
        startActivityForResult(signInIntent, REQUESTCODE_GOOGLE);
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        //Activity result was from Google sign in
        if (requestCode == REQUESTCODE_GOOGLE) {
            setLoading(true);

            Task<GoogleSignInAccount> task = GoogleSignIn.getSignedInAccountFromIntent(data);
            try {
                //Google sign in was successful, authenticate with Firebase
                GoogleSignInAccount account = task.getResult(ApiException.class);
                firebaseAuthWithGoogle(account);
            }
            catch (ApiException e) {
                Log.w(Constants.TAG, "Google sign in failed", e);
                Toast.makeText(SignInActivity.this, "Google sign in failed!", Toast.LENGTH_LONG).show();
            }
        }

        //Pass the activity result back to the Twitter sign in button
        twitterSignInButton.onActivityResult(requestCode, resultCode, data);
    }

    private void firebaseAuthWithGoogle(GoogleSignInAccount account) {
        signInChoice = "Google";

        AuthCredential credential = GoogleAuthProvider.getCredential(account.getIdToken(), null);

        //Either link with anonymous user or create new account
        if (linkLogin)
            linkToAnonymousSignIn(credential);
        else
            firebaseAuthWithCredentials(credential);
    }

    private void setUpTwitter() {
        //Set callback for Twitter sign in button
        twitterSignInButton = findViewById(R.id.twitter_signin_button);
        twitterSignInButton.setCallback(new Callback<TwitterSession>() {
            @Override
            public void success(Result<TwitterSession> result) {
                //Twitter sign in session success, authenticate with firebase
                firebaseAuthWithTwitter(result.data);
            }

            @Override
            public void failure(TwitterException exception) {
                Log.w(Constants.TAG, "twitter login: failure", exception);
                Toast.makeText(SignInActivity.this, "Twitter sign in failed!", Toast.LENGTH_LONG).show();
            }
        });
    }

    private void firebaseAuthWithTwitter(TwitterSession session) {
        signInChoice = "Twitter";
        setLoading(true);

        AuthCredential credential = TwitterAuthProvider.getCredential(
                session.getAuthToken().token,
                session.getAuthToken().secret);

        //Either link with anonymous user or create new account
        if (linkLogin)
            linkToAnonymousSignIn(credential);
        else
            firebaseAuthWithCredentials(credential);
    }

    private void firebaseAuthWithCredentials(AuthCredential credential) {
        firebaseAuth.signInWithCredential(credential)
                .addOnCompleteListener(this, new OnCompleteListener<AuthResult>() {
                    @Override
                    public void onComplete(@NonNull Task<AuthResult> task) {
                        if (task.isSuccessful()) {
                            //Whole login is finished, proceed to next activity
                            switchActivity();
                        }
                        else {
                            Log.w(Constants.TAG, "sign in with credential: failure", task.getException());
                            Toast.makeText(SignInActivity.this, "Firebase authentication failed.",
                                    Toast.LENGTH_SHORT).show();
                        }
                    }
                });
    }

    private void setUpAnonymous() {
        findViewById(R.id.anonymous_signin_button).setOnClickListener(this);
    }

    private void signInAnonymously() {
        setLoading(true);

        firebaseAuth.signInAnonymously()
                .addOnCompleteListener(this, new OnCompleteListener<AuthResult>() {
                    @Override
                    public void onComplete(@NonNull Task<AuthResult> task) {
                        //Sign in success
                        if (task.isSuccessful()) {
                            signInChoice = "Anonymous";

                            //Whole login is finished, proceed to next activity
                            switchActivity();
                        }
                        else {
                            Log.w(Constants.TAG, "Sign in anonymously: failure", task.getException());
                            Toast.makeText(SignInActivity.this, "Sign in failed failed.",
                                    Toast.LENGTH_SHORT).show();
                        }
                    }
                });
    }

    private void linkToAnonymousSignIn(final AuthCredential credential) {
        firebaseAuth.getCurrentUser().linkWithCredential(credential)
                .addOnCompleteListener(this, new OnCompleteListener<AuthResult>() {
                    @Override
                    public void onComplete(@NonNull Task<AuthResult> task) {
                        if (task.isSuccessful()) {
                            firebaseAuthWithCredentials(credential);
                        }
                        else {
                            Log.w(Constants.TAG, "link with credential:failure", task.getException());

                            //Display message depending on type of error
                            String message = "Linking failed";
                            if (task.getException().getClass() == FirebaseAuthUserCollisionException.class)
                                message = "Account already exist, cannot link";
                            Toast.makeText(SignInActivity.this, message, Toast.LENGTH_SHORT).show();
                        }
                    }
                });
    }

    private void switchActivity() {
        setLoading(true);
        logSignInEvent();

        //Create reference to logged in user
        FirebaseDatabase db = FirebaseDatabase.getInstance();
        DatabaseReference userRef = db.getReference("users/" + firebaseAuth.getCurrentUser().getUid());

        userRef.addListenerForSingleValueEvent(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                setLoading(false);

                //Already has a nickname or link login session, means you should skip nickname step
                if (dataSnapshot.exists() || linkLogin) {
                    startActivity(new Intent(SignInActivity.this, MainMenu.class));
                }
                else {
                    startActivity(new Intent(SignInActivity.this, NicknameActivity.class));
                }
            }
            @Override
            public void onCancelled(DatabaseError databaseError) {
                Log.w(Constants.TAG, "Checking if nickname exists: onCancelled");
            }
        });
    }

    private void setLoading(boolean loading) {
        if (loading) {
            getWindow().setFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE,
                    WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE);
            findViewById(R.id.signin_progressbar).setVisibility(View.VISIBLE);
        }
        else {
            getWindow().clearFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE);
            findViewById(R.id.signin_progressbar).setVisibility(View.GONE);
        }
    }

    private void logSignInEvent() {
        Bundle bundle = new Bundle();
        bundle.putString(getString(R.string.event_signin_parameter), signInChoice);
        firebaseAnalytics.logEvent(getString(R.string.event_signin), bundle);
    }
}
