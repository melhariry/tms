<%@ WebHandler Language="C#" Class="SubmitCommandResult" %>

using System;
using System.Web;

public class SubmitCommandResult : IHttpHandler {
    
    public void ProcessRequest (HttpContext context) {
        context.Response.ContentType = "text/plain";
        try
        {
            DB db = new DB();
            string command = context.Request.Params["Command"];
            switch(command)
            { 
                case "TestHealth":
                    if (db.SubmitHealthTestResults(
                    context.Request.Headers["SerialNumber"],
                    Int32.Parse(context.Request.Params["Crypto"]),
                    Int32.Parse(context.Request.Params["Printer"]),
                    Int32.Parse(context.Request.Params["Timer"]),
                    Int32.Parse(context.Request.Params["Buzzer"]),
                    Int32.Parse(context.Request.Params["Led"]),
                    Int32.Parse(context.Request.Params["Rtc"]),
                    Int32.Parse(context.Request.Params["Memory"]),
                    Int64.Parse(context.Request.Params["UsedDiskSize"]),
                    Int64.Parse(context.Request.Params["UsedRamSize"])
                    ))
                    {
                        context.Response.StatusCode = 200;
                        context.Response.Write("Success");
                    }
                    else
                    {
                        context.Response.StatusCode = 500;
                        context.Response.StatusDescription = "An error occurred while submitting health test results";
                    }
                    break;
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