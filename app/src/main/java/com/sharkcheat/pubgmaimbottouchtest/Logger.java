package com.sharkcheat.pubgmaimbottouchtest;

import android.content.Context;
import android.util.Log;

public class Logger {

    private static String TAG = "RCG";
    private static String[] cmdType = {"VERBOSE","DEBUG","INFO","ERROR"};
    private static String typeLog = null;
    public static String VERBOSE = cmdType[0];
    public static String DEBUG = cmdType[1];
    public static String INFO = cmdType[2];
    public static String ERROR = cmdType[3];
    private Context context;
    private String logText;

    public Logger (String logText){
        this.logText = logText;
    }

    public static Logger makeLog(String cmd, String logText){
        if (cmd != null || cmd.equalsIgnoreCase("") || cmd.isEmpty())
            setTypeLog(cmd);
        return new Logger(logText);
    }

    public void show(){
        if (getTypeLog() != null || getTypeLog().equalsIgnoreCase("") || getTypeLog().isEmpty()){
            if (getTypeLog().equalsIgnoreCase(VERBOSE))
                showLogVerbose(getLogText());
            else if (getTypeLog().equalsIgnoreCase(DEBUG))
                showLogDebug(getLogText());
            else if (getTypeLog().equalsIgnoreCase(INFO))
                showLogInfo(getLogText());
            else if (getTypeLog().equalsIgnoreCase(ERROR))
                showLogError(getLogText());
        }
    }

    private static void setTypeLog(String typeLog) {
        Logger.typeLog = typeLog;
    }

    private static String getTypeLog() {
        return typeLog;
    }

    private String getLogText() {
        return logText;
    }

    private static void showLogVerbose(String message){
        Log.v(TAG, message);
    }

    private static void showLogDebug(String message){
        Log.d(TAG, message);
    }

    private static void showLogInfo(String message){
        Log.i(TAG, message);
    }

    private static void showLogError(String message){
        Log.e(TAG, message);
    }
}
