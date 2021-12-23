package com.sharkcheat.pubgmaimbottouchtest;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.graphics.Canvas;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.view.GestureDetector;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.CompoundButton;
import android.widget.RelativeLayout;
import android.widget.Switch;
import android.widget.Toast;

import com.topjohnwu.superuser.Shell;

import java.io.File;

public class MyService extends Service {

    static {
        System.loadLibrary("RainCityGamingTECH");
    }
    public static native void getInitCanvasDrawing(CanvasDrawingView canvasDrawingView, Canvas canvas);
    private native int getInitBase(int Width, int Height);
    private native void closeSocket();


    private CanvasDrawingView canvasDrawingView;
    private static WindowManager.LayoutParams canvasDrawingParams;
    private static MyService instance = null;
    private static Context context = null;
    private WindowManager windowManager;
    private WindowManager.LayoutParams params;
    private View mainView;
    private GestureDetector gestureDetector;
    private RelativeLayout layout_menu, layout_controll;

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        if (getContext() != null && getInstance() == null){
            setInstance(this);
            gestureDetector = new GestureDetector(context, new SingleTapConfirm());
            windowManager = (WindowManager) getInstance().getSystemService(WINDOW_SERVICE);
            params = Devices.getParams();
            mainView = LayoutInflater.from(getContext()).inflate(R.layout.service_layout, null);
            windowManager.addView(mainView, params);
            layout_menu = mainView.findViewById(R.id.layout_menu);
            layout_controll = mainView.findViewById(R.id.layout_controll);
            layout_menu.setVisibility(View.VISIBLE);
            layout_controll.setVisibility(View.GONE);
            mainView.findViewById(R.id.layout_hide).setOnClickListener(view -> {
                mainView.findViewById(R.id.layout_menu).setVisibility(View.GONE);
                mainView.findViewById(R.id.layout_controll).setVisibility(View.VISIBLE);
            });
            mainView.findViewById(R.id.layout_close).setOnClickListener(view -> {
                Stop();
            });
            mainView.findViewById(R.id.image_view_controll).setOnTouchListener(onTouchListenerLayout);
            Switch swicth_aimbot_touch = mainView.findViewById(R.id.swicth_aimbot_touch);
            swicth_aimbot_touch.setOnCheckedChangeListener((compoundButton, isChecked) -> {
                if (isChecked){
                    new Thread(() -> {
                        if (getInitBase(Devices.getWidth(getInstance()), Devices.getHeight(getInstance())) != 1) {
                            System.exit(1);
                        } else {
                            new Handler(Looper.getMainLooper()).post(() -> CanvasDrawingOverlay());
                        }
                    }).start();
                    new Thread(() -> {
                        try {
                            Thread.sleep(400);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                        File daemonFile = new File(getFilesDir().getPath(), "libPUBGMDaemon.so");
                        if (daemonFile.exists())
                            Shell.sh(daemonFile.toString()).exec();
                    }).start();
                } else {
                    closeSocket();
                }
            });
        }
    }

    private void CanvasDrawingOverlay() {
        try {
            if (canvasDrawingView == null){
                windowManager = FWindowManager.getWindowManager(context);
                canvasDrawingView = new CanvasDrawingView(context);
                canvasDrawingView.setFPS(60);
                canvasDrawingView.runDrawing();
                canvasDrawingParams = CanvasDrawingParams.getParams();
                windowManager.addView(canvasDrawingView, canvasDrawingParams);
            }
        } catch (Exception e){
            e.printStackTrace();
        }
    }
    private void setInstance(MyService myService) {
        instance = myService;
    }

    public static void Start(Context ctx){
        if (getContext() == null){
            setContext(ctx);
            getContext().startService(new Intent(getContext(), MyService.class));
        }
    }

    public static void Stop(){
        if (getContext() != null){
            getInstance().stopSelf();
            getInstance().onDestroy();
            getContext().startService(new Intent(getContext(), MyService.class));
            setContext(null);
            instance = null;
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        if (mainView != null && windowManager != null)
            windowManager.removeView(mainView);
        if (canvasDrawingView != null && windowManager != null)
            windowManager.removeView(canvasDrawingView);
        mainView = null;
        windowManager = null;
        closeSocket();
    }
    private static MyService getInstance() {
        return instance;
    }

    private static Context getContext() {
        return context;
    }

    private static void setContext(Context context) {
        MyService.context = context;
    }

    private View.OnTouchListener onTouchListenerLayout = new View.OnTouchListener() {
        private int initialX;
        private int initialY;
        private float initialTouchX;
        private float initialTouchY;
        @Override
        public boolean onTouch(View v, MotionEvent event) {
            if (gestureDetector.onTouchEvent(event)) {
                layout_menu.setVisibility(View.VISIBLE);
                layout_controll.setVisibility(View.GONE);
                return true;
            } else {
                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        initialX = params.x;
                        initialY = params.y;
                        initialTouchX = event.getRawX();
                        initialTouchY = event.getRawY();
                        return true;
                    case MotionEvent.ACTION_MOVE:
                        params.x = initialX + (int) (event.getRawX() - initialTouchX);
                        params.y = initialY + (int) (event.getRawY() - initialTouchY);
                        windowManager.updateViewLayout(mainView, params);
                        return true;
                }
                return false;
            }
        }
    };
    public class SingleTapConfirm extends GestureDetector.SimpleOnGestureListener {
        @Override
        public boolean onSingleTapUp(MotionEvent event) {
            return true;
        }
    }
}