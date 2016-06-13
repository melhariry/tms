﻿using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Linq;
using System.Text;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

public partial class Config : System.Web.UI.Page
{
    List<string> time = new List<string>();
    DataTable allTerminals;
    DataRow commandToSend;
    int filesCount;
    string rawHTMLError = string.Empty, rawHTMLSuccess = string.Empty, dumpPath = string.Empty;
    protected void Page_Load(object sender, EventArgs e)
    {
        allTerminals = DB.Instance.GetTerminals();
        dumpPath = Server.MapPath("~/Dump/");
        filesCount = GetFilesCount();
        if (!IsPostBack)
        {
            if (Session["id"] == null)
                Response.Redirect("~/Login.aspx");
            time.Add("Hours");
            time.Add("Minutes");
            TimeList.DataSource = time;
            TimeList.DataBind();
        }
    }

    protected void PrintStatus()
    {
        if (!rawHTMLError.Equals(string.Empty))
            Response.Write(Methods.ReturnHTMLStatus(rawHTMLError, "error"));
        if (!rawHTMLSuccess.Equals(string.Empty))
            Response.Write(Methods.ReturnHTMLStatus(rawHTMLSuccess, "success"));
        rawHTMLError = string.Empty;
        rawHTMLSuccess = string.Empty;
    }
    protected void GenerateConfigBtn_Click(object sender, EventArgs e)
    {
        string filePath = Server.MapPath("~/Dump/TMSConfig.csv"), param = string.Empty;
        bool commandSuccess = true;
        double interval = Convert.ToDouble(IntervalTxt.Text);
        if (interval < 0)
        {
            rawHTMLError = "Please Enter a valid interval";
            return;
        }
        if (TimeList.SelectedIndex == 0)
            IntervalTxt.Text = (interval * 60).ToString();
        string config = FirstIpTxt.Text + '\n' + SecondIpTxt.Text + '\n' + FTPUserTxt.Text + '\n' + FTPPassTxt.Text + '\n' + IntervalTxt.Text;
        System.IO.File.WriteAllText(filePath, config);
        using (Stream fileStream = File.OpenRead(filePath))
        {
            if (Methods.UploadToFtp("TMSConfig.csv", (int)fileStream.Length, string.Empty, fileStream, true))
                rawHTMLSuccess += "<li>Config file uploaded successfully</li>";
            else
            {
                rawHTMLError += "<li>Config file failed to upload</li>";
                return;
            }
            File.Delete(filePath);
        }
        ClearTextBoxes();
        for (int i = 0; i < allTerminals.Rows.Count; i++)
        {
            commandToSend = DB.Instance.GetCommandToSend(Convert.ToInt32(allTerminals.Rows[i][0]));
            param = commandToSend["PullFileParameters"].ToString();
            param += "TMSConfig.csv;";
            if (!DB.Instance.UpdateCommandToSend(Convert.ToInt32(commandToSend["Id"]), 32, string.Empty, string.Empty, string.Empty, string.Empty, param, false))
                commandSuccess = false;
        }
        if (commandSuccess)
            rawHTMLSuccess += "<li>Commands updated successfully</li>";
        else
            rawHTMLError += "<li>Commands failed to update, please contact database admin</li>";
    }
    private void ClearTextBoxes()
    {
        FirstIpTxt.Text = string.Empty;
        SecondIpTxt.Text = string.Empty;
        FTPUserTxt.Text = string.Empty;
        IntervalTxt.Text = string.Empty;
    }

    protected int GetFilesCount()
    {
        filesCount = Directory.GetFiles(dumpPath, "*", SearchOption.TopDirectoryOnly).Length;
        return filesCount;
    }

    protected void DeleteDumpBtn_Click(object sender, EventArgs e)
    {
        if (filesCount == 0)
        {
            rawHTMLError += "<li>No dump files to delete</li>";
            return;
        }
        try
        {
            System.IO.DirectoryInfo dumpInfo = new DirectoryInfo(dumpPath);
            foreach (FileInfo file in dumpInfo.GetFiles())
            {
                file.Delete();
            }
            rawHTMLSuccess += "<li>Files deleted successfully</li>";
        }
        catch (Exception)
        {
            rawHTMLError += "<li>Failed to delete files</li>";
        }
    }
}