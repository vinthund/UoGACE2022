package com.example.nerfgun.Activity;

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
import android.widget.RelativeLayout;

import com.example.nerfgun.R;
import com.example.nerfgun.Util.Config;
import com.example.nerfgun.Util.Constant;
import com.example.nerfgun.Util.Utility;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;

public class SigninActivity extends AppCompatActivity {

    private FirebaseAuth mAuth;
    EditText mailTextEdit;
    EditText passwordTextEdit;
    Button signinBtn;
    Button signupBtn;
    RelativeLayout loading;
    boolean isFillUsername;
    boolean isFillPassword;
    Config config;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        boolean locOn = Utility.IsLocationOn(this);


        setContentView(R.layout.activity_signin);
        mailTextEdit = (EditText)findViewById(R.id.emailEditText);
        passwordTextEdit = (EditText)findViewById(R.id.passwordEditText);
        signinBtn = (Button)findViewById(R.id.signinBtn);
        signupBtn = (Button)findViewById(R.id.signupBtn);
        loading = findViewById(R.id.loading);
        signinBtn.setEnabled(false);
        mAuth = FirebaseAuth.getInstance();

        config = new Config(this);
        String mail = config.GetValue("mail");
        String pass = config.GetValue("pass");
        if(mail != ""){
            mailTextEdit.setText(mail);
        }
        if(pass != ""){
            passwordTextEdit.setText(pass);
        }
        if(mail != "" && pass != "" && mail.length() > 4 && pass.length() > 4){
            loading.setVisibility(View.VISIBLE);
            signinBtn.setEnabled(true);
            signin(null);
        }


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
                signinBtn.setEnabled(false);
                if (s.length() > 4)
                {
                    isFillUsername = true;
                    if(isFillUsername && isFillPassword) {
                        signinBtn.setEnabled(true);
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
                signinBtn.setEnabled(false);
                if (s.length() > 4)
                {
                    isFillPassword = true;
                    if(isFillUsername && isFillPassword) {
                        signinBtn.setEnabled(true);
                    }
                }

            }

            @Override
            public void afterTextChanged(Editable editable) {

            }
        });
    }

    public void signin(View view) {

        String mail = mailTextEdit.getText().toString();
        String password = passwordTextEdit.getText().toString();

        if(!Patterns.EMAIL_ADDRESS.matcher(mail).matches()){
            Utility.displayToast(SigninActivity.this, Constant.invalidMailMessage);
            return;
        }

        loading.setVisibility(View.VISIBLE);
        loginUser(mail, password);
    }

    private void loginUser(String mail, String password){


        if(!TextUtils.isEmpty(mail) && Patterns.EMAIL_ADDRESS.matcher(mail).matches() && !TextUtils.isEmpty(password)){
            mAuth.signInWithEmailAndPassword(mail, password).addOnCompleteListener(new OnCompleteListener<AuthResult>() {
                @Override
                public void onComplete(@NonNull Task<AuthResult> task) {
                    if(task.isSuccessful()){
                        Utility.displayToast(SigninActivity.this, Constant.loggedInSuccessfulMessage);
                        Intent intent = new Intent(SigninActivity.this, MainActivity.class);
                        startActivity(intent);
                    }
                    else{
                        Utility.displayToast(SigninActivity.this, "Login Error : " + task.getException().getMessage());
                    }
                    loading.setVisibility(View.INVISIBLE);
                }
            });
        }
    }

    public void signup(View view) {
        Intent intent = new Intent(this, SignupActivity.class);
        startActivity(intent);
    }
    public void hideKeyboard(View view) {
        InputMethodManager inputMethodManager =(InputMethodManager)getSystemService(Activity.INPUT_METHOD_SERVICE);
        inputMethodManager.hideSoftInputFromWindow(view.getWindowToken(), 0);
    }
}