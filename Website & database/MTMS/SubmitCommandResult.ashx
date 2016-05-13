<%@ WebHandler Language="C#" Class="SubmitCommandResult" %>

using System;
using System.Web;

public class SubmitCommandResult : IHttpHandler {
    
    public void ProcessRequest (HttpContext context) {
        string status = string.Empty, parameters = string.Empty;
        context.Response.ContentType = "text/plain";
        try
        {
            DB db = new DB();
            string command = context.Request.Params["Command"];
            //TODO: Handle DeleteFiles, ListFiles, ListApps
           
            switch (command)
            { 
                //Pos Terminal done executing all commands, flush command to send
                case "Finish":
                    if (!db.ClearCommandToSend(context.Request.Headers["SerialNumber"]))
                    {
                        context.Response.StatusCode = 500;
                        context.Response.StatusDescription = "An error occurred while submitting the terminating command";
                        return;
                    }
                break;
                    
                case "TestHealth":
                    if (!db.SubmitHealthTestResults(
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
                        context.Response.StatusCode = 500;
                        context.Response.StatusDescription = "An error occurred while submitting the test health result";
                        return;
                    }
                break;
                    
                case "DeleteFile":
                    string[] fileNames = context.Request.Params["Name"].Split(',');
                    string[] fileStatus = context.Request.Params["Status"].Split(',');
                    for (int i = 0; i < fileNames.Length; i++)
                    {
                        if (Convert.ToInt32(fileStatus[i]) == 0)
                        {
                            db.DeletePosFileFromList(context.Request.Headers["SerialNumber"], fileNames[0]);
                        }
                        else
                        {
                            parameters += fileNames[i] + ';';
                            status += fileStatus[i] + ';';
                        }
                    }
                    if (status.Equals(string.Empty))
                        status = "200";
                break;
                    
                case "ListFiles":
                break;
                    
                case "ListApps":
                    string[] AppNames = context.Request.Params["Name"].Split(',');
                    string[] AppVersions = context.Request.Params["Version"].Split(',');
                    string[] AppComs = context.Request.Params["Com"].Split(',');
                    for (int i = 0; i < AppNames.Length; i++)
                    {
                        if (!db.SubmitPosAppList(context.Request.Headers["SerialNumber"], AppNames[i], Convert.ToInt32(AppVersions[i]), AppComs[i]))
                        {
                            status += "500;";
                            parameters += AppNames[i] + ';';
                        }
                             
                    }
                    if (status.Equals(string.Empty))
                        status = "200;";
                break;
                    
            }


            //Log Result for all commands except testhealth and finish
            if (db.SubmitCommandResult(
                context.Request.Headers["SerialNumber"],
                command,
                status,
                DateTime.Now,
                parameters
                ))
            {
                context.Response.StatusCode = 200;
                context.Response.Write("Success");
            }
            else
            {
                context.Response.StatusCode = 500;
                context.Response.StatusDescription = "An error occurred while logging the command result";
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