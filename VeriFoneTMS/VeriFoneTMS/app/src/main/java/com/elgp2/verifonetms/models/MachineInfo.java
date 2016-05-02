package com.elgp2.verifonetms.models;

/**
 * Created by POS_GP_TEAM_2016 on 5/2/2016.
 */
public class MachineInfo {

    private String serialNumber;

    private String manufacturer;

    private String brand;

    private String model;

    private int sdkVersion;

    private String versionRelease;

    private long freeRamSize;

    private long totalRamSize;

    private long freeDiskSpace;

    private long totalDiskSpace;


    public void setTotalDiskSpace(long totalDiskSpace) {
        this.totalDiskSpace = totalDiskSpace;
    }

    public void setSerialNumber(String serialNumber) {
        this.serialNumber = serialNumber;
    }

    public void setManufacturer(String manufacturer) {
        this.manufacturer = manufacturer;
    }

    public void setBrand(String brand) {
        this.brand = brand;
    }

    public void setModel(String model) {
        this.model = model;
    }

    public void setSdkVersion(int sdkVersion) {
        this.sdkVersion = sdkVersion;
    }

    public void setVersionRelease(String versionRelease) {
        this.versionRelease = versionRelease;
    }

    public void setFreeRamSize(long freeRamSize) {
        this.freeRamSize = freeRamSize;
    }

    public void setTotalRamSize(long totalRamSize) {
        this.totalRamSize = totalRamSize;
    }

    public void setFreeDiskSpace(long freeDiskSpace) {
        this.freeDiskSpace = freeDiskSpace;
    }

    public String getSerialNumber() {
        return serialNumber;
    }

    public String getManufacturer() {
        return manufacturer;
    }

    public String getBrand() {
        return brand;
    }

    public String getModel() {
        return model;
    }

    public int getSdkVersion() {
        return sdkVersion;
    }

    public String getVersionRelease() {
        return versionRelease;
    }

    public long getFreeRamSize() {
        return freeRamSize;
    }

    public long getTotalRamSize() {
        return totalRamSize;
    }

    public long getFreeDiskSpace() {
        return freeDiskSpace;
    }

    public long getTotalDiskSpace() {
        return totalDiskSpace;
    }
}
