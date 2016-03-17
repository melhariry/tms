<%@ WebHandler Language="C#" Class="CreatePosRecord" %>

using System;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.IO;
using System.Data;
using System.Data.SqlClient;
using System.Configuration;

public class CreatePosRecord : IHttpHandler
{
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
            int PosId = -1;
            if(db.CreatePosRecord(
                context.Request.Headers["SerialNumber"],
                context.Request.Params["Vendor"],
                " ",
                getUserIP(),
                Int64.Parse(context.Request.Params["TotalDiskCapacity"]),
                Int64.Parse(context.Request.Params["TotalRamSize"]),
                out PosId
                ))
            {
                context.Response.StatusCode = 200;
                context.Response.Write("Success");
            }
            else
            {
                context.Response.StatusCode = 500;
                context.Response.StatusDescription = "An error occurred while creating record";
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