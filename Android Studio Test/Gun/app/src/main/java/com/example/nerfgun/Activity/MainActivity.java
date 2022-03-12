package com.example.nerfgun.Activity;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import com.example.nerfgun.R;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;

public class MainActivity extends AppCompatActivity {

    private FirebaseAuth mAuth;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mAuth = FirebaseAuth.getInstance();
    }

    @Override
    public void onStart() {
        super.onStart();
        // Check if user is signed in (non-null) and update UI accordingly.
        FirebaseUser currentUser = mAuth.getCurrentUser();
        if(currentUser == null){
            Intent intent = new Intent(this, SigninActivity.class);
            startActivity(intent);
        }
    }

    public void signoutClick(View view) {
        mAuth.signOut();
        Intent intent = new Intent(this, SigninActivity.class);
        startActivity(intent);
    }

    public void liveClick(View view) {
        Intent intent = new Intent(this, LiveActivity.class);
        startActivity(intent);
    }
}