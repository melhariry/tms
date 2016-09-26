package com.elgp2.verifonetms.models;

/**
 * Created by POS_GP_TEAM_2016 on 5/31/2016.
 * this class holds all info needed to connect with MTMS server that is usually retrieved from commConfig file
 */
public class CommInfo {

    /**
     * default server IP
     */
    private String firstServerIP;
    /**
     * backup server ip
     */
    private String secondServerIP;
    /**
     * user name of FTP server
     */
    private String FTPUserName;
    /**
     * password to FTP Server
     */
    private String FTPPassword;
    /**
     * wakeup app Manager service interval
     */
    private long wakeUpInterval;

    public String getFirstServerIP() {
        return firstServerIP;
    }

    public String getSecondServerIP() {
        return secondServerIP;
    }

    public String getFTPUserName() {
        return FTPUserName;
    }

    public String getFTPPassword() {
        return FTPPassword;
    }

    public long getWakeUpInterval() {
        return wakeUpInterval;

    }

    public void setFirstServerIP(String firstServerIP) {
        this.firstServerIP = firstServerIP;
    }

    public void setSecondServerIP(String secondServerIP) {
        this.secondServerIP = secondServerIP;
    }

    public void setFTPUserName(String FTPUserName) {
        this.FTPUserName = FTPUserName;
    }

    public void setFTPPassword(String FTPPassword) {
        this.FTPPassword = FTPPassword;
    }

    public void setWakeUpInterval(long wakeUpInterval) {
        this.wakeUpInterval = wakeUpInterval;
    }

}
