package com.elgp2.verifonetms;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;

import java.util.List;

/**
 * Execute different machine related actions (e.g : list_apps, delete_files, update_apps,etc)
 * Created by Ibrahim Hussiny on 4/29/2016.
 */
public class Execute {


    /*returns a list of all installed apps ont the system*/
    public List<PackageInfo> getInstalledApps(Context context){
        final PackageManager packageManager = context.getPackageManager();
        //list of installed apps
        List<PackageInfo> apps = packageManager.getInstalledPackages(0);
        return apps;
    }

}
