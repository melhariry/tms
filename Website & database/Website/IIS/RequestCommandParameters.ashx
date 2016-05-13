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
            DataRow parameters = (new DB()).GetCommandParameters(context.Request.Headers["SerialNumber"]);
            string command = context.Request.Params["Command"];
            string selectedParameter = string.Empty;
            switch (command)
            {
                case "DeleteFile":
                    selectedParameter= parameters["DeleteFileParameters"].ToString();
                    break;

                case "DeleteApp":
                    selectedParameter = parameters["DeleteAppParameters"].ToString();
                    break;

                case "UpdateFile":
                    selectedParameter = parameters["UpdateFileParameters"].ToString();
                    break;

                case "UpdateApp":
                    selectedParameter = parameters["UpdateAppParameters"].ToString();
                    break;

                case "PutFile":
                    selectedParameter = parameters["PutFileParameters"].ToString();
                    break;
            }
            
            context.Response.StatusCode = 200;
            context.Response.Write(selectedParameter);

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