package com.elgp2.verifonetms;

import android.app.Application;
import android.content.Context;

import com.elgp2.verifonetms.communication.MyVolley;

/**
 * base application class to insure MyVolley is initialized and share ApplicationContext globally
 * Created by POS_GP_TEAM_2016 on 5/7/2016.
 */
public class TMSApplication extends Application {

    private static TMSApplication tmsApplication;

    /**
     * share Application Context globally
     * @return Application Context
     */
    public static Context getInstance(){
        return tmsApplication;
    }
    @Override
    public void onCreate(){
        super.onCreate();
        if(tmsApplication == null)
            tmsApplication =this;
       // MyVolley.init(this);
    }
}
