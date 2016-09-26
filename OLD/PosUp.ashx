<%@ WebHandler Language="C#" Class="PosUp" %>

using System;
using System.IO;
using System.Web;
using System.Data;
using System.Data.SqlClient;
using System.Configuration;

public class PosUp : IHttpHandler {

    public void ProcessRequest (HttpContext context) {
        
        context.Response.ContentType = "text/plain";
        //System.Threading.Thread.Sleep(5);
        context.Response.Write("034");

        using (StreamWriter w = File.AppendText("E:\\TMS\\Logging\\PosUpLog.txt"))
        {
            
           
            
            string S=new StreamReader(HttpContext.Current.Request.InputStream).ReadToEnd();
            w.WriteLine("newRequest from:" + context.Request.Headers["SerialNumber"]);
            w.WriteLine(S);
            
          
            w.WriteLine("-----");
        }
       /* try
        {
            DB db = new DB();
            int posId = -1;
            context.Response.StatusCode = 200;
            if (db.PosExists(context.Request.Headers["SerialNumber"], Methods.GetUserIP(context), out posId))
            {
                //send commands
                DataRow commandToSend = db.GetCommandToSend(posId);
                context.Response.Write(commandToSend["Command"]);
            }
            else
                context.Response.Write("CreatePosRecord; ");
        }
        catch (Exception EX)
        {
            context.Response.StatusCode = 500;
            context.Response.StatusDescription = EX.Message;
        }*/
    }
 
    public bool IsReusable {
        get {
            return false;
        }
    }

}