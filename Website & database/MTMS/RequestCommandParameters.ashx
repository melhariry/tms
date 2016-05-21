<%@ WebHandler Language="C#" Class="RequestCommandParameters" %>

using System;
using System.Web;
using System.Data;

public class RequestCommandParameters : IHttpHandler {
    
    /*files should be added to the db in the following format:
     * filename1.txt;filename2.file;
     * */
    public void ProcessRequest (HttpContext context) {
        context.Response.ContentType = "text/plain";
        try
        {
            DataRow parameters = DB.Instance.GetCommandParameters(context.Request.Headers["SerialNumber"]);
            context.Response.StatusCode = 200;
            context.Response.Write(parameters[context.Request.Params["Command"] + "Parameters"].ToString());

        }
        catch (Exception ex)
        {
            context.Response.StatusCode = 500;
            context.Response.StatusDescription = ex.Message; 
        }
    }
 
    public bool IsReusable {
        get {
            return false;
        }
    }

}