<%@ WebHandler Language="C#" Class="SubmitCommandResult" %>

using System;
using System.Web;

public class SubmitCommandResult : IHttpHandler {
    
    public void ProcessRequest (HttpContext context) {
        string status = string.Empty, parameters = string.Empty;
        context.Response.ContentType = "text/plain";
        string[] commandParams;
        string[] commandStatus;
        try
        {
            string command = context.Request.Params["Command"];
            
            switch (command)
            { 
                //Pos Terminal done executing all commands, flush command to send
                case "Finish":
                    if (!DB.Instance.ClearCommandToSend(context.Request.Headers["SerialNumber"]))
                    {
                        context.Response.StatusCode = 500;
                        context.Response.StatusDescription = "An error occurred while submitting the terminating command";
                        return;
                    }
                break;
                    
                case "TestHealth":
                    if (!DB.Instance.SubmitHealthTestResults(
                        context.Request.Headers["SerialNumber"],
                        Int32.Parse(context.Request.Params["Crypto"]),
                        Int32.Parse(context.Request.Params["Printer"]),
                        Int32.Parse(context.Request.Params["Timer"]),
                        Int32.Parse(context.Request.Params["Buzzer"]),
                        Int32.Parse(context.Request.Params["Led"]),
                        Int32.Parse(context.Request.Params["Rtc"]),
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
                    commandParams = context.Request.Params["Name"].Split(',');
                    commandStatus = context.Request.Params["Status"].Split(',');
                    for (int i = 0; i < commandParams.Length; i++)
                    {
                        if (Convert.ToInt32(commandStatus[i]) == 0)
                        {
                            DB.Instance.DeletePosFileFromList(context.Request.Headers["SerialNumber"], commandParams[i]);
                        }
                        parameters += commandParams[i] + ';';
                        status += commandStatus[i] + ';';
                    }
                break;
                    
                case "ListFiles":
                    commandParams = context.Request.Params["Name"].Split(',');
                    string[] parentFolders = context.Request.Params["Parent"].Split(',');
                    string[] fileSizes = context.Request.Params["Size"].Split(',');
                    for (int i = 0; i < commandParams.Length; i++)
                    {
                        if (!DB.Instance.SubmitPosFileList(context.Request.Headers["SerialNumber"], commandParams[i], Convert.ToInt64(fileSizes[i]), parentFolders[i]))
                        {
                            status += "500;";
                            parameters += commandParams[i] + ';';
                        }
                    } 
                break;
                    
                case "ListApps":
                    commandParams = context.Request.Params["Name"].Split(',');
                    string[] appVersions = context.Request.Params["Version"].Split(',');
                    string[] appComs = context.Request.Params["Com"].Split(',');
                    if (!DB.Instance.DeletePosAppList(context.Request.Headers["SerialNumber"]))
                    {
                        context.Response.StatusCode = 500;
                        context.Response.StatusDescription = "An error occurred while deleting the app list";
                        return;
                    }
                    for (int i = 0; i < commandParams.Length; i++)
                    {
                        if (!DB.Instance.SubmitPosAppList(context.Request.Headers["SerialNumber"], commandParams[i], appVersions[i], appComs[i]))
                        {
                            status += "500;";
                            parameters += commandParams[i] + ';';
                        }
                             
                    }
                break;
                    
                case "UpdateApp":
                    commandParams = context.Request.Params["Name"].Split(',');
                    commandStatus = context.Request.Params["Status"].Split(',');
                    for (int i = 0; i < commandParams.Length; i++)
                    {
                        parameters += commandParams[i] + ';';
                        status += commandStatus[i] + ';';
                    }               
                break;
                    
                //Push file and pull file cases
                default:
                    commandParams = context.Request.Params["Path"].Split(',');
                    commandStatus = context.Request.Params["Status"].Split(',');
                    for (int i = 0; i < commandParams.Length; i++)
                    {
                        parameters += commandParams[i] + ';';
                        status += commandStatus[i] + ';';
                    }
                break;
                    
            }

            if (status.Equals(string.Empty))
                status = "0";
            //Log Result for all commands except testhealth and finish
            if (DB.Instance.SubmitCommandResult(
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
            context.Response.Write(EX.Message);
        }
    }
 
    public bool IsReusable {
        get {
            return false;
        }
    }

}