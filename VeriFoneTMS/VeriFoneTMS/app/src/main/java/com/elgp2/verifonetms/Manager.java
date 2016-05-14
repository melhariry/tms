package com.elgp2.verifonetms;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.util.Log;

import com.elgp2.verifonetms.communication.Communication;
import com.elgp2.verifonetms.communication.MyVolley;
import com.elgp2.verifonetms.models.AppInfo;
import com.elgp2.verifonetms.utilities.SystemUtil;

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
    @Override
    public void onCreate() {
        MyVolley.init(this);
        mCommunication = new Communication();
        mExecute = new Execute(this);
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

    public void posUp(){
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
                    //list files command
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
                    String [] fileNames = requestCommandParameters("Put");
                    if(fileNames != null){

                    }
                }
                if(b.get(5)){
                    //pull file command
                    //TODO add this paramter to database
                    String [] fileNames = requestCommandParameters("PullFile");
                    if(fileNames != null){

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
                    mCommunication.createPosRecord();
                    waitForResponse();
                }
                //finish commands execution
                submitFinishCommand();
                waitForResponse();
                //stop the service TODO reschedule it
                this.stopSelf();
            }
            else{
                Log.d(Tag,"response not ok");
                //TODO add error handler to terminate the service and reschedule it
            }
        }
        catch (Exception e){
            e.printStackTrace();
            Log.d(Tag , "response parsing failed");
        }

    }

    public String [] requestCommandParameters(String command){
        mCommunication.requestCommandParameters(command);
        String response = waitForResponse();
        String [] params = null;
        if(response != "-1"){
            try{
             params = response.split(";");
            }
            catch(Exception e){
                e.printStackTrace();
                Log.d(Tag, "response for command paramters parsing failed");
            }
        }
        return params;
    }

    public void submitFinishCommand(){
        Map<String,String> params= new HashMap<String, String>();
        params.put("Command", "Finish");
        mCommunication.submitCommandResults(params);
    }

    public void submitAppList(){
        Map<String,String> params = new HashMap<String, String>();
        params.put("Command","ListApps");
        List<AppInfo> appList =mExecute.getInstalledApps();
        for(AppInfo appInfo: appList){
            params.put("Name",appInfo.getAppLabel());
            params.put("Version",appInfo.getVersionName());
            params.put("Com",appInfo.getCompanyName());
        }
        mCommunication.submitCommandResults(params);
    }

    public void submitDeleteFiles(String [] paths){
        Map<String , String> params = new HashMap<String, String>();
        params.put("Command","DeleteFile");
        List<Boolean> deleteResults = mExecute.deleteFiles(Arrays.asList(paths));
        for(int i = 0 ; i< paths.length ; i++){
            params.put("Name",paths[i]);
            if(deleteResults.get(i))
                params.put("Status","0");
            else
                params.put("Status","1");
        }
        mCommunication.submitCommandResults(params);
    }

    public void submitTestHealth(){
        Map<String,String> params = new HashMap<String,String>();
        params.put("Command","TestHealth");
        params.put("Crypto","0");
        params.put("Printer","0");
        params.put("Timer","0");
        params.put("Buzzer","0");
        params.put("Led","0");
        params.put("Rtc","0");
        params.put("Memory","0");
        params.put("UsedDiskSize", String.valueOf(SystemUtil.getTotalDiskSpace() - SystemUtil.getFreeDiskSpace()));
        params.put("UsedRamSize",String.valueOf(SystemUtil.getTotalRamSize() - SystemUtil.getFreeRamSize()));
        mCommunication.submitCommandResults(params);
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
