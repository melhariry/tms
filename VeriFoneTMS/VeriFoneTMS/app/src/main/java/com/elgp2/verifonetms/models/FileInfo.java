package com.elgp2.verifonetms.models;

import java.io.File;

/**
 * Created by POS_GP_TEAM_2016 on 5/1/2016.
 */
public class FileInfo {

    /*absolute path of the file*/
    private String name;

    private String parentName;
    /*fileSize in Bytes*/
    private long fileSize;
    private int isFolder;

    public void setName(String name) {
        this.name = name;
    }

    public void setParentName(String parentName) {
        this.parentName = parentName;
    }

    public void setFileSize(long fileSize) {
        this.fileSize = fileSize;
    }

    public void setIsFolder(int isFolder) {
        this.isFolder = isFolder;
    }

    public String getName() {
        return name;
    }

    public int getIsFolder() {
        return isFolder;
    }

    public long getFileSize() {
        return fileSize;
    }

    public String getParentName() {
        return parentName;
    }
}
