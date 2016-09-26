<%@ WebHandler Language="C#" Class="RequestCommandParameters" %>

using System;
using System.Web;

public class RequestCommandParameters : IHttpHandler {
    
    /*files should be added to the db in the following format:
     * filename1.txt;filename2.file;
     * */
    public void ProcessRequest (HttpContext context) {
        context.Response.ContentType = "text/plain";
        try
        {
            DB db = new DB();
            System.Data.DataRow parameter = db.GetDeleteFileParameters(context.Request.Headers["SerialNumber"]);
            context.Response.Write(parameter[0]);
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