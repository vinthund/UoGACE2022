package com.example.gun;

import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class LoginActivity extends AppCompatActivity {

    EditText usernameTextEdit;
    EditText passwordTextEdit;
    TextView messageLink;
    Button loginBtn;
    Button registerBtn;
    boolean isFillUsername;
    boolean isFillPassword;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);
        usernameTextEdit = (EditText)findViewById(R.id.usernameTextEdit);
        passwordTextEdit = (EditText)findViewById(R.id.passwordTextEdit);
        loginBtn = (Button)findViewById(R.id.loginBtn);
        registerBtn = (Button)findViewById(R.id.registerBtn);
        messageLink = (TextView)findViewById(R.id.messageLink);
        loginBtn.setEnabled(false);

        usernameTextEdit.setOnFocusChangeListener(new View.OnFocusChangeListener() {
            @Override
            public void onFocusChange(View v, boolean hasFocus) {
                if (!hasFocus) {
                    hideKeyboard(v);
                }
            }
        });

        passwordTextEdit.setOnFocusChangeListener(new View.OnFocusChangeListener() {
            @Override
            public void onFocusChange(View v, boolean hasFocus) {
                if (!hasFocus) {
                    hideKeyboard(v);
                }
            }
        });

        usernameTextEdit.addTextChangedListener(new TextWatcher() {

            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            public void onTextChanged(CharSequence s, int start, int before,
                                      int count) {
                isFillUsername = false;
                loginBtn.setEnabled(false);
                messageLink.setVisibility(View.VISIBLE);
                if (s.length() > 4)
                {
                    isFillUsername = true;
                    if(isFillUsername && isFillPassword){
                        messageLink.setVisibility(View.INVISIBLE);
                        loginBtn.setEnabled(true);
                    }
                }

            }

            @Override
            public void afterTextChanged(Editable editable) {

            }
        });

        passwordTextEdit.addTextChangedListener(new TextWatcher() {

            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            public void onTextChanged(CharSequence s, int start, int before,
                                      int count) {
                isFillPassword = false;
                loginBtn.setEnabled(false);
                messageLink.setVisibility(View.VISIBLE);
                if (s.length() > 4)
                {
                    isFillPassword = true;
                    if(isFillUsername && isFillPassword){
                        messageLink.setVisibility(View.INVISIBLE);
                        loginBtn.setEnabled(true);
                    }
                }

            }

            @Override
            public void afterTextChanged(Editable editable) {

            }
        });
    }

    public void login(View view) {
        if(usernameTextEdit.getText().toString().matches("admin") && passwordTextEdit.getText().toString().matches("admin"))
        {
            Intent intent = new Intent(this, LaunchActivity.class);
            startActivity(intent);
        }
    }

    public void register(View view) {
        Intent intent = new Intent(this, RegisterActivity.class);
        startActivity(intent);
    }
    public void hideKeyboard(View view) {
        InputMethodManager inputMethodManager =(InputMethodManager)getSystemService(Activity.INPUT_METHOD_SERVICE);
        inputMethodManager.hideSoftInputFromWindow(view.getWindowToken(), 0);
    }
}