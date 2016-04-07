using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Net;

/// <summary>
/// Summary description for Methods
/// </summary>
public static class Methods
{
    public static string GetUserIP(HttpContext context)
    {
        string VisitorsIPAddr = string.Empty;

        if (context.Request.ServerVariables["HTTP_X_FORWARDED_FOR"] != null)
            VisitorsIPAddr = context.Request.ServerVariables["HTTP_X_FORWARDED_FOR"].ToString();

        else if (context.Request.UserHostAddress.Length != 0)
            VisitorsIPAddr = context.Request.UserHostAddress;

        return VisitorsIPAddr;
    }

    public static void CreateFtpDirectory(string vendor, string serialNumber)
    {
        WebRequest request = WebRequest.Create("ftp://localhost/MTMS_FTP/Terminals/" + vendor + "/" + serialNumber + "/");
        request.Method = WebRequestMethods.Ftp.MakeDirectory;
        request.Credentials = new NetworkCredential("MTMS_FTP", "1234");
        FtpWebResponse response = (FtpWebResponse)request.GetResponse();
        response.Close();
    }
}