﻿<%@ WebHandler Language="C#" Class="PosUp" %>

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
            
            if (db.PosExists(context.Request.Headers["SerialNumber"], Methods.GetUserIP(context), out posId))
            {
                //send commands
                DataRow commandToSend = db.GetCommandToSend(posId);
                //No command to send
                context.Response.StatusCode = 200;
                context.Response.Write("0" + commandToSend["Command"]);
            }
            else
                context.Response.Write("128");
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