using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Net;
using System.Data;
using System.IO;

/// <summary>
/// Summary description for Methods
/// </summary>
public static class Methods
{
    private static string username = "MTMS_FTP";
    private static string password = "1234";
    public static string GetUserIP(HttpContext context)
    {
        string VisitorsIPAddr = string.Empty;

        if (context.Request.ServerVariables["HTTP_X_FORWARDED_FOR"] != null)
            VisitorsIPAddr = context.Request.ServerVariables["HTTP_X_FORWARDED_FOR"].ToString();

        else if (context.Request.UserHostAddress.Length != 0)
            VisitorsIPAddr = context.Request.UserHostAddress;

        return VisitorsIPAddr;
    }

    public static void CreateFtpDirectory(string serialNumber, string vendor)
    {
        WebRequest request = WebRequest.Create("ftp://localhost/Terminals/" + vendor + "/" + serialNumber + "/");
        request.Method = WebRequestMethods.Ftp.MakeDirectory;
        request.Credentials = new NetworkCredential(username, password);
        FtpWebResponse response = (FtpWebResponse)request.GetResponse();
        response.Close();
    }

    public static void CreateFtpInnerDirectory(string serialNumber, string vendor, string folderName)
    {
        WebRequest request = WebRequest.Create("ftp://localhost/Terminals/" + vendor + "/" + serialNumber + "/" + folderName + "/");
        request.Method = WebRequestMethods.Ftp.MakeDirectory;
        request.Credentials = new NetworkCredential(username, password);
        FtpWebResponse response = (FtpWebResponse)request.GetResponse();
        response.Close();
    }

    public static int getTerminalsCount() 
    {
        return Convert.ToInt32(DB.Instance.GetTerminalsCount()[0]);
    }

    public static int getGroupsCount()
    {
        return Convert.ToInt32(DB.Instance.GetGroupsCount()[0]);
    }

    public static bool UploadToFtp(string name, int contentLength, string path, Stream inputStream, bool configFlag = false)
    {
        DataRow terminalInfo = DB.Instance.GetTerminalInfo(Convert.ToInt32(HttpContext.Current.Request.Params["id"]));
        string ftpDirectory = "ftp://localhost/" + path;
        string fileName = name;
        byte[] buffer = new byte[4096];
        int bytesWritten = 0;
        int totalBytes = contentLength;
        try
        {
            FtpWebRequest request = (FtpWebRequest)WebRequest.Create(ftpDirectory + fileName);
            request.Method = WebRequestMethods.Ftp.UploadFile;
            request.Credentials = new NetworkCredential(username, password);
            request.ContentLength = contentLength;
            request.KeepAlive = false;
            request.UseBinary = true;

            using (Stream fileStream = inputStream)
            {
                using (Stream requestStream = request.GetRequestStream())
                {
                    while (totalBytes > 0)
                    {
                        if (configFlag)
                            fileStream.Seek(0, SeekOrigin.Begin);
                        bytesWritten = fileStream.Read(buffer, 0, buffer.Length);
                        requestStream.Write(buffer, 0, bytesWritten);
                        totalBytes -= bytesWritten;
                    }
                    requestStream.Close();
                }
                fileStream.Close();
            }
            FtpWebResponse response = (FtpWebResponse)request.GetResponse();
            response.Close();
            return true;
        }
        catch (WebException)
        {
            return false;
        }
    }

    public static bool DownloadFromFtp(string fileName, string localPath, string ftpPath)
    {
        int bytesRead = 0;
        byte[] buffer = new byte[4096];
        try
        {
            DataRow terminalInfo = DB.Instance.GetTerminalInfo(Convert.ToInt32(HttpContext.Current.Request.Params["id"]));
            string ftpDirectory = "ftp://localhost/" + ftpPath;
            FtpWebRequest request = (FtpWebRequest)WebRequest.Create(ftpDirectory + fileName);
            request.Method = WebRequestMethods.Ftp.DownloadFile;
            request.Credentials = new NetworkCredential(username, password);
            request.KeepAlive = false;
            request.UseBinary = true;

            using (FileStream fileStream = new FileStream(localPath, FileMode.Create))
            {
                using (Stream requestStream = request.GetResponse().GetResponseStream())
                {
                    do
                    {
                        bytesRead = requestStream.Read(buffer, 0, buffer.Length);
                        fileStream.Write(buffer, 0, bytesRead);

                    } while (bytesRead != 0);
                    requestStream.Close();
                }
                fileStream.Close();
            }
            FtpWebResponse response = (FtpWebResponse)request.GetResponse();
            response.Close();
            return true;
        }
        catch (WebException)
        {
            return false;
        }
    }
    public static string ReturnHTMLStatus(string message, string status, bool isList = true)
    {
        string rawHTML = string.Empty;
        rawHTML += "<div class=\"alert alert-" + status + "\">";
        rawHTML += "<button type=\"button\" class=\"close\" data-dismiss=\"alert\">×</button>";
        if (status.Equals("error"))
            rawHTML += "<strong>Error: </strong>";
        if (isList)
            rawHTML += "<ul>" + message + "</ul></div>";
        else
            rawHTML += message + "</div>";
        return rawHTML;
    }

}