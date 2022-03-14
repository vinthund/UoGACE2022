package com.example.nerfgun.Util;

import android.content.SharedPreferences;

import androidx.appcompat.app.AppCompatActivity;

public class Config {

    public Config(AppCompatActivity context){
        prefs = context.getSharedPreferences("com.example.medical", context.MODE_PRIVATE);
    }

    private SharedPreferences prefs;
    public void SetValue(String key, String val)
    {
        prefs.edit().putString(key, val).apply();
    }
    public String GetValue(String key){
        String data = prefs.getString(key, "");
        return data;
    }
    public void Clear(){
        SetValue("mail", "");
        SetValue("pass", "");
    }
}
