package com.elgp2.verifonetms;

import android.annotation.TargetApi;
import android.app.Activity;
import android.app.AlarmManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.os.IBinder;
import android.os.SystemClock;
import android.util.Log;
import android.widget.Toast;

import com.elgp2.verifonetms.Executer.AppUpdater;
import com.elgp2.verifonetms.Executer.Execute;
import com.elgp2.verifonetms.communication.Communication;
import com.elgp2.verifonetms.communication.Constants;
import com.elgp2.verifonetms.communication.FtpClient;
import com.elgp2.verifonetms.communication.MyVolley;
import com.elgp2.verifonetms.communication.NukeSSLCerts;
import com.elgp2.verifonetms.models.AppInfo;
import com.elgp2.verifonetms.models.CommInfo;
import com.elgp2.verifonetms.models.FileInfo;
import com.elgp2.verifonetms.utilities.AppUtil;
import com.elgp2.verifonetms.utilities.FileUtil;
import com.elgp2.verifonetms.utilities.SystemUtil;

import java.util.AbstractMap;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.BitSet;
import java.util.List;
import java.util.Map;

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

    private AppUpdater mAppUpdater;

    private AlarmManager alarmMgr;
    private PendingIntent alarmIntent;

    @Override
    public void onCreate() {
        MyVolley.init(this);
        CommInfo commInfo = FileUtil.getCommParameters(this);
        mCommunication = new Communication(commInfo);
        mExecute = new Execute(this);
        ftpClient = new FtpClient(commInfo);
        mAppUpdater = new AppUpdater();
        Log.d(Tag, "Manager Service on Create");
        //Trust all SSL Certificates
        NukeSSLCerts.nuke();
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
        //cancel all requests waiting in Queue
        if(MyVolley.getRequestQueue() != null)
            MyVolley.getRequestQueue().cancelAll("Comm");
        Log.d(Tag, "Manager Service destroyed");
    }

    /**
     * handler to finish the running service and reschedule it to start after certain period
     */
    private void finishService(){
        try {
            alarmMgr = (AlarmManager)this.getSystemService(Context.ALARM_SERVICE);
            Intent intent = new Intent("com.elgp2.alarmAction");
            alarmIntent = PendingIntent.getBroadcast(this,0,intent,0);

            alarmMgr.set(AlarmManager.ELAPSED_REALTIME_WAKEUP,
                    SystemClock.elapsedRealtime() +
                            Constants.wakeupInterval * 1000, alarmIntent);
        } catch (Exception e) {
            e.printStackTrace();
        }
        this.stopSelf();
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
                        submitUpdateApp(appNames);
                        waitForResponse();
                    }
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
                if(b.get(7)){
                    // create POS record command
                    Log.d(Tag,"create POS record");
                    mCommunication.createPosRecord(this);
                    waitForResponse();
                    Log.d(Tag,"response waiting finished after update app command");
                }
                //finish commands execution
                submitFinishCommand();
                waitForResponse();
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
        //finish the current service and reschedules it
        finishService();
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
        params.add(new AbstractMap.SimpleEntry<>("Command", "Finish"));
        mCommunication.submitCommandResults(params);
    }

    private void submitAppList(){
        List<Map.Entry<String,String>> params= new ArrayList<Map.Entry<String, String>>();
        params.add(new AbstractMap.SimpleEntry<>("Command", "ListApps"));
        List<AppInfo> appList =mExecute.getInstalledApps();
        for(AppInfo appInfo: appList){
            params.add(new AbstractMap.SimpleEntry<>("Name", appInfo.getAppLabel()));
            params.add(new AbstractMap.SimpleEntry<>("Version", String.valueOf(appInfo.getVersionName())));
            params.add(new AbstractMap.SimpleEntry<>("Com", appInfo.getCompanyName()));
        }
        mCommunication.submitCommandResults(params);
    }

    private void submitFileList(){
        List<Map.Entry<String,String>> params = new ArrayList<Map.Entry<String, String>>();
        params.add(new AbstractMap.SimpleEntry<>("Command","ListFiles"));
        List<FileInfo> fileList = mExecute.getFileInfoList();
        for(FileInfo fileInfo : fileList){
            params.add(new AbstractMap.SimpleEntry<>("Name",fileInfo.getName()));
            params.add(new AbstractMap.SimpleEntry<>("Type",String.valueOf(fileInfo.getIsFolder())));
            params.add(new AbstractMap.SimpleEntry<>("Parent",fileInfo.getParentName()));
            params.add(new AbstractMap.SimpleEntry<>("Size",String.valueOf(fileInfo.getFileSize())));
        }
        mCommunication.submitCommandResults(params);
    }

    private void submitDeleteFiles(String [] paths){
        List<Map.Entry<String,String>> params= new ArrayList<Map.Entry<String, String>>();
        params.add(new AbstractMap.SimpleEntry<>("Command", "DeleteFile"));
        List<Boolean> deleteResults = mExecute.deleteFiles(Arrays.asList(paths));
        for(int i = 0 ; i< paths.length ; i++){
            params.add(new AbstractMap.SimpleEntry<>("Name", paths[i]));
            if(deleteResults.get(i))
                params.add(new AbstractMap.SimpleEntry<>("Status", "0"));
            else
                params.add(new AbstractMap.SimpleEntry<>("Status", "1"));
        }
        mCommunication.submitCommandResults(params);
    }

    private void submitFTP(String[] filePaths,Boolean pull) {
        List<Map.Entry<String,String>> params= new ArrayList<Map.Entry<String, String>>();
        if(!pull)
            params.add(new AbstractMap.SimpleEntry<>("Command", "PushFile"));
        else
            params.add(new AbstractMap.SimpleEntry<>("Command", "PullFile"));
        try {
            if (!ftpClient.connectFTP()) {
                for(String path : filePaths){
                    params.add(new AbstractMap.SimpleEntry<>("Path", path));
                    params.add(new AbstractMap.SimpleEntry<>("Status", "1"));
                }
            }
            else{
                for(String path : filePaths){
                    params.add(new AbstractMap.SimpleEntry<>("Path", path));
                    if(pull)
                        ftpClient.downloadFile(path,this);
                    else
                        ftpClient.uploadFile(path, this);
                    Boolean status = waitForFtp();
                    if(status)
                        params.add(new AbstractMap.SimpleEntry<>("Status", "0"));
                    else
                        params.add(new AbstractMap.SimpleEntry<>("Status","1"));
                }
            }
        }
        catch (Exception e){
            e.printStackTrace();
            Log.d(Tag,"ftp failed to connect");
            for(String path : filePaths){
                params.add(new AbstractMap.SimpleEntry<>("Path", path));
                params.add(new AbstractMap.SimpleEntry<>("Status", "1"));
            }
        }
        ftpClient.disConnectFtp();
        mCommunication.submitCommandResults(params);
    }

    private void submitUpdateApp(String[] appNames){

        List<Map.Entry<String,String>> params= new ArrayList<Map.Entry<String, String>>();
        params.add(new AbstractMap.SimpleEntry<>("Command", "UpdateApp"));

        try {
            if (!ftpClient.connectFTP()) {
                for(String appName : appNames){
                    params.add(new AbstractMap.SimpleEntry<>("Status", "1"));
                    params.add(new AbstractMap.SimpleEntry<>("Name", appName));
                }
            }
            else {
                List<AppInfo> installedApps = mExecute.getInstalledApps();
                for (String appName : appNames) {
                        Log.d(Tag,"app file name = "+ appName);
                        ftpClient.downloadAppFile(appName, this);
                        Boolean status = waitForFtp();
                    if(!AppUtil.isAppInstalled(appName,this,installedApps)) {
                        if (status){
                            Log.d(Tag, "App download started");
                            mAppUpdater.updateApp(appName, this);
                            int count =0;
                            Boolean installationFinished = true;
                            while (AppUpdater.updateStatus == AppUpdater.AppUpdateStatus.UPDATE_STARTED) {
                                if(count > 300) {
                                    Log.d(Tag,"app install time out");
                                    installationFinished = false;
                                    break;
                                }
                                try{
                                    count++;
                                    Log.d(Tag,"status "+ AppUpdater.updateStatus);
                                    Thread.sleep(100);

                                }
                                catch (InterruptedException e) {
                                    e.printStackTrace();
                                }
                            }
                            if(installationFinished) {
                                Log.d(Tag, "app installed ");
                                params.add(new AbstractMap.SimpleEntry<String, String>("Status", "0"));
                                params.add(new AbstractMap.SimpleEntry<String, String>("Name", appName));
                            }
                            else{
                                Log.d(Tag, "app installation time out ");
                                params.add(new AbstractMap.SimpleEntry<String, String>("Status", "1"));
                                params.add(new AbstractMap.SimpleEntry<String, String>("Name", appName));
                            }
                        } else {
                            params.add(new AbstractMap.SimpleEntry<String, String>("Status", "1"));
                            params.add(new AbstractMap.SimpleEntry<String, String>("Name", appName));
                            Log.d(Tag, "app downlaod failed");
                        }
                    }
                    else{
                        params.add(new AbstractMap.SimpleEntry<String, String>("Status", "0"));
                        params.add(new AbstractMap.SimpleEntry<String, String>("Name", appName));
                        Log.d(Tag, "app is already installed");
                    }
                }
            }
        }
        catch(Exception e){
            e.printStackTrace();
            Log.d(Tag,"ftp connection error at app update");
            for(String appName : appNames){
                params.add(new AbstractMap.SimpleEntry<>("Name", appName));
                params.add(new AbstractMap.SimpleEntry<>("Status", "1"));
            }
        }
        ftpClient.disConnectFtp();
        mCommunication.submitCommandResults(params);
    }

    private void submitTestHealth(){
        List<Map.Entry<String,String>> params= new ArrayList<Map.Entry<String, String>>();
        params.add(new AbstractMap.SimpleEntry<>("Command", "TestHealth"));
        params.add(new AbstractMap.SimpleEntry<>("Crypto", "0"));
        params.add(new AbstractMap.SimpleEntry<>("Printer", "0"));
        params.add(new AbstractMap.SimpleEntry<>("Timer", "0"));
        params.add(new AbstractMap.SimpleEntry<>("Buzzer", "0"));
        params.add(new AbstractMap.SimpleEntry<>("Led", "0"));
        params.add(new AbstractMap.SimpleEntry<>("Rtc", "0"));
        params.add(new AbstractMap.SimpleEntry<>("Memory", "0"));
        params.add(new AbstractMap.SimpleEntry<>("UsedDiskSize", String.valueOf(SystemUtil.getTotalDiskSpace(this) - SystemUtil.getFreeDiskSpace(this))));
        params.add(new AbstractMap.SimpleEntry<>("UsedRamSize", String.valueOf(SystemUtil.getTotalRamSize() - SystemUtil.getFreeRamSize())));
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
