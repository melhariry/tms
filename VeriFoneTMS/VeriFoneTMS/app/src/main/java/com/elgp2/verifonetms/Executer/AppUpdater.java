package com.elgp2.verifonetms.Executer;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Environment;
import android.util.Log;
import android.widget.Toast;

import com.elgp2.verifonetms.Manager;
import com.elgp2.verifonetms.ui.MainActivity;
import com.elgp2.verifonetms.utilities.FileUtil;

import java.io.File;

/**
 * Created by POS_GP_TEAM_2016 on 5/22/2016.
 */
public class AppUpdater extends BroadcastReceiver {

    private final String Tag = "AppUpdater";

    public  enum AppUpdateStatus {
        UPDATE_STARTED,
        UPDATE_FINISHED
    }

    public static  AppUpdateStatus updateStatus = AppUpdateStatus.UPDATE_STARTED;

    public AppUpdater(){
    }

    public void updateApp(String appName,Context context){
        updateStatus = AppUpdateStatus.UPDATE_STARTED;
        Intent intent = new Intent(Intent.ACTION_VIEW);
        intent.setDataAndType(Uri.fromFile(new File(Environment.getExternalStorageDirectory() + "/MTMS/" + appName)), "application/vnd.android.package-archive");
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        context.startActivity(intent);
    }

    @Override
    public void onReceive(Context context, Intent intent) {
        if(intent.getAction() != null) {
            if (intent.getAction().equals(Intent.ACTION_PACKAGE_ADDED)) {
                Log.d(Tag, "Package_ADDED");
                updateStatus = AppUpdateStatus.UPDATE_FINISHED;
            } else if (intent.getAction().equals(Intent.ACTION_PACKAGE_REPLACED)) {
                Log.d(Tag, "Package Replaced");
                updateStatus = AppUpdateStatus.UPDATE_FINISHED;
            } else if (intent.getAction().equals("android.intent.action.BOOT_COMPLETED")) {
                Intent managerIntent = new Intent(context, Manager.class);
                context.startService(managerIntent);
            } else if(intent.getAction().equals("com.elgp2.alarmAction")) {
                Intent managerIntent = new Intent(context, Manager.class);
                context.startService(managerIntent);
            }
        }

    }
}
