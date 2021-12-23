package com.sharkcheat.pubgmaimbottouchtest;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Process;
import android.provider.Settings;

import androidx.appcompat.app.AppCompatActivity;

import com.topjohnwu.superuser.Shell;

import java.io.File;

public class MainActivity extends AppCompatActivity {
    private int REQUEST_OVERLAY = 5500;
    private File daemonPath;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.service_layout);
        ShellUtils.checkRoot(this);
        if (requestOverlayPermission()){
            initDaemon();
            MyService.Start(this);
        }
    }

    private void initDaemon() {
        daemonPath = new File(getFilesDir().getPath(), "libPUBGMDaemon.so");
        FileCommons.copyFromAssets(this, getFilesDir().getPath(), "libPUBGMDaemon.so");
        if (daemonPath.exists())
            daemonPath.setExecutable(true, true);
    }

    private boolean requestOverlayPermission() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (Settings.canDrawOverlays(this)) {
                return true;
            } else {
                AlertDialog.Builder builder = new AlertDialog.Builder(this);
                builder.setMessage("This application requires overlay permission please permission first");
                builder.setNegativeButton("EXIT", (dialogInterface, i) -> {
                    finish();
                    Process.killProcess(Process.myPid());
                });
                builder.setPositiveButton("OK", (dialogInterface, i) -> {
                    Intent intent = new Intent(Settings.ACTION_MANAGE_OVERLAY_PERMISSION, Uri.parse("package:" + getPackageName()));
                    startActivityForResult(intent, REQUEST_OVERLAY);
                });
                builder.setCancelable(false);
                builder.show();
            }
        } else {
            return true;
        }
        return false;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == REQUEST_OVERLAY) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                requestOverlayPermission();
            }
        }
    }
}