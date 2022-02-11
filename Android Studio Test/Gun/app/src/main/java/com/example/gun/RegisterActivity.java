package com.example.gun;

import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;

public class RegisterActivity extends AppCompatActivity {

    EditText firstnameTextEdit;
    EditText lastnameTextEdit;
    EditText usernameTextEdit;
    EditText mailTextEdit;
    EditText passwordTextEdit;
    EditText confirmPasswordTextEdit;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_register);

        firstnameTextEdit = (EditText)findViewById(R.id.firstnameTextEdit);
        lastnameTextEdit = (EditText)findViewById(R.id.lastnameTextEdit);
        usernameTextEdit = (EditText)findViewById(R.id.usernameTextEdit);
        mailTextEdit = (EditText)findViewById(R.id.mailTextEdit);
        passwordTextEdit = (EditText)findViewById(R.id.passwordTextEdit);
        confirmPasswordTextEdit = (EditText)findViewById(R.id.confirmPasswordTextEdit);
        EditText[] editTexts = { firstnameTextEdit, lastnameTextEdit, usernameTextEdit, mailTextEdit, passwordTextEdit, confirmPasswordTextEdit};


        for(EditText textEdit : editTexts)
        textEdit.setOnFocusChangeListener(new View.OnFocusChangeListener() {
            @Override
            public void onFocusChange(View v, boolean hasFocus) {
                if (!hasFocus) {
                    hideKeyboard(v);
                }
            }
        });
    }

    public void register(View view) {
        Intent intent = new Intent(this, LoginActivity.class);
        startActivity(intent);
    }

    public void login(View view) {
        Intent intent = new Intent(this, LoginActivity.class);
        startActivity(intent);
    }
    public void hideKeyboard(View view) {
        InputMethodManager inputMethodManager =(InputMethodManager)getSystemService(Activity.INPUT_METHOD_SERVICE);
        inputMethodManager.hideSoftInputFromWindow(view.getWindowToken(), 0);
    }
}