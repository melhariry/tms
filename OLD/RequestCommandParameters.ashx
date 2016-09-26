<%@ WebHandler Language="C#" Class="RequestCommandParameters" %>

using System;
using System.IO;
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
            //DataRow parameters = (new DB()).GetCommandParameters(context.Request.Headers["SerialNumber"]);
            using (StreamWriter w = File.AppendText("E:\\TMS\\Logging\\RequestCommandResultLog.txt"))
            {
                string S = new StreamReader(HttpContext.Current.Request.InputStream).ReadToEnd();
                w.WriteLine("newRequest from:" + context.Request.Headers["SerialNumber"]);
                w.WriteLine(S);
                w.WriteLine("-----");
            }
            string command = context.Request.Params["Command"];
            string selectedParameter = string.Empty;
            switch (command)
            {
                case "DeleteFile":
                    //selectedParameter = "update.mci;update.mmci;FileAccess.CAP;HelloWorld.CAP;";
                    selectedParameter = "pri/testpri1.txt;pub/app.mci;pub/app.mmci;pub/update.mci;pub/update.mmci;";
                    //selectedParameter= parameters["DeleteFileParameters"].ToString();
                    break;
                case "PushFile":
                    selectedParameter = "pri/v2Alpha;";
                    //selectedParameter= parameters["DeleteFileParameters"].ToString();
                    break;
                case "PullFile":
                    selectedParameter = "pub/la2aDir/hanshof.txt;";
                    //selectedParameter = parameters["DeleteAppParameters"].ToString();
                    break;

                case "UpdateFile":
                    
                    //selectedParameter = parameters["UpdateFileParameters"].ToString();
                    break;

                case "UpdateApp":
                    selectedParameter = "HelloWorld.CAP;";
                    //selectedParameter = parameters["UpdateAppParameters"].ToString();
                    break;

                case "PutFile":
                    //selectedParameter = parameters["PutFileParameters"].ToString();
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