package com.elgp2.verifonetms.communication;

import android.util.Log;

import com.elgp2.verifonetms.TMSApplication;
import com.elgp2.verifonetms.utilities.FileUtil;
import com.elgp2.verifonetms.utilities.SystemUtil;

import java.io.File;
import java.io.IOException;

import it.sauronsoftware.ftp4j.FTPAbortedException;
import it.sauronsoftware.ftp4j.FTPClient;
import it.sauronsoftware.ftp4j.FTPDataTransferException;
import it.sauronsoftware.ftp4j.FTPDataTransferListener;
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
    private final String Tag = "FtpClient";
    private final String userName ;
    private final String passWord ;
    private String publicIP ;
    /**
     * instance of ftp4j library's FTPClient
     */
    protected FTPClient ftpClient;

    public FtpClient(){
        userName = Constants.FTPUserName;
        passWord = Constants.FTPPassWord;
        ftpClient = new FTPClient();
        publicIP = Constants.publicIP;
    }

    public void downloadFile(final String filePath , final String fileName) throws FTPException, IOException, FTPIllegalReplyException {
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
                            ftpClient.changeDirectory("/Terminals/Verifone/"+ SystemUtil.getMachineSerial()+"/");
                            FileUtil.createPrivateFile(filePath, fileName, TMSApplication.getInstance());
                            File f = new File(TMSApplication.getInstance().getFilesDir().getAbsolutePath()+"/"+filePath+"/"+fileName);

                            ftpClient.download(fileName, f,new MyFTPListener());
                            Log.d(Tag, "download finished");
                        } else
                            Log.e(Tag, "Authentication to FtpServer failed");
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

    public void uploadFile(final String filePath, final String fileName){
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
                            ftpClient.changeDirectory("/Terminals/Verifone/"+ SystemUtil.getMachineSerial()+"/");
                            File f = new File(TMSApplication.getInstance().getFilesDir().getAbsolutePath()+"/"+filePath+"/"+fileName);
                            if(f.exists())
                                ftpClient.upload(f,new MyFTPListener());
                            else{
                                ftpClient.disconnect(true);
                            }
                        } else
                            Log.e(Tag, "Authentication to FtpServer failed");
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
    public class MyFTPListener implements FTPDataTransferListener
    {

        public void started()
        {
            // Transfer started
            Log.d(Tag,"TRANSFER-STATUS: File transfer started...");
        }

        public void transferred(int length)
        {
            // Yet other length bytes has been transferred since the last time this
            // method was called
        }

        public void completed()
        {
            // Transfer completed
            Log.d(Tag,"TRANSFER-STATUS: File transfer completed...");
            try {
                ftpClient.disconnect(true);
            } catch (IOException e) {
                e.printStackTrace();
            } catch (FTPIllegalReplyException e) {
                e.printStackTrace();
            } catch (FTPException e) {
                e.printStackTrace();
            }
        }

        public void aborted()
        {
            // Transfer aborted
            Log.d(Tag,"TRANSFER-STATUS: File transfer aborted...");
            try {
                ftpClient.disconnect(true);
            } catch (IOException e) {
                e.printStackTrace();
            } catch (FTPIllegalReplyException e) {
                e.printStackTrace();
            } catch (FTPException e) {
                e.printStackTrace();
            }
        }

        public void failed()
        {
            // Transfer failed
            Log.d(Tag,"TRANSFER-STATUS: File transfer failed...");
            try {
                ftpClient.disconnect(true);
            } catch (IOException e) {
                e.printStackTrace();
            } catch (FTPIllegalReplyException e) {
                e.printStackTrace();
            } catch (FTPException e) {
                e.printStackTrace();
            }
        }
    }
}
