package com.elgp2.verifonetms;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.nfc.Tag;
import android.os.Environment;
import android.support.annotation.BoolRes;
import android.support.v4.content.res.TypedArrayUtils;
import android.util.Log;

import com.elgp2.verifonetms.models.AppInfo;
import com.elgp2.verifonetms.models.FileInfo;
import com.elgp2.verifonetms.models.MachineInfo;
import com.elgp2.verifonetms.utilities.AppUtil;
import com.elgp2.verifonetms.utilities.FileUtil;
import com.elgp2.verifonetms.utilities.SystemUtil;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

/**
 * Execute different machine related actions (e.g : list_apps, delete_files, update_apps,etc)
 * Created by POS_GP_TEAM_2016 on 4/29/2016.
 */
public class Execute {

    /*Tag for debugging*/
     private final String Tag= "Execute";
     private final Context context;

    public Execute() {
        context=TMSApplication.getInstance();
    }

    /*returns a list of all installed apps ont the system*/
    public List<AppInfo> getInstalledApps(){
        final PackageManager packageManager = context.getPackageManager();
        //list of installed apps
        List<PackageInfo> apps = packageManager.getInstalledPackages(0);

        List<AppInfo> appsInfo=new ArrayList<AppInfo>();
        for(PackageInfo packageInfo:apps) {
            //user installed non-system app
            //TODO remove this if case if we need to send system apps also
           if( (packageInfo.applicationInfo.flags & ApplicationInfo.FLAG_SYSTEM) == 0){
                AppInfo appInfo = new AppInfo();

                appInfo.setAppLabel(packageManager.getApplicationLabel(packageInfo.applicationInfo).toString());
                appInfo.setVersionName(packageInfo.versionName);
                appInfo.setCompanyName(AppUtil.getCompanyName(packageInfo.packageName));

                appsInfo.add(appInfo);
           }
        }

        return appsInfo;
    }

    public List<FileInfo> getPublicFiles(){
        return listFiles(Environment.getExternalStorageDirectory().getAbsolutePath(),true);
    }
    /*returns list of private files(files inside our app folder)*/
    public List<FileInfo> getPrivateFiles(){
        return listFiles(context.getFilesDir().getAbsolutePath(), false);
    }

    public List<Boolean> deleteFiles(List<String>  filesPaths){
        List<Boolean> results = new ArrayList<Boolean>();
        if(filesPaths != null && filesPaths.size() > 0){
            for(String filePath:filesPaths){
                results.add(FileUtil.deleteDir(filePath));
            }
        }
        return results;
    }

    /*
     *creates a new file or directory on device external memory
    * @relativePath : path within the external memory (e.g: myDir1/myDir2)
    * @fileName : name of file to be create (e.g : myFile.txt)
    * returns true on success false otherwise
    * */

    public Boolean createPublicFile(String relativePath,String fileName){
        return FileUtil.createPublicFile(relativePath,fileName);
    }

    /*
    *creates a new file or directory on application internal storage
    * @relativePath : path within the external memory (e.g: myDir1/myDir2)
    * @fileName : name of file to be create (e.g : myFile.txt)
    * returns true on success false otherwise
    * */
    public Boolean createPrivateFile(String relativePath,String fileName){
        return FileUtil.createPrivateFile(relativePath, fileName, context);
    }

    /*returns list of files inside an absolute folder path*/
    private List<FileInfo> listFiles(String path,boolean isPublic){
      List<FileInfo> publicFileList=new ArrayList<FileInfo>();

      File externalStorageDir = new File( path );
      File[] list = externalStorageDir.listFiles();
      if(list != null){
          for(int i=0;i<list.length;i++) {
              File file = list[i];
              if(file.isDirectory()){
                 File[] temp = file.listFiles();
                  if(temp != null && temp.length >0 ) {
                      list = FileUtil.concat(list, temp);
                  }
                  //an Empty folder
                  else{
                      FileInfo fileInfo = new FileInfo();
                      //true for public file
                      fileInfo.setAttribute(isPublic);
                      fileInfo.setFileAbsolutePath(file.getAbsolutePath());
                      fileInfo.setFileSize(FileUtil.getFileSize(file));

                      publicFileList.add(fileInfo);
                  }
              }
              else {
                  FileInfo fileInfo = new FileInfo();
                  //true for public file
                  fileInfo.setAttribute(isPublic);
                  fileInfo.setFileAbsolutePath(file.getAbsolutePath());
                  fileInfo.setFileSize(FileUtil.getFileSize(file));

                  publicFileList.add(fileInfo);
              }
          }
      }
      return publicFileList;
    }

    /**
     * @return object contains machine info
     */
    public MachineInfo getMachineInfo(){
       MachineInfo machineInfo=new MachineInfo();

       machineInfo.setBrand(SystemUtil.getMachineBrand());
       machineInfo.setFreeDiskSpace(SystemUtil.getFreeDiskSpace());
       machineInfo.setFreeRamSize(SystemUtil.getFreeRamSize());
       machineInfo.setManufacturer(SystemUtil.getMachineManufacturer());
       machineInfo.setModel(SystemUtil.getMachineModel());
       machineInfo.setSdkVersion(SystemUtil.getMachineSDKVersion());
       machineInfo.setSerialNumber(SystemUtil.getMachineSerial());
       machineInfo.setTotalDiskSpace(SystemUtil.getTotalDiskSpace());
       machineInfo.setTotalRamSize(SystemUtil.getTotalRamSize());
       machineInfo.setVersionRelease(SystemUtil.getMachineVersionRelease());

       return machineInfo;
    }

}
