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

    private List<FileInfo> FileList;

    public Execute(Context c)
    {
        context=c;
        FileList =new ArrayList<FileInfo>();
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

    public List<FileInfo> getFileInfoList(){
        List<FileInfo> dragonFileList = new ArrayList<FileInfo>();
        FileList.clear();
        //list public files
        listFiles(Environment.getExternalStorageDirectory().getAbsolutePath()+"/Download","",true);
        dragonFileList.addAll(FileList);
        FileList.clear();
        listFiles(context.getFilesDir().getAbsolutePath(),"",false);
        dragonFileList.addAll(FileList);
        FileList.clear();
        return dragonFileList;
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
        return FileUtil.createPublicFile(relativePath, fileName);
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

    //list private file data and puts the results in privateFileList
    public void listFiles(String path,String parentName, Boolean isPublic){
        File currentFolder =new File(path);
        File [] list = currentFolder.listFiles();
        FileInfo currentFolderInfo = new FileInfo();
        if(!parentName.isEmpty()) {
            currentFolderInfo.setName(currentFolder.getName().replace(" ",""));
            currentFolderInfo.setIsFolder(1);
            currentFolderInfo.setParentName(parentName.replace(" ",""));
            currentFolderInfo.setFileSize(0);
            FileList.add(currentFolderInfo);
        }
        if(list != null && list.length >0){
            for(File fileWalker : list){
                if(fileWalker.isDirectory()){
                    if(!parentName.isEmpty())
                        listFiles(fileWalker.getAbsolutePath(), currentFolder.getName(),isPublic);
                    else{
                        if(isPublic)
                            listFiles(fileWalker.getAbsolutePath(),"pub",isPublic);
                        else
                            listFiles(fileWalker.getAbsolutePath(), "pri",isPublic);
                    }
                }
                else{
                    FileInfo currentFileInfo = new FileInfo();
                    currentFileInfo.setFileSize(FileUtil.getFileSize(fileWalker));
                    if(parentName.isEmpty()) {
                        if(isPublic)
                            currentFileInfo.setParentName("pub");
                        else
                            currentFileInfo.setParentName("pri");
                    }
                    else
                        currentFileInfo.setParentName(currentFolder.getName().replace(" ",""));
                    currentFileInfo.setIsFolder(0);
                    currentFileInfo.setName(fileWalker.getName().replace(" ",""));
                    FileList.add(currentFileInfo);
                }
            }
        }
        else{
            //empty folder
            if(parentName.isEmpty()){
                FileInfo emptyFolderInfo = new FileInfo();
                emptyFolderInfo.setName(currentFolder.getName().replace(" ", ""));
                emptyFolderInfo.setIsFolder(1);
                if(isPublic)
                    emptyFolderInfo.setParentName("pub");
                else
                    emptyFolderInfo.setParentName("pri");
                emptyFolderInfo.setFileSize(0);
                FileList.add(emptyFolderInfo);
            }
        }
    }

    /**
     * @return object contains machine info
     */
    public MachineInfo getMachineInfo(){
       MachineInfo machineInfo=new MachineInfo();

       machineInfo.setBrand(SystemUtil.getMachineBrand());
       machineInfo.setFreeDiskSpace(SystemUtil.getFreeDiskSpace(context));
       machineInfo.setFreeRamSize(SystemUtil.getFreeRamSize());
       machineInfo.setManufacturer(SystemUtil.getMachineManufacturer());
       machineInfo.setModel(SystemUtil.getMachineModel());
       machineInfo.setSdkVersion(SystemUtil.getMachineSDKVersion());
       machineInfo.setSerialNumber(SystemUtil.getMachineSerial());
       machineInfo.setTotalDiskSpace(SystemUtil.getTotalDiskSpace(context));
       machineInfo.setTotalRamSize(SystemUtil.getTotalRamSize());
       machineInfo.setVersionRelease(SystemUtil.getMachineVersionRelease());

       return machineInfo;
    }
}
