<%@ WebHandler Language="C#" Class="PosUp" %>

using System;
using System.Web;
using System.Data;
using System.Data.SqlClient;
using System.Configuration;

public class PosUp : IHttpHandler {

    public void ProcessRequest (HttpContext context) {
        context.Response.ContentType = "text/plain";
        try
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
        }
    }
 
    public bool IsReusable {
        get {
            return false;
        }
    }

}