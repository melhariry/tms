package com.elgp2.verifonetms.communication;

import android.content.Context;
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

    public  enum ftpStatus {
        FTPUNINITIALIZED,
        FTPSTARTED,
        FTPERROR,
        FTPTRANSFERING,
        FTPCOMPLETED
    }

    private ftpStatus status;
    /**
     * instance of ftp4j library's FTPClient
     */
    protected FTPClient ftpClient;

    public FtpClient(){
        userName = Constants.FTPUserName;
        passWord = Constants.FTPPassWord;
        ftpClient = new FTPClient();
        publicIP = Constants.publicIP;
        status = ftpStatus.FTPUNINITIALIZED;
    }

    public ftpStatus getStatus() {
        return status;
    }

    public void downloadFile(final String filePath,final Context context) {
        Log.d(Tag,"downlaod starts");
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    String fileRealtivePath;
                    ftpClient.connect(publicIP);
                    status = ftpStatus.FTPSTARTED;
                    if (ftpClient.isConnected()) {
                        Log.d(Tag, "FTPClient connected");
                       ftpClient.login(userName, passWord);
                        if (ftpClient.isAuthenticated()) {
                            Log.d(Tag, "FTPClient Authenticated");
                            ftpClient.changeDirectory("/Terminals/VeriFone/" + SystemUtil.getMachineSerial() + "/" + FileUtil.getFTPAbsolutePath(filePath) + "/");
                            if(FileUtil.isPub(filePath)) {
                                Log.d(Tag,"it is pub");
                                fileRealtivePath = filePath.replace("pub/","");
                                Log.d(Tag ,"ftp path "+fileRealtivePath );
                                FileUtil.createPublicFile(fileRealtivePath, FileUtil.getFileName(fileRealtivePath));
                            }
                            else {
                                Log.d(Tag,"it is pri");
                                fileRealtivePath = filePath.replace("pri/","");
                                FileUtil.createPrivateFile(fileRealtivePath, FileUtil.getFileName(fileRealtivePath), context);
                            }
                            Log.d(Tag,"file path :" + FileUtil.getAbsolutePath(filePath, context));
                            File f = new File(FileUtil.getAbsolutePath(filePath, context));
                            if(f.exists())
                                ftpClient.download(FileUtil.getFileName(fileRealtivePath), f,new MyFTPListener());
                            else{

                                status= ftpStatus.FTPERROR;
                                ftpClient.disconnect(true);
                            }
                        } else
                            Log.e(Tag, "Authentication to FtpServer failed");
                    } else {
                        Log.e(Tag, "FTPClient failed to connect");
                        status = ftpStatus.FTPERROR;
                    }
                }
                catch (Exception e){
                    e.printStackTrace();
                    status = ftpStatus.FTPERROR;
                }
            }
        });
        thread.start();
    }

    public void uploadFile(final String filePath, final Context context){
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    String fileRelativePath;
                    status = ftpStatus.FTPSTARTED;
                     ftpClient.connect(publicIP);

                    if (ftpClient.isConnected()) {
                        Log.d(Tag, "FTPClient connected");
                        ftpClient.login(userName, passWord);
                        if (ftpClient.isAuthenticated()) {
                            Log.d(Tag, "FTPClient Authenticated");

                            ftpClient.changeDirectory("/Terminals/VeriFone/" + SystemUtil.getMachineSerial() + "/" + FileUtil.getFTPAbsolutePath(filePath) + "/");
                            Log.d(Tag, "ftp on server path : " + "/Terminals/VeriFone/" + SystemUtil.getMachineSerial() + "/" + FileUtil.getFTPAbsolutePath(filePath) + "/");
                            Log.d(Tag,"filePath given : "+ filePath);

                            File f = new File(FileUtil.getAbsolutePath(filePath,context));
                            if(f.exists()) {
                                Log.d(Tag,"file existis");
                                ftpClient.upload(f, new MyFTPListener());
                            }
                            else{
                                Log.d(Tag,"error at uplad file doesn't exist");
                                status = ftpStatus.FTPERROR;
                                ftpClient.disconnect(true);
                            }
                        } else
                            Log.e(Tag, "Authentication to FtpServer failed");
                    } else {
                        Log.e(Tag, "FTPClient failed to connect");
                        status = ftpStatus.FTPERROR;
                    }
                }
                catch (Exception e){
                    e.printStackTrace();
                    status = ftpStatus.FTPERROR;
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
            status = ftpStatus.FTPSTARTED;
        }

        public void transferred(int length)
        {
            status = ftpStatus.FTPTRANSFERING;
        }

        public void completed()
        {
            // Transfer completed
            Log.d(Tag,"TRANSFER-STATUS: File transfer completed...");
            status = ftpStatus.FTPCOMPLETED;
            try {
                ftpClient.disconnect(true);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        public void aborted()
        {
            // Transfer aborted
            Log.d(Tag,"TRANSFER-STATUS: File transfer aborted...");
            status = ftpStatus.FTPERROR;
            try {
                ftpClient.disconnect(true);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        public void failed()
        {
            // Transfer failed
            Log.d(Tag,"TRANSFER-STATUS: File transfer failed...");
            status  = ftpStatus.FTPERROR;
            try {
                ftpClient.disconnect(true);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
}
