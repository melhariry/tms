package com.elgp2.verifonetms.utilities;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Environment;

import com.elgp2.verifonetms.models.AppInfo;

import java.util.List;
import java.util.regex.Pattern;

/**
 * Created by POS_GP_TEAM_2016 on 5/1/2016.
 */
public class AppUtil {

    private AppUtil(){
    }

    public static String getCompanyName(String packageName){

        //remove the company domain (com. , net.,etc)
        packageName = packageName.substring(4);
        if(packageName.contains("."))
            return packageName.split("\\.")[0];
        else
            return  packageName;

    }

    public static Boolean isAppInstalled(String apkName,Context context, List<AppInfo> installedApps) {
        final PackageManager pm = context.getPackageManager();
        String fullPath = Environment.getExternalStorageDirectory() + "/MTMS/" + apkName;
        PackageInfo info = pm.getPackageArchiveInfo(fullPath, 0);
        for(AppInfo app : installedApps){
            if(app.getVersionName().equals(info.versionName) && app.getPackageName().equals(info.packageName) && app.getVersionCode() == info.versionCode){
                return true;
            }
        }
        return false;
    }
}
