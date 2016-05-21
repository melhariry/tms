package com.elgp2.verifonetms;

import android.annotation.TargetApi;
import android.app.Service;
import android.content.Intent;
import android.os.Build;
import android.os.IBinder;
import android.provider.Settings;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.util.Log;

import com.elgp2.verifonetms.communication.Communication;
import com.elgp2.verifonetms.communication.FtpClient;
import com.elgp2.verifonetms.communication.MyVolley;
import com.elgp2.verifonetms.models.AppInfo;
import com.elgp2.verifonetms.models.FileInfo;
import com.elgp2.verifonetms.utilities.SystemUtil;

import java.util.AbstractMap;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.BitSet;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

/**
 * Created by POS_GP_TEAM_2016 on 5/9/2016.
 */
public class Manager extends Service {

    /**
     * Tag for Debugging
     */
    private final String Tag = "Manager";

    private Communication mCommunication;

    private Execute mExecute;

    private FtpClient ftpClient;
    @Override
    public void onCreate() {
        MyVolley.init(this);
        mCommunication = new Communication();
        mExecute = new Execute(this);
        ftpClient = new FtpClient();
        Log.d(Tag, "Manager Service on Create");
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.d(Tag,"Manager Service started");
        new Thread(new Runnable() {
            @Override
            public void run() {
                posUp();
            }
        }).start();

        return Service.START_STICKY;
    }


    @Override
    public IBinder onBind(Intent arg0) {
        return null;
    }

    @Override
    public void onDestroy() {
        if(MyVolley.getRequestQueue() != null)
            MyVolley.getRequestQueue().cancelAll("Comm");
        Log.d(Tag, "Manager Service destroyed");
    }


    @TargetApi(Build.VERSION_CODES.KITKAT)
    private void posUp(){
        try {
            mCommunication.posUp();

            int response = Integer.valueOf(waitForResponse());

            if(response != -1){
                BitSet b = BitSet.valueOf(new long[]{response});

                if(b.get(0)){
                    // test POS command
                    submitTestHealth();
                    waitForResponse();
                }
                if(b.get(1)){
                    //listFiles
                    submitFileList();
                    waitForResponse();
                }
                if(b.get(2)){
                    //list apps command
                    submitAppList();
                    waitForResponse();
                }
                if(b.get(3)){
                    //delete files command
                    String[] fileNames = requestCommandParameters("DeleteFile");
                    if(fileNames != null){
                        submitDeleteFiles(fileNames);
                        waitForResponse();
                    }
                }
                if(b.get(4)){
                    //push file command
                    String [] filePaths = requestCommandParameters("PushFile");
                    if(filePaths != null){
                        submitFTP(filePaths, false);
                        waitForResponse();
                    }
                }
                if(b.get(5)){
                    //pull file command
                    String [] filePaths = requestCommandParameters("PullFile");
                    if(filePaths != null){
                        submitFTP(filePaths,true);
                        waitForResponse();
                    }
                }
                if(b.get(6)){
                    //update apps command
                    String [] appNames = requestCommandParameters("UpdateApp");
                    if(appNames != null){

                    }
                }
                if(b.get(7)){
                    // create POS record command
                    Log.d(Tag,"create POS record");
                    mCommunication.createPosRecord(this);
                    waitForResponse();
                }
                //finish commands execution
                //submitFinishCommand();
                //waitForResponse();
            }
            else{
                Log.d(Tag, "error after sending PosUP service terminating");
                //TODO add error handler to terminate the service and reschedule it
            }
        }
        catch (Exception e){
            e.printStackTrace();
            Log.d(Tag , "response parsing failed");
        }
        //stop the service TODO reschedule it
        this.stopSelf();
    }



    private String [] requestCommandParameters(String command){
        mCommunication.requestCommandParameters(command);
        String response = waitForResponse();
        String [] params = null;
        if(response != "-1"){
            try{
             params = response.split(";");
            }
            catch(Exception e){
                e.printStackTrace();
                Log.d(Tag, "response for command parameters parsing failed");
            }
        }
        return params;
    }

    private void submitFinishCommand(){
        List<Map.Entry<String,String>> params= new ArrayList<Map.Entry<String, String>>();
        params.add(new AbstractMap.SimpleEntry<String, String>("Command", "Finish"));
        mCommunication.submitCommandResults(params);
    }

    private void submitAppList(){
        List<Map.Entry<String,String>> params= new ArrayList<Map.Entry<String, String>>();
        params.add(new AbstractMap.SimpleEntry<String, String>("Command", "ListApps"));
        List<AppInfo> appList =mExecute.getInstalledApps();
        for(AppInfo appInfo: appList){
            params.add(new AbstractMap.SimpleEntry<String, String>("Name", appInfo.getAppLabel()));
            params.add(new AbstractMap.SimpleEntry<String, String>("Version", "3"));
            params.add(new AbstractMap.SimpleEntry<String, String>("Com", appInfo.getCompanyName()));
        }
        mCommunication.submitCommandResults(params);
    }

    private void submitFileList(){
        List<Map.Entry<String,String>> params = new ArrayList<Map.Entry<String, String>>();
        params.add(new AbstractMap.SimpleEntry<String, String>("Command","ListFiles"));
        List<FileInfo> fileList = mExecute.getFileInfoList();
        for(FileInfo fileInfo : fileList){
            params.add(new AbstractMap.SimpleEntry<String, String>("Name",fileInfo.getName()));
            params.add(new AbstractMap.SimpleEntry<String, String>("Type",String.valueOf(fileInfo.getIsFolder())));
            params.add(new AbstractMap.SimpleEntry<String, String>("Parent",fileInfo.getParentName()));
            params.add(new AbstractMap.SimpleEntry<String, String>("Size",String.valueOf(fileInfo.getFileSize())));
        }
        mCommunication.submitCommandResults(params);
    }

    private void submitDeleteFiles(String [] paths){
        List<Map.Entry<String,String>> params= new ArrayList<Map.Entry<String, String>>();
        params.add(new AbstractMap.SimpleEntry<String, String>("Command", "DeleteFile"));
        List<Boolean> deleteResults = mExecute.deleteFiles(Arrays.asList(paths));
        for(int i = 0 ; i< paths.length ; i++){
            params.add(new AbstractMap.SimpleEntry<String, String>("Name", paths[i]));
            if(deleteResults.get(i))
                params.add(new AbstractMap.SimpleEntry<String, String>("Status", "0"));
            else
                params.add(new AbstractMap.SimpleEntry<String, String>("Status", "1"));
        }
        mCommunication.submitCommandResults(params);
    }

    private void submitFTP(String[] filePaths,Boolean pull) {
        List<Map.Entry<String,String>> params= new ArrayList<Map.Entry<String, String>>();
        params.add(new AbstractMap.SimpleEntry<String, String>("Command", "PushFile"));
        for(String path : filePaths){
            params.add(new AbstractMap.SimpleEntry<String, String>("Path",path));
            if(pull)
                ftpClient.downloadFile(path,this);
            else
                ftpClient.uploadFile(path,this);
           Boolean status = waitForFtp();
            if(status)
                params.add(new AbstractMap.SimpleEntry<String, String>("Status","0"));
            else
                params.add(new AbstractMap.SimpleEntry<String, String>("Status","1"));
        }
        mCommunication.submitCommandResults(params);
    }

    private void submitTestHealth(){
        List<Map.Entry<String,String>> params= new ArrayList<Map.Entry<String, String>>();
        params.add(new AbstractMap.SimpleEntry<String, String>("Command", "TestHealth"));
        params.add(new AbstractMap.SimpleEntry<String, String>("Crypto", "0"));
        params.add(new AbstractMap.SimpleEntry<String, String>("Printer", "0"));
        params.add(new AbstractMap.SimpleEntry<String, String>("Timer", "0"));
        params.add(new AbstractMap.SimpleEntry<String, String>("Buzzer", "0"));
        params.add(new AbstractMap.SimpleEntry<String, String>("Led", "0"));
        params.add(new AbstractMap.SimpleEntry<String, String>("Rtc", "0"));
        params.add(new AbstractMap.SimpleEntry<String, String>("Memory", "0"));
        params.add(new AbstractMap.SimpleEntry<String, String>("UsedDiskSize", String.valueOf(SystemUtil.getTotalDiskSpace(this) - SystemUtil.getFreeDiskSpace(this))));
        params.add(new AbstractMap.SimpleEntry<String, String>("UsedRamSize", String.valueOf(SystemUtil.getTotalRamSize() - SystemUtil.getFreeRamSize())));
        mCommunication.submitCommandResults(params);
    }

    private Boolean waitForFtp() {

        while(ftpClient.getStatus() != FtpClient.ftpStatus.FTPCOMPLETED && ftpClient.getStatus() != FtpClient.ftpStatus.FTPERROR ){
            //TODO check this condition
        }
        if(ftpClient.getStatus() == FtpClient.ftpStatus.FTPCOMPLETED)
            return true;
        else
            return false;
    }

    /**
     * helper method to block thread till response/error is sent back
     * @return response
     */
    private String waitForResponse(){
        int count =0;
        String response = "-1";
        while(mCommunication.getStatus() != Communication.commStatus.ERROR && mCommunication.getStatus() != Communication.commStatus.RESPONSE){
            if(count > 30) {
                return response;
            }
            try {
                count++;
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        if(mCommunication.getStatus() == Communication.commStatus.RESPONSE){
             response = mCommunication.getResponse();
        }
        else{
            Log.d(Tag,"transmission error");
            Log.e(Tag, "Volley error: " + mCommunication.getError());
            return response;
        }
        return response;
    }
}
