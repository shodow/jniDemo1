package com.lindong.jnidemo1;

import android.util.Log;
import android.widget.RatingBar;

public class JniInterface {
    static {
        System.loadLibrary("interface");
    }
    private static String TAG = JniInterface.class.getSimpleName();
    private static JniInterface instance = null;

    public static JniInterface getInstance() {
        if (null == instance) {
            instance = new JniInterface();
        }
        return instance;
    }

    private JniInterface() {
        Log.d(TAG, "JniInterface");
    }

    public native void jniOnLoadTest();

    public native String jniOnload1(Person person);
}
