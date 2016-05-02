package com.elgp2.verifonetms.models;

import java.io.File;

/**
 * Created by POS_GP_TEAM_2016 on 5/1/2016.
 */
public class FileInfo {

    /*absolute path of the file*/
    private String fileAbsolutePath;
    /*fileSize in Bytes*/
    private long fileSize;
    /*attribute of file like in Castle true-> public file , false -> private file */
    private boolean attribute;

    public void setFileAbsolutePath(String fileAbsolutePath) {
        this.fileAbsolutePath = fileAbsolutePath;
    }

    public void setFileSize(long fileSize) {
        this.fileSize = fileSize;
    }

    public void setAttribute(boolean attribute) {
        this.attribute = attribute;
    }

    public long getFileSize() {
        return fileSize;
    }

    public String getFileAbsolutePath() {
        return fileAbsolutePath;
    }

    public boolean isPublic() {
        return attribute;
    }

}
