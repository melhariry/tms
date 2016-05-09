package com.elgp2.verifonetms.ui;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Button;

import com.elgp2.verifonetms.Execute;
import com.elgp2.verifonetms.Manager;
import com.elgp2.verifonetms.R;
import com.elgp2.verifonetms.communication.Communication;
import com.elgp2.verifonetms.communication.FtpClient;

import java.util.BitSet;

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

  //      execute =new Execute();
//        comm = new Communication();
        listAppsButton= (Button) findViewById(R.id.list_apps_button);
        listAppsButtonHandler();
    }

    /*when Click the Button show a list of installed apps*/
    private void listAppsButtonHandler(){
        listAppsButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                 Intent managerIntent= new Intent(getApplicationContext(), Manager.class);
                startService(managerIntent);
                finish();
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
