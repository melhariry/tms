package com.elgp2.verifonetms.utilities;

import android.app.ActivityManager;
import android.content.Context;
import android.os.Build;
import android.os.Environment;
import android.os.StatFs;

import com.elgp2.verifonetms.TMSApplication;

import java.io.File;
import java.text.DecimalFormat;

/**
 * Created by POS_GP_TEAM_2016 on 5/2/2016.
 */
public class SystemUtil {

    private SystemUtil(){}

    public static String getMachineSerial(){
        return Build.SERIAL;
    }

    public static String getMachineManufacturer(){
        return Build.MANUFACTURER;
    }

    public static String getMachineBrand(){
        return Build.BRAND;
    }


    public static String getMachineModel(){
        return Build.MODEL;

    }

    public static int  getMachineSDKVersion(){
        return Build.VERSION.SDK_INT;
    }

    public static String getMachineVersionRelease(){
        return Build.VERSION.RELEASE;
    }

    public static long getFreeRamSize(){
        ActivityManager.MemoryInfo memoryInfo = getSystemMemoryInfo(TMSApplication.getInstance());
        return memoryInfo.availMem/(1024*1024);
    }

    public static long getTotalRamSize(){
        ActivityManager.MemoryInfo memoryInfo = getSystemMemoryInfo(TMSApplication.getInstance());
        return memoryInfo.totalMem/(1024*1024);
    }

    public static long getFreeDiskSpace(Context context){
        File internalStorageFile=context.getFilesDir();
        return internalStorageFile.getFreeSpace()/(1024*1024);
    }

    public static long getTotalDiskSpace(Context context){
        File internalStorageFile=context.getFilesDir();
       return internalStorageFile.getTotalSpace()/(1024*1024);
    }

    /*source: http://stackoverflow.com/questions/7115016/how-to-find-the-amount-of-free-storage-disk-space-left-on-android*/
    public static String floatForm (double d) {
        return new DecimalFormat("#.##").format(d);
    }

    /*source :http://stackoverflow.com/questions/7115016/how-to-find-the-amount-of-free-storage-disk-space-left-on-android*/
    public static String bytesToHuman (long size) {
        long Kb = 1  * 1024;
        long Mb = Kb * 1024;
        long Gb = Mb * 1024;
        long Tb = Gb * 1024;
        long Pb = Tb * 1024;
        long Eb = Pb * 1024;

        if (size <  Kb)                 return floatForm(        size     ) + " byte";
        if (size >= Kb && size < Mb)    return floatForm((double)size / Kb) + " Kb";
        if (size >= Mb && size < Gb)    return floatForm((double)size / Mb) + " Mb";
        if (size >= Gb && size < Tb)    return floatForm((double)size / Gb) + " Gb";
        if (size >= Tb && size < Pb)    return floatForm((double)size / Tb) + " Tb";
        if (size >= Pb && size < Eb)    return floatForm((double)size / Pb) + " Pb";
        if (size >= Eb)                 return floatForm((double)size / Eb) + " Eb";

        return "???";
    }

    private static ActivityManager.MemoryInfo getSystemMemoryInfo(Context context){
        ActivityManager.MemoryInfo memoryInfo = new ActivityManager.MemoryInfo();
        ActivityManager activityManager = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
        activityManager.getMemoryInfo(memoryInfo);
        return memoryInfo;
    }


}
