package com.elgp2.verifonetms;

import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    /*Button to list all installed apps*/
    private Button listAppsButton;
    private Execute execute;
    /*ListView to show a list of installed apps*/
    private ListView appsList;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        execute =new Execute();

        appsList =(ListView) findViewById(R.id.apps_list);

        listAppsButton= (Button) findViewById(R.id.list_apps_button);
        listAppsButtonHandler();
    }

    /*when Click the Button show a list of installed apps*/
    private void listAppsButtonHandler(){
        listAppsButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                List<PackageInfo> apps;
                apps= execute.getInstalledApps(getApplicationContext());
                //add apps info to listView
                ArrayList<String> apps_info=new ArrayList<String>();
                for(PackageInfo appInfo : apps){
                    String temp="";
                    temp+=("app_name:"+appInfo.packageName+"\n");
                    temp+=("version_name:"+appInfo.versionName+"\n");

                    apps_info.add(temp);
                }
                ArrayAdapter<String> adapter = new ArrayAdapter<String>(getApplicationContext(),android.R.layout.simple_list_item_1,apps_info);
                appsList.setAdapter(adapter);
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
