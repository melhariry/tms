package com.elgp2.verifonetms;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.support.annotation.Nullable;
import android.util.Log;

import com.elgp2.verifonetms.communication.Communication;
import com.elgp2.verifonetms.communication.MyVolley;

import java.util.BitSet;

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
        mExecute = new Execute();
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
        Log.d(Tag, "Manager Service destroyed");
    }

    public void posUp(){
        mCommunication.posUp();
        Log.d(Tag,"before waiting");
        int response = waitForResponse();
        if(response != -1){
            Log.d(Tag,"response ok:"+response);
            BitSet b = BitSet.valueOf(new long[]{response});

            if(b.get(0)){
                // test POS command
            }
            if(b.get(1)){
                //list files command
            }
            if(b.get(2)){
                //list apps command
            }
            if(b.get(3)){
                //delete files command
            }
            if(b.get(4)){
                //push file command
            }
            if(b.get(5)){
                //pull file command
            }
            if(b.get(6)){
                //update apps command
            }
            if(b.get(7)){
                // create POS record command
                Log.d(Tag,"create POS record");
                mCommunication.createPosRecord();
                waitForResponse();
            }
            else{
                Log.d(Tag,"errrror");
            }
           this.stopSelf();
        }
        else{
            Log.d(Tag,"response not ok");
            //TODO add error handler to terminate the service and reschedule it
        }
    }

    /**
     * helper method to block thread till response/error is sent back
     * @return response
     */
    private int waitForResponse(){
        int count =0;
        int response = -1;
        while(mCommunication.getStatus() != Communication.commStatus.ERROR && mCommunication.getStatus() != Communication.commStatus.RESPONSE){
            Log.d(Tag,"response at loop: "+ mCommunication.getStatus().toString());
            if(count > 30) {
                Log.d(Tag,"wait loop :( ");
                return response;
            }
            try {
                count++;
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        Log.d(Tag,"after loop");
        if(mCommunication.getStatus() == Communication.commStatus.RESPONSE){
            Log.d(Tag,"response delivered");
             response =  Integer.valueOf(mCommunication.getResponse());
        }
        else{
            Log.d(Tag,"transmission error");
            Log.e(Tag, "Volley error: " + mCommunication.getError());
            return response;
        }
        return response;
    }
}
