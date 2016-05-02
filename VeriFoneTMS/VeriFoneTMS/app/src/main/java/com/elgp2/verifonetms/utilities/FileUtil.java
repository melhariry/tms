package com.elgp2.verifonetms.utilities;

import android.content.Context;
import android.nfc.Tag;
import android.os.Environment;
import android.provider.MediaStore;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import com.elgp2.verifonetms.models.FileInfo;

import java.io.File;
import java.io.IOException;
import java.util.List;

/**
 * Created by POS_GP_TEAM_2016 on 5/1/2016.
 */
public class FileUtil {

    /*Tag for debugging*/
    private final static String Tag ="FileUtil";

    private FileUtil(){}

    /*
    *creates a new file or directory on device external memory
    * @relativePath : path within the external memory (e.g: myDir1/myDir2)
    * @fileName : name of file to be create (e.g : myFile.txt)
    * returns true on success false otherwise
    * */
    public static Boolean createPublicFile(String relativePath,String fileName){
        File myDir= new File(Environment.getExternalStorageDirectory().getAbsolutePath()+"/"+relativePath+"/");
        //create the parent folders if they are not already created
        myDir.mkdirs();
        try{
            File file = new File(myDir,fileName);
            if(!file.exists()) {
                if(!file.createNewFile()) {
                    Log.d(Tag, "file unable to be created");
                    return false;
                }
            }
        }
        catch(NullPointerException e){
            Log.d(Tag,"NULL pointer Exception");
            e.printStackTrace();
            return false;
        } catch (IOException e) {
            Log.d(Tag,"IOException at file creation");
            e.printStackTrace();
            return false;
        }
        return true;
    }

    /*
    *creates a new file or directory on application internal storage
    * @relativePath : path within the external memory (e.g: myDir1/myDir2)
    * @fileName : name of file to be create (e.g : myFile.txt)
    * @context Application context
    * returns true on success false otherwise
    * */
    public static Boolean createPrivateFile(String relativePath, String fileName,Context context){
        File myDir = new File(context.getFilesDir().getAbsolutePath()+"/"+relativePath+"/");
        //create the parent folders if they are not already created
        myDir.mkdirs();
        try{
            File file = new File(myDir,fileName);
            if(!file.exists()) {
                if(!file.createNewFile()) {
                    Log.d(Tag, "file unable to be created");
                    return false;
                }
            }
        }
        catch(NullPointerException e){
            Log.d(Tag,"NULL pointer Exception");
            e.printStackTrace();
            return false;
        } catch (IOException e) {
            Log.d(Tag,"IOException at file creation");
            e.printStackTrace();
            return false;
        }
        return true;
    }

    /*returns the file size in Bytes*/
    public static long getFileSize(File f){
        long size =0 ;

        if(f.isDirectory()){
            File[] list = f.listFiles();
            if(list != null) {
                for (File file : list) {
                    size += getFileSize(file);
                }
            }
        }
        else{
            size = f.length();
        }
        return size;
    }

    /*delete file or Dir note, if directory is to be deleted all files in it must be deleted first then delete the folder itself*/
    public static Boolean deleteDir(String absolutePath){
        File file = new File(absolutePath);
        if(file.isDirectory()){
            File[] childFiles = file.listFiles();
            if(childFiles != null && childFiles.length > 0){
                for(File child: childFiles ){
                    deleteDir(child.getAbsolutePath());
                }
            }
        }
        return file.delete();
    }

    /*Concat two File arrays */
    public static File[] concat(File[] a, File[] b) {
        int aLen = a.length;
        int bLen = b.length;
        File[] c= new File[aLen+bLen];
        System.arraycopy(a, 0, c, 0, aLen);
        System.arraycopy(b, 0, c, aLen, bLen);
        return c;
    }
}
