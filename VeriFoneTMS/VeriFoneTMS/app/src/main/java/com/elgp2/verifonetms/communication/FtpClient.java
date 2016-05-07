package com.elgp2.verifonetms.communication;

import android.util.Log;

import com.elgp2.verifonetms.TMSApplication;
import com.elgp2.verifonetms.utilities.FileUtil;

import java.io.File;
import java.io.IOException;

import it.sauronsoftware.ftp4j.FTPAbortedException;
import it.sauronsoftware.ftp4j.FTPClient;
import it.sauronsoftware.ftp4j.FTPDataTransferException;
import it.sauronsoftware.ftp4j.FTPException;
import it.sauronsoftware.ftp4j.FTPIllegalReplyException;

/**
 * FtpClient to download and upload files to FTPServer
 * Created by POS_GP_TEAM_2016 on 5/7/2016.
 */
public class FtpClient {

    /**
     * Tag for Debugging
     */
    protected final String Tag = "FtpClient";
    protected final String userName ;
    protected final String passWord ;
    protected String publicIP ;
    /**
     * instance of ftp4j library's FTPClient
     */
    protected FTPClient ftpClient;

    public FtpClient(){
        userName = "MTMS_USER";
        passWord = "1234";
        ftpClient = new FTPClient();
        publicIP = "41.47.139.128";
    }

    public void downloadFile(String filePath) throws FTPException, IOException, FTPIllegalReplyException {
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    ftpClient.connect(publicIP);
                    if (ftpClient.isConnected()) {
                        Log.d(Tag, "FTPClient connected");
                        ftpClient.login(userName, passWord);
                        if (ftpClient.isAuthenticated()) {
                            Log.d(Tag, "FTPClient Authenticated");
                            FileUtil.createPrivateFile("priv1", "download.txt", TMSApplication.getInstance());
                            File f = new File(TMSApplication.getInstance().getFilesDir().getAbsolutePath()+"/"+"priv1/download.txt");
                            if(!f.exists())
                                Log.e("ff","Errorrrr");
                            //ftpClient.download("Terminals/Verifone/system.txt", f);

                            ftpClient.upload(f);
                        } else
                            Log.e(Tag, "Authentation to FtpServer failed");
                    } else {
                        Log.e(Tag, "FTPClient failed to connect");
                    }
                }
                catch (Exception e){
                    e.printStackTrace();
                }
            }
        });
        thread.start();
    }

}
