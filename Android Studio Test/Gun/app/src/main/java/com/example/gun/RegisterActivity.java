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
import android.widget.Toast;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;

public class RegisterActivity extends AppCompatActivity {

    private FirebaseAuth mAuth;
//    EditText firstnameTextEdit;
//    EditText lastnameTextEdit;
//    EditText usernameTextEdit;
    EditText mailTextEdit;
    EditText passwordTextEdit;
    Button signupBtn;
    boolean isFillUsername;
    boolean isFillPassword;
//    EditText confirmPasswordTextEdit;
    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_register);

//        firstnameTextEdit = (EditText)findViewById(R.id.firstnameTextEdit);
//        lastnameTextEdit = (EditText)findViewById(R.id.lastnameTextEdit);
//        usernameTextEdit = (EditText)findViewById(R.id.usernameTextEdit);
        mailTextEdit = (EditText)findViewById(R.id.emailEditText);
        passwordTextEdit = (EditText)findViewById(R.id.passwordEditText);
        signupBtn = (Button)findViewById(R.id.signUpBtn);
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
            displayToast(Constant.invalidMailMessage);
            return;
        }

        createUser(mail, password);
    }

    private void createUser(String mail, String password){
        if(!TextUtils.isEmpty(mail) && !TextUtils.isEmpty(password)){
            mAuth.createUserWithEmailAndPassword(mail, password).addOnCompleteListener(new OnCompleteListener<AuthResult>() {
                @Override
                public void onComplete(@NonNull Task<AuthResult> task) {
                    if(task.isSuccessful()){
                        displayToast(Constant.registerSuccessfulMessage);
                        Intent intent = new Intent(RegisterActivity.this, LoginActivity.class);
                        startActivity(intent);
                    }
                    else{
                        displayToast("Registration Error : " + task.getException().getMessage());
                    }
                }
            });
        }
    }

    public void login(View view) {
        Intent intent = new Intent(this, LoginActivity.class);
        startActivity(intent);
    }
    public void hideKeyboard(View view) {
        InputMethodManager inputMethodManager =(InputMethodManager)getSystemService(Activity.INPUT_METHOD_SERVICE);
        inputMethodManager.hideSoftInputFromWindow(view.getWindowToken(), 0);
    }

    private void displayToast(String message){
        Toast.makeText(RegisterActivity.this, message, Toast.LENGTH_SHORT).show();
    }
}