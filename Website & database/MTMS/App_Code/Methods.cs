﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Net;

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
}