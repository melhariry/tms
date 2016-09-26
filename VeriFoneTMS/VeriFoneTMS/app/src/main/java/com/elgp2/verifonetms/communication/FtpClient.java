package com.elgp2.verifonetms.communication;

import android.content.Context;
import android.os.Environment;
import android.util.Log;

import com.elgp2.verifonetms.TMSApplication;
import com.elgp2.verifonetms.models.CommInfo;
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

    public FtpClient(CommInfo commInfo){
        userName  = commInfo.getFTPUserName();
        passWord  = commInfo.getFTPPassword();
        ftpClient = new FTPClient();
        publicIP  = commInfo.getFirstServerIP();
        status    = ftpStatus.FTPUNINITIALIZED;
    }

    public ftpStatus getStatus() {
        return status;
    }

    public void downloadFile(final String filePath,final Context context) {
        Log.d(Tag,"downlaod starts");

        try {

            status = ftpStatus.FTPSTARTED;

            if (ftpClient.isAuthenticated()) {
                Log.d(Tag, "FTPClient Authenticated");
                Log.d(Tag,"ftp current dir before moving to root: "+ ftpClient.currentDirectory());
                //go to ftp root
                ftpClient.changeDirectory("/");
                Log.d(Tag, "ftp current dir after moving to root: " + ftpClient.currentDirectory());
                if (filePath.contains("pub/")){
                    ftpClient.changeDirectory("/Terminals/VeriFone/" + SystemUtil.getMachineSerial() + "/pub/MTMS/");
                }
                else if(filePath.contains("pri/")){
                    ftpClient.changeDirectory("/Terminals/VeriFone/" + SystemUtil.getMachineSerial() + "/pri/");
                }

                File f = null;

                if(FileUtil.isPub(filePath) || filePath.equals("hotList.txt") || filePath.equals("TransactionConfig.txt")) {
                    FileUtil.createPublicFile("MTMS", FileUtil.getFileName(filePath));
                    f = new File(Environment.getExternalStorageDirectory().getAbsolutePath()+"/MTMS/"+FileUtil.getFileName(filePath));
                }
                else {
                    FileUtil.createPrivateFile("", FileUtil.getFileName(filePath), context);
                    f= new File(context.getFilesDir().getAbsolutePath() + "/"+FileUtil.getFileName(filePath));
                }

                if(f.exists() && f != null) {
                    Log.d(Tag,"file exist ftp download starts");
                    ftpClient.download(FileUtil.getFileName(filePath), f, new MyFTPListener());
                }
                else{
                    Log.d(Tag,"file doesn't exist ftp download failed");
                    status= ftpStatus.FTPERROR;
                }
            } else {
                Log.e(Tag, "Authentication to FtpServer failed");
                status = ftpStatus.FTPERROR;
            }

        }
        catch (Exception e){
            e.printStackTrace();
            status = ftpStatus.FTPERROR;
        }
    }


    public void downloadAppFile(final String fileName,final Context context) {
        Log.d(Tag,"downlaod app starts");

        try {
            status = ftpStatus.FTPSTARTED;

            if (ftpClient.isAuthenticated()) {
                Log.d(Tag, "FTPClient Authenticated");

                ftpClient.changeDirectory("/");
                ftpClient.changeDirectory("/Apps/VeriFone/");

                FileUtil.createPublicFile("MTMS", fileName);

                File f = new File(Environment.getExternalStorageDirectory().getAbsolutePath()+"/MTMS/"+fileName);

                if(f.exists())
                    ftpClient.download(fileName, f,new MyFTPListener());
                else{
                    Log.d(Tag,"file not found at FTP download app");
                    status= ftpStatus.FTPERROR;
                }
            } else {
                status = ftpStatus.FTPERROR;
                Log.e(Tag, "Authentication to FtpServer failed");
            }
        }
        catch (Exception e){
            e.printStackTrace();
            status = ftpStatus.FTPERROR;
        }
    }

    public void uploadFile(final String filePath, final Context context){

        try {
            Log.d(Tag,"upload routine started");
            status = ftpStatus.FTPSTARTED;

            if (ftpClient.isAuthenticated()) {
                Log.d(Tag, "FTPClient Authenticated");
                ftpClient.changeDirectory("/");

                File f;
                if(FileUtil.isPub(filePath) ) {
                    ftpClient.changeDirectory("/Terminals/VeriFone/" + SystemUtil.getMachineSerial() + "/pub/MTMS/");
                    FileUtil.createPublicFile("MTMS", FileUtil.getFileName(filePath));
                    f = new File(Environment.getExternalStorageDirectory().getAbsolutePath()+"/MTMS/"+FileUtil.getFileName(filePath));
                }
                else {
                    ftpClient.changeDirectory("/Terminals/VeriFone/" + SystemUtil.getMachineSerial() + "/pri/");
                    FileUtil.createPrivateFile("", FileUtil.getFileName(filePath), context);
                    f= new File(context.getFilesDir().getAbsolutePath() + "/"+FileUtil.getFileName(filePath));
                }

                if(f.exists()) {
                    ftpClient.upload(f, new MyFTPListener());
                }
                else{
                    Log.d(Tag,"error at uplad file doesn't exist");
                    status = ftpStatus.FTPERROR;
                }
            } else {
                status = ftpStatus.FTPERROR;
                Log.e(Tag, "Authentication to FtpServer failed");
            }

        }
        catch (Exception e){
            e.printStackTrace();
            status = ftpStatus.FTPERROR;
        }

    }

    public Boolean connectFTP() throws FTPException, IOException, FTPIllegalReplyException {
        ftpClient.connect(publicIP);
        if(ftpClient.isConnected()){
            ftpClient.login(userName, passWord);
            return true;
        }
        else
            return false;
    }

    public void disConnectFtp(){
        if(ftpClient.isConnected()){
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
        }

        public void aborted()
        {
            // Transfer aborted
            Log.d(Tag,"TRANSFER-STATUS: File transfer aborted...");
            status = ftpStatus.FTPERROR;
        }

        public void failed()
        {
            // Transfer failed
            Log.d(Tag,"TRANSFER-STATUS: File transfer failed...");
            status  = ftpStatus.FTPERROR;
        }
    }
}
