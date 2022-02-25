package com.example.gun;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.util.Patterns;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;

public class LoginActivity extends AppCompatActivity {

    private FirebaseAuth mAuth;
    EditText mailTextEdit;
    EditText passwordTextEdit;
    Button loginBtn;
    Button registerBtn;
    boolean isFillUsername;
    boolean isFillPassword;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);
        mailTextEdit = (EditText)findViewById(R.id.emailEditText);
        passwordTextEdit = (EditText)findViewById(R.id.passwordEditText);
        loginBtn = (Button)findViewById(R.id.loginBtn);
        registerBtn = (Button)findViewById(R.id.registerBtn);
        loginBtn.setEnabled(false);
        mAuth = FirebaseAuth.getInstance();


        EditText[] editTexts = { mailTextEdit, passwordTextEdit};
        for(EditText textEdit : editTexts) {
            textEdit.setOnFocusChangeListener(new View.OnFocusChangeListener() {
                @Override
                public void onFocusChange(View v, boolean hasFocus) {
                    if (!hasFocus) {
                        hideKeyboard(v);
                    }
                }
            });
        }


        mailTextEdit.addTextChangedListener(new TextWatcher() {

            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            public void onTextChanged(CharSequence s, int start, int before,
                                      int count) {
                isFillUsername = false;
                loginBtn.setEnabled(false);
                if (s.length() > 4)
                {
                    isFillUsername = true;
                    if(isFillUsername && isFillPassword) {
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
                if (s.length() > 4)
                {
                    isFillPassword = true;
                    if(isFillUsername && isFillPassword) {
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

        String mail = mailTextEdit.getText().toString();
        String password = passwordTextEdit.getText().toString();

        if(!Patterns.EMAIL_ADDRESS.matcher(mail).matches()){
            displayToast(Constant.invalidMailMessage);
            return;
        }

        loginUser(mail, password);
    }

    private void loginUser(String mail, String password){


        if(!TextUtils.isEmpty(mail) && Patterns.EMAIL_ADDRESS.matcher(mail).matches() && !TextUtils.isEmpty(password)){
            mAuth.signInWithEmailAndPassword(mail, password).addOnCompleteListener(new OnCompleteListener<AuthResult>() {
                @Override
                public void onComplete(@NonNull Task<AuthResult> task) {
                    if(task.isSuccessful()){
                        displayToast(Constant.loggedInSuccessfulMessage);
                        Intent intent = new Intent(LoginActivity.this, LaunchActivity.class);
                        startActivity(intent);
                    }
                    else{
                        displayToast("Login Error : " + task.getException().getMessage());
                    }
                }
            });
        }
    }

    public void signup(View view) {
        Intent intent = new Intent(this, RegisterActivity.class);
        startActivity(intent);
    }
    public void hideKeyboard(View view) {
        InputMethodManager inputMethodManager =(InputMethodManager)getSystemService(Activity.INPUT_METHOD_SERVICE);
        inputMethodManager.hideSoftInputFromWindow(view.getWindowToken(), 0);
    }

    private void displayToast(String message){
        Toast.makeText(LoginActivity.this, message, Toast.LENGTH_SHORT).show();
    }
}