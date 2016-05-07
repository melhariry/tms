package com.elgp2.verifonetms.ui;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.AuthFailureError;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import com.elgp2.verifonetms.Execute;
import com.elgp2.verifonetms.R;
import com.elgp2.verifonetms.models.FileInfo;
import com.elgp2.verifonetms.volley.Communication;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class MainActivity extends AppCompatActivity {

    /*Button to list all installed apps*/
    private Button listAppsButton;
    /*Tag for debugging*/
    private final String Tag = "MainActivity";
    private Execute execute;

    private Communication comm;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        execute =new Execute();
        comm = new Communication();
        listAppsButton= (Button) findViewById(R.id.list_apps_button);
        listAppsButtonHandler();
    }

    /*when Click the Button show a list of installed apps*/
    private void listAppsButtonHandler(){
        listAppsButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                comm.createPosRecord();
                /*
                //String url ="http://192.168.1.6/MTMS/PosUp.ashx";
                String url = String.format("http://41.47.139.203/MTMS/PosUp.ashx?SerialNumber=%1$s",
                        1245);

                StringRequest stringRequest = new StringRequest(Request.Method.GET, url,
                        new Response.Listener<String>() {
                            @Override
                            public void onResponse(String response) {
                                httpResult.setText("Response is: "+ response);
                            }
                        }, new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        httpResult.setText(error.toString());
                    }
                }){
                    @Override
                    public Map<String, String> getHeaders() throws AuthFailureError {
                        Map<String, String>  params = new HashMap<String, String>();
                        params.put("SerialNumber", "1245");

                        return params;
                    }
                };
                // Add the request to the RequestQueue.
                queue.add(stringRequest);*/
            }
        });
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
