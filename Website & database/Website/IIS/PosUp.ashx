<%@ WebHandler Language="C#" Class="PosUp" %>

using System;
using System.Web;
using System.Data;
using System.Data.SqlClient;
using System.Configuration;

public class PosUp : IHttpHandler {

    protected string getUserIP()
    {
        string VisitorsIPAddr = string.Empty;
        if (HttpContext.Current.Request.ServerVariables["HTTP_X_FORWARDED_FOR"] != null)
        {
            VisitorsIPAddr = HttpContext.Current.Request.ServerVariables["HTTP_X_FORWARDED_FOR"].ToString();
        }
        else if (HttpContext.Current.Request.UserHostAddress.Length != 0)
        {
            VisitorsIPAddr = HttpContext.Current.Request.UserHostAddress;
        }
        return VisitorsIPAddr;
    }
    
    public void ProcessRequest (HttpContext context) {
        context.Response.ContentType = "text/plain";
        try
        {
            DB db = new DB();
            if (db.PosExists(context.Request.Headers["SerialNumber"]))
            {
                context.Response.StatusCode = 200;
                //send file
                context.Response.Write("Success");
            }
            else
            {
                //Model is set to empty and disk size, memory size is set to zero mo2aqattan
                int PosId = -1;
                if (db.CreatePosRecord(
                    context.Request.Headers["SerialNumber"],
                    context.Request.Params["Vendor"],
                    "",
                    getUserIP(),
                    0,0,
                    out PosId
                    ))
                {
                    context.Response.StatusCode = 200;
                    context.Response.Write("Welcome to the family!");
                }
                else
                {
                    context.Response.StatusCode = 500;
                    context.Response.StatusDescription = "An error occurred while creating record"; 
                }
            }
        }
        catch (Exception EX)
        {
            context.Response.StatusCode = 500;
            context.Response.StatusDescription = EX.Message;
        }
    }
 
    public bool IsReusable {
        get {
            return false;
        }
    }

}