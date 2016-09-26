package com.elgp2.verifonetms.communication;

import android.content.Context;
import android.util.Log;

import com.android.volley.AuthFailureError;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.elgp2.verifonetms.models.CommInfo;
import com.elgp2.verifonetms.utilities.SystemUtil;

import java.util.AbstractMap;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * communication class to send and receive data to server
 * Created by POS_GP_TEAM_2016 on 5/7/2016.
 */
public class Communication {

    /**
     * Tag to attach to requests
     */
    private static final String Tag = "Comm";
    /**
     * queue to hold all request going out
     */
    private RequestQueue requestQueue;
    /**
     * Status of communication
     */
    public enum commStatus {
        UNINITIALIZED,
        STARTED,
        ERROR,
        RESPONSE
    }

    /**
     * holds the current communication status
     */
    protected commStatus status;
    /**
     * holds the response value
     */
    protected String response;
    /**
     * holds error description
     */
    protected String error;

    private String publicIP ;
    /**
     * holds parameters inside request
     */
    private List<Map.Entry<String,String>> params;

    public Communication(CommInfo commInfo){
        requestQueue = MyVolley.getRequestQueue();
        status = commStatus.UNINITIALIZED;
        publicIP = commInfo.getFirstServerIP();
        params= new ArrayList<Map.Entry<String,String>>();
    }

    public String getResponse() {
        return response;
    }

    public String getError() {
        return error;
    }

    public static String getTag() {
        return Tag;
    }

    public commStatus getStatus() {
        return status;
    }

    public void posUp(){
        params.clear();
        String uri = buildUri("PosUp.ashx");
        sendRequest(uri);
    }

    public void createPosRecord(Context context){
        params.clear();
        params.add(new  AbstractMap.SimpleEntry("Vendor", "VeriFone"));
        params.add(new AbstractMap.SimpleEntry<String, String>("Model", SystemUtil.getMachineModel()));
        params.add(new AbstractMap.SimpleEntry<String, String>("TotalDiskCapacity", String.valueOf(SystemUtil.getTotalDiskSpace(context))));
        params.add(new AbstractMap.SimpleEntry<String, String>("TotalRamSize", String.valueOf(SystemUtil.getTotalRamSize())));
        String uri = buildUri("CreatePosRecord.ashx");
        sendRequest(uri);
    }

    public void requestCommandParameters(String commandName){
        params.clear();
        params.add(new AbstractMap.SimpleEntry<String, String>("Command", commandName));
        String uri =buildUri("RequestCommandParameters.ashx");
        sendRequest(uri);
    }

    public void submitCommandResults( List<Map.Entry<String , String>> map){
        params.clear();
        params.addAll(map);
        String uri = buildUri("SubmitCommandResult.ashx");
        sendRequest(uri);
    }

    /**
     * helper method to sendRequests to server
     * @param URI: request to be sent to server
     */
    private void sendRequest(final String URI){

        StringRequest stringRequest = new StringRequest(Request.Method.POST,URI,
                new Response.Listener<String>() {
                    @Override
                    public void onResponse(String res) {
                        status = commStatus.RESPONSE;
                        Log.d(Tag,"status:"+ status.toString());
                        Log.d(Tag,res);
                        response = res;
                    }
                }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError err) {
                status = commStatus.ERROR;
                Log.d(Tag,err.toString());
                error = err.toString();
            }
        }){
            @Override
            public Map<String, String> getHeaders() throws AuthFailureError {
                Map<String, String>  headers = new HashMap<String, String>();
                headers.put("SerialNumber", SystemUtil.getMachineSerial());
                return headers;
            }
        };
        status = commStatus.STARTED;
        //attach Tag to request
        stringRequest.setTag(Tag);
        Log.d(Tag, "request started");
        requestQueue.add(stringRequest);
    }

    /**
     * helper method to build request
     * @param pageName
     * @return String contains request Uri
     */
    private String buildUri(String pageName ){
        String uri = "https://" + publicIP +"/MTMS/" + pageName;
        if(params != null &&params .size() > 0 ) {
            uri += "?";
            for(Map.Entry<String, String> entry : params){
                uri += ("&"+ entry.getKey()+"="+entry.getValue());
            }
        }
        Log.d(Tag,uri.replaceAll("\\s",""));
        return uri.replaceAll("\\s+", "");
    }
}
