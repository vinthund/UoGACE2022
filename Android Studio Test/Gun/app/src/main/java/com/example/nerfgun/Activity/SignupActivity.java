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
import com.example.nerfgun.Util.Constant;
import com.example.nerfgun.Util.Utility;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;

public class SignupActivity extends AppCompatActivity {
    private FirebaseAuth mAuth;
    //    EditText firstnameTextEdit;
//    EditText lastnameTextEdit;
//    EditText usernameTextEdit;
    EditText mailTextEdit;
    EditText passwordTextEdit;
    RelativeLayout loading;
    Button signupBtn;
    boolean isFillUsername;
    boolean isFillPassword;
    //    EditText confirmPasswordTextEdit;
    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_signup);

//        firstnameTextEdit = (EditText)findViewById(R.id.firstnameTextEdit);
//        lastnameTextEdit = (EditText)findViewById(R.id.lastnameTextEdit);
//        usernameTextEdit = (EditText)findViewById(R.id.usernameTextEdit);
        mailTextEdit = (EditText)findViewById(R.id.emailEditText);
        passwordTextEdit = (EditText)findViewById(R.id.passwordEditText);
        signupBtn = (Button)findViewById(R.id.signupBtn);
        loading = findViewById(R.id.loading);
//        confirmPasswordTextEdit = (EditText)findViewById(R.id.confirmPasswordTextEdit);
        EditText[] editTexts = { /*firstnameTextEdit, lastnameTextEdit, usernameTextEdit,*/ mailTextEdit, passwordTextEdit/*, confirmPasswordTextEdit*/};


        mailTextEdit.addTextChangedListener(new TextWatcher() {

            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            public void onTextChanged(CharSequence s, int start, int before,
                                      int count) {
                isFillUsername = false;
                signupBtn.setEnabled(false);
                if (s.length() > 4)
                {
                    isFillUsername = true;
                    if(isFillUsername && isFillPassword) {
                        signupBtn.setEnabled(true);
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
                signupBtn.setEnabled(false);
                if (s.length() > 4)
                {
                    isFillPassword = true;
                    if(isFillUsername && isFillPassword) {
                        signupBtn.setEnabled(true);
                    }
                }

            }

            @Override
            public void afterTextChanged(Editable editable) {

            }
        });

        mAuth = FirebaseAuth.getInstance();
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

    public void signup(View view) {

//        String firstname = firstnameTextEdit.getText().toString();
//        String lastname = lastnameTextEdit.getText().toString();
//        String username = usernameTextEdit.getText().toString();
        String mail = mailTextEdit.getText().toString();
        String password = passwordTextEdit.getText().toString();

        if(!Patterns.EMAIL_ADDRESS.matcher(mail).matches()){
            Utility.displayToast(SignupActivity.this, Constant.invalidMailMessage);
            return;
        }

        loading.setVisibility(View.VISIBLE);
        createUser(mail, password);
    }

    private void createUser(String mail, String password){
        if(!TextUtils.isEmpty(mail) && !TextUtils.isEmpty(password)){
            mAuth.createUserWithEmailAndPassword(mail, password).addOnCompleteListener(new OnCompleteListener<AuthResult>() {
                @Override
                public void onComplete(@NonNull Task<AuthResult> task) {
                    if(task.isSuccessful()){
                        Utility.displayToast(SignupActivity.this, Constant.registerSuccessfulMessage);
                        Intent intent = new Intent(SignupActivity.this, SigninActivity.class);
                        startActivity(intent);
                    }
                    else{
                        Utility.displayToast(SignupActivity.this,"Registration Error : " + task.getException().getMessage());
                    }
                    loading.setVisibility(View.INVISIBLE);
                }
            });
        }
    }

    public void login(View view) {
        Intent intent = new Intent(this, SigninActivity.class);
        startActivity(intent);
    }
    public void hideKeyboard(View view) {
        InputMethodManager inputMethodManager =(InputMethodManager)getSystemService(Activity.INPUT_METHOD_SERVICE);
        inputMethodManager.hideSoftInputFromWindow(view.getWindowToken(), 0);
    }
}