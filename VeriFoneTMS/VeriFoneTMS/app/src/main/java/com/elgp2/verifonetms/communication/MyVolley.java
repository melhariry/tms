package com.elgp2.verifonetms.communication;

import android.content.Context;

import com.android.volley.RequestQueue;
import com.android.volley.toolbox.Volley;

/**
 * Helper Class to provide one centralized Volley's RequestQueue
 * Created by POS_GP_TEAM_2016 on 5/7/2016.
 */
public class MyVolley {

    private static RequestQueue requestQueue;
    private MyVolley(){}

    public static void init(Context c){
        if(requestQueue == null)
            requestQueue = Volley.newRequestQueue(c);
    }

    public static RequestQueue getRequestQueue(){
        if(requestQueue != null)
            return requestQueue;
        else
            throw new IllegalStateException("requestQueue was not initialized");
    }
}
