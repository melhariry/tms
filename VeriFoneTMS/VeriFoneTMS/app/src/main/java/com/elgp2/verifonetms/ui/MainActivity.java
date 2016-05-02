package com.elgp2.verifonetms.ui;

import android.content.pm.PackageInfo;
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
import android.widget.Toast;

import com.elgp2.verifonetms.Execute;
import com.elgp2.verifonetms.R;
import com.elgp2.verifonetms.models.AppInfo;
import com.elgp2.verifonetms.models.FileInfo;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    /*Button to list all installed apps*/
    private Button listAppsButton;
    /*Tag for debugging*/
    private final String Tag = "MainActivity";
    private Execute execute;
    /*ListView to show a list of installed apps*/
    private ListView appsList;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        execute =new Execute(getApplicationContext());

        appsList =(ListView) findViewById(R.id.apps_list);

        listAppsButton= (Button) findViewById(R.id.list_apps_button);
        listAppsButtonHandler();
    }

    /*when Click the Button show a list of installed apps*/
    private void listAppsButtonHandler(){
        listAppsButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(execute.createPublicFile("myFolder1/myFolder2","myFile3.txt")) {
                    Log.d(Tag, "myFile1 successfully created in public storage");
                }
                else {
                    //make sure to disable USB storage to be able to write to external memory
                    Log.d(Tag, "myFile1 creation failed :(");
                }
                if(execute.createPrivateFile("priv1/priv2", "elGp2.txt"))
                    Log.d(Tag,"elGp2 was created  ");
                else
                    Log.d(Tag,"elGp2 creation failed :( ");

                //list private files
                List<FileInfo> pubFileList= execute.getPrivateFiles();
                List<String> filesData=new ArrayList<String>();
                for(FileInfo f:pubFileList){
                    String temp="";
                    temp+=("file_path: "+f.getFileAbsolutePath()+"\n");
                    temp+=("file_size: "+f.getFileSize());

                    filesData.add(temp);
                }
                ArrayAdapter<String> adapter = new ArrayAdapter<String>(getApplication(),android.R.layout.simple_list_item_1,android.R.id.text1,filesData);
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
