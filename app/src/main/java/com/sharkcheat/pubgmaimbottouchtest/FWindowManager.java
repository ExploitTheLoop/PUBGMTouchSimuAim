package com.sharkcheat.pubgmaimbottouchtest;

import android.content.Context;
import android.view.WindowManager;

public class FWindowManager {
    public static WindowManager getWindowManager(Context context){
        return (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
    }
}
