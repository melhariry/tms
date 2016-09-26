package com.elgp2.verifonetms.models;

/**
 * Created by POS_GP_TEAM_2016 on 5/1/2016.
 */
/*
* Class to represent info of applications installed on the machine
* */
public class AppInfo {

    private String appLabel;
    private String companyName;
    private String versionName;
    private String packageName;
    private int versionCode;

    public int getVersionCode() {
        return versionCode;
    }

    public void setVersionCode(int versionCode) {
        this.versionCode = versionCode;
    }

    public String getPackageName() {
        return packageName;
    }

    public void setPackageName(String packageName) {
        this.packageName = packageName;
    }

    public void setAppLabel(String appLabel) {
        this.appLabel = appLabel.replace(" ","");
    }

    public void setCompanyName(String companyName) {
        this.companyName = companyName;
    }

    public void setVersionName(String versionName) {
        this.versionName = versionName;
    }

    public String getAppLabel() {
        return appLabel;
    }

    public String getCompanyName() {
        return companyName;
    }

    public String getVersionName() {
        return versionName;
    }
}
