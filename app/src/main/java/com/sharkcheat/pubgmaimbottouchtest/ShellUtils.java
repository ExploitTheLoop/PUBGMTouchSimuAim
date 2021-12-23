package com.sharkcheat.pubgmaimbottouchtest;

import android.app.Activity;
import android.app.ProgressDialog;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.Looper;

import com.topjohnwu.superuser.Shell;

public class ShellUtils {
    public static class ooo extends AsyncTask<Boolean, String, Boolean> {

        private Activity activity;
        ProgressDialog progressDialog;

        public ooo (Activity activity){
            this.activity = activity;
            progressDialog = new ProgressDialog(activity);
        }

        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            activity.runOnUiThread(() -> {
                progressDialog.setTitle("Loading...");
                progressDialog.setMessage("Checking Root access");
                progressDialog.show();
            });
        }

        @Override
        protected Boolean doInBackground(Boolean... booleans) {
            new Thread(() -> new Handler(Looper.getMainLooper()).post(() -> {
                if (Shell.rootAccess())
                    Logger.makeLog(Logger.INFO, "ROOT").show();
                else
                    Logger.makeLog(Logger.INFO, "NO ROOT").show();
            })).start();
            return false;
        }

        @Override
        protected void onPostExecute(Boolean aBoolean) {
            super.onPostExecute(aBoolean);
            activity.runOnUiThread(() -> progressDialog.dismiss());
        }
    }
    public static void checkRoot(Activity activity){
       new ooo(activity)
               .execute();
    }
}
