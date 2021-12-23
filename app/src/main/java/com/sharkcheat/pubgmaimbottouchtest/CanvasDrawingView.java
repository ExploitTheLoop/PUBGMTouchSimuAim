package com.sharkcheat.pubgmaimbottouchtest;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.graphics.Rect;
import android.graphics.RectF;
import android.graphics.drawable.GradientDrawable;
import android.os.Handler;
import android.os.Looper;
import android.os.SystemClock;
import android.text.TextUtils;
import android.view.View;


import java.text.SimpleDateFormat;
import java.util.Date;

public class CanvasDrawingView extends View {
    private int FPS = 60;
    private Paint mTextPaint, mStrokePaint, mTextPaint2, mFilledPaint, mCirclePaint, mRectPaint;
    public Paint fillPaint = new Paint();
    public Paint strokePaint = new Paint();
    private Rect eRect;
    private GradientDrawable eGradientDrawable;
    private Date time;
    private SimpleDateFormat formatter;
    public CanvasDrawingView(Context context) {
        super(context, null, 0);
        InitializePaints();
        setFocusableInTouchMode(false);
        setBackgroundColor(Color.TRANSPARENT);
    }

    private int eColor[] = {Color.TRANSPARENT,Color.GREEN,Color.TRANSPARENT};

    private void InitializePaints() {
        fillPaint.setStyle(Paint.Style.FILL);
        fillPaint.setColor(Color.YELLOW);
        strokePaint.setStyle(Paint.Style.STROKE);
        strokePaint.setColor(Color.BLACK);
        strokePaint.setStrokeWidth(2);

        eRect = new Rect(-100,60,100,100);
        eGradientDrawable = new GradientDrawable();
        eGradientDrawable.setOrientation(GradientDrawable.Orientation.LEFT_RIGHT);
        eGradientDrawable.setColors(eColor);
        eGradientDrawable.setShape(GradientDrawable.RECTANGLE);
        eGradientDrawable.setBounds(eRect);


        mStrokePaint = new Paint();
        mStrokePaint.setStyle(Paint.Style.STROKE);
        mStrokePaint.setAntiAlias(true);
        mStrokePaint.setColor(Color.rgb(0, 0, 0));

        mTextPaint = new Paint();
        mTextPaint.setStyle(Paint.Style.FILL_AND_STROKE);
        mTextPaint.setAntiAlias(true);
        mTextPaint.setColor(Color.RED);
        mTextPaint.setTextAlign(Paint.Align.CENTER);
        mTextPaint.setStrokeWidth(1.1f);

        mTextPaint2 = new Paint();
        mTextPaint2.setStyle(Paint.Style.FILL_AND_STROKE);
        mTextPaint2.setAntiAlias(true);
        mTextPaint2.setColor(Color.RED);
        mTextPaint2.setTextAlign(Paint.Align.LEFT);
        mTextPaint2.setStrokeWidth(1.1f);

        mFilledPaint = new Paint();
        mFilledPaint.setStyle(Paint.Style.FILL);
        mFilledPaint.setAntiAlias(true);

        mCirclePaint = new Paint();
        mCirclePaint.setStyle(Paint.Style.STROKE);
        mCirclePaint.setAntiAlias(true);

        mRectPaint = new Paint();
        mRectPaint.setStyle(Paint.Style.STROKE);
        mRectPaint.setAntiAlias(true);
    }
    Handler handler = new Handler(Looper.getMainLooper());
    Runnable runnable = new Runnable() {
        @Override
        public void run() {
            try {
                if (true)
                {
                    invalidate();
                }
            } catch (Exception e){
            }
            handler.postDelayed(this, FPS);
        }
    };

    public void runDrawing() {
        runnable.run();
    }

    public void setFPS(int fps){
        FPS = 60 - fps;
    }
    private int    mFPS = 0;         // the value to show
    private int    mFPSCounter = 0;  // the value to count
    private long   mFPSTime = 0;     // last update time

    @Override
    protected void onDraw(Canvas canvas) {
        if (canvas != null) {
            ClearCanvas(canvas);
            MyService.getInitCanvasDrawing(this, canvas);
        }
    }
    private int convertDipToPixels(int i) {
        return (int) ((((float) i) * getContext().getResources().getDisplayMetrics().density) + 0.5f);
    }

    public void ClearCanvas(Canvas cvs) {
        cvs.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR);
        ShowFPSOnScreen(cvs);
    }

    @SuppressLint("SimpleDateFormat")
    public void ShowFPSOnScreen(Canvas cvs) {
        formatter = new SimpleDateFormat("HH:mm:ss");
        mTextPaint2.setColor(Color.YELLOW);
        mTextPaint2.setTextSize(convertDipToPixels(14));
        String dateString = formatter.format(new Date());
        if (SystemClock.uptimeMillis() - mFPSTime > 1000) {
            mFPSTime = SystemClock.uptimeMillis();
            mFPS = mFPSCounter;
            mFPSCounter = 0;
        } else {
            mFPSCounter++;
        }
        String s = "FPS " + mFPS + "    " + dateString;
        cvs.drawText(s, 100,50, mTextPaint2);
    }


    public void DrawEnemyCount(Canvas cvs, int colors, float dx, float dy) {
        if (colors == 1){ // Enemy = 0
            eColor[1] = Color.GREEN;
        }
        if (colors == 2){ // Enemy > 5
            eColor[1] = Color.YELLOW;
        }
        if (colors == 3){ // Enemy > 10
            eColor[1] = Color.parseColor("#FF6D00");
        }
        if (colors == 4){ // Enemy > 15
            eColor[1] = Color.RED;
        }
        cvs.save();
        cvs.translate(dx,dy);
        eGradientDrawable.setGradientType(GradientDrawable.LINEAR_GRADIENT);
        eGradientDrawable.draw(cvs);
        cvs.restore();
    }

    public void DrawText(Canvas cvs, int a, int r, int g, int b, String txt, float posX, float posY, float size) {
        mTextPaint.setShadowLayer(3, 0,0, Color.BLACK);
        mTextPaint.setColor(Color.rgb(r, g, b));
        mTextPaint.setAlpha(a);
        if (getRight() > 1920 || getBottom() > 1920)
            mTextPaint.setTextSize(4 + size);
        else if (getRight() == 1920 || getBottom() == 1920)
            mTextPaint.setTextSize(2 + size);
        else
            mTextPaint.setTextSize(size);
        cvs.drawText(txt, posX, posY, mTextPaint);
    }

    public void DrawText2(Canvas cvs, int a, int r, int g, int b, String txt, float posX, float posY, float size) {
        mTextPaint.clearShadowLayer();
        mTextPaint.setColor(Color.rgb(r, g, b));
        mTextPaint.setAlpha(a);
        if (getRight() > 1920 || getBottom() > 1920)
            mTextPaint.setTextSize(4 + size);
        else if (getRight() == 1920 || getBottom() == 1920)
            mTextPaint.setTextSize(2 + size);
        else
            mTextPaint.setTextSize(size);
        cvs.drawText(txt, posX, posY, mTextPaint);
    }

    public void DrawLine(Canvas cvs, int a, int r, int g, int b, float lineWidth, float fromX, float fromY, float toX, float toY) {
        mStrokePaint.setColor(Color.rgb(r, g, b));
        mStrokePaint.setAlpha(a);
        mStrokePaint.setStrokeWidth(lineWidth);
        cvs.drawLine(fromX, fromY, toX, toY, mStrokePaint);
    }

    public void DrawCircle(Canvas cvs, int a, int r, int g, int b, float cx, float cy, float radius){
        mFilledPaint.setColor(Color.rgb(r, g, b));
        mFilledPaint.setAlpha(a);
        cvs.drawCircle(cx, cy, radius, mFilledPaint);
    }

    public void DrawCircleStroke(Canvas cvs, int a, int r, int g, int b, float cx, float cy, float radius){
        mCirclePaint.setStrokeWidth(4);
        mCirclePaint.setColor(Color.rgb(r, g, b));
        mCirclePaint.setAlpha(a);
        cvs.drawCircle(cx, cy, radius, mCirclePaint);
    }

    public void DrawCircle2(Canvas cvs, int a, int r, int g, int b, float cx, float cy, float radius){
        mCirclePaint.setStrokeWidth(2);
        mCirclePaint.setColor(Color.rgb(r, g, b));
        mCirclePaint.setAlpha(a);
        cvs.drawCircle(cx, cy, radius, mCirclePaint);
    }

    public void DrawRect(Canvas canvas, int a, int r, int g, int b, float strokeSize, float startX, float startY, float endX, float endY){
        mRectPaint.setColor(Color.rgb(r, g, b));
        mTextPaint.setAlpha(a);
        mRectPaint.setStrokeWidth(strokeSize);
        mRectPaint.setStyle(Paint.Style.STROKE);
        canvas.drawRect(startX, startY, endX, endY, mRectPaint);
    }



    public void DrawFilled(Canvas canvas, int a, int r, int g, int b, float stroke, float left, float top, float right, float bottom){
        fillPaint.setARGB(a,r, g, b);
        RectF rect = new RectF(left, top + stroke, right, bottom);
        canvas.drawRect(rect, fillPaint);
        canvas.drawRect(rect, strokePaint);
    }

    public void DrawHealth(Canvas canvas, int a, int r, int g, int b, float rectBarHealthleft, float rectBarHealthtop, float rectBarHealthright, float rectBarHealthbottom, float rectBarStrokeleft, float rectBarStroketop, float rectBarStrokeright, float rectBarStrokebottom){
        fillPaint.setARGB(a,r, g, b);
        RectF rectBarHealth = new RectF(rectBarHealthleft, rectBarHealthtop + 30, rectBarHealthright, rectBarHealthbottom);
        RectF rectBarStroke = new RectF(rectBarStrokeleft, rectBarStroketop + 30, rectBarStrokeright, rectBarStrokebottom);
        canvas.drawRect(rectBarHealth, fillPaint);
        canvas.drawRect(rectBarStroke, strokePaint);
    }

    private boolean isEmpty(String s){
        return TextUtils.isEmpty(s);
    }
}
