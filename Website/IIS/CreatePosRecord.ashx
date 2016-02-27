<%@ WebHandler Language="C#" Class="CreatePosRecord" %>

using System;
using System.Web;
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
            if(db.CreatePosRecord(
                context.Request.Params["SerialNumber"],
                context.Request.Params["Vendor"],
                context.Request.Params["Model"],
                Decimal.Parse(context.Request.Params["LocationLat"]),
                Decimal.Parse(context.Request.Params["LocationLng"]),
                Int64.Parse(context.Request.Params["TotalDiskCapacity"]),
                Int64.Parse(context.Request.Params["TotalRamSize"])
                ))
            {
                context.Response.StatusCode = 200;
                //send file
                context.Response.Write("StartHealthTest");
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