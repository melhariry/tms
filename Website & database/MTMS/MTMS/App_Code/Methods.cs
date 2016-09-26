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

    public static void CreateFtpDirectory(string id)
    {
        WebRequest request = WebRequest.Create("ftp://localhost/MTMS_FTP/" + id + "/");
        request.Method = WebRequestMethods.Ftp.MakeDirectory;
        request.Credentials = new NetworkCredential("anonymous", "ibrahim@mail.com");
        FtpWebResponse response = (FtpWebResponse)request.GetResponse();
        response.Close();
    }
}