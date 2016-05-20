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
    
    
    public void ProcessRequest (HttpContext context) {
        context.Response.ContentType = "text/plain";        
        try
        {
            DB db = new DB();
            int PosId = -1;
            if(db.CreatePosRecord(
                context.Request.Headers["SerialNumber"],
                context.Request.Params["Vendor"],
                "Vega3000",
                Methods.GetUserIP(context),
                Int64.Parse(context.Request.Params["TotalDiskCapacity"]),
                Int64.Parse(context.Request.Params["TotalRamSize"]),
                out PosId
                ))
            {
                Methods.CreateFtpDirectory(PosId.ToString());
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