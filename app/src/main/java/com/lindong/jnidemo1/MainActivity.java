package com.lindong.jnidemo1;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;

public class MainActivity extends AppCompatActivity {

    private JniInterface mJniInterface;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        JniInterface.getInstance().jniOnLoadTest();

        Log.d("====", "jniOnload1 return = " +
                JniInterface.getInstance().jniOnload1(new Person()));
    }


}