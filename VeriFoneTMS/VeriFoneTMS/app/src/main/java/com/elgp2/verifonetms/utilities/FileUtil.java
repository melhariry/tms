package com.elgp2.verifonetms.utilities;

import java.io.File;
import java.util.List;

/**
 * Created by POS_GP_TEAM_2016 on 5/1/2016.
 */
public class FileUtil {

    private FileUtil(){}

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
