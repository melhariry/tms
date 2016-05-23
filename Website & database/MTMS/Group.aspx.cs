using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Data;
using System.Collections;
using System.Web.UI.HtmlControls;
using System.IO;

public partial class Group : System.Web.UI.Page
{
    DataTable groupTerminals;
    string rawHTMLError = string.Empty, rawHTMLSuccess = string.Empty;
    protected void Page_Load(object sender, EventArgs e)
    {  
        groupTerminals = DB.Instance.GetGroupTerminals(Convert.ToInt32(Context.Request.Params["id"]));
    }
    protected void PrintGroupTerminals()
    {
        string rawHTML = string.Empty;
        for (int i = 0; i < groupTerminals.Rows.Count; i++)
        {
            rawHTML += "<tr> <td class=\"sorting_1\">";
            rawHTML += "<a href='terminal.aspx?id=" + groupTerminals.Rows[i][0] + "'>";
            rawHTML += groupTerminals.Rows[i][0] + "</a></td>";
            for (int j = 1; j < 7; j++)
                rawHTML += "<td>" + groupTerminals.Rows[i][j] + "</td>";
            rawHTML += "</tr>";
        }
        Response.Write(rawHTML);
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

    private void ClearCheckBoxes()
    {
        foreach (Control ctrl in GroupForm.Controls)
        {
            if (ctrl is CheckBox)
            {
                (ctrl as CheckBox).Checked = false;
            }
        }
    }
    
    protected void SubmitBtn_Click(object sender, EventArgs e)
    {
        //TODO:: Handle success and errors for ftpupload and updateCommandToSend
        BitArray cmd = new BitArray(32, false);
        DataRow oldCommand;
        bool commandSubmitted = true;
        string updateAppParams, pullFileParams, vendor, pushFileParams;
        string apkPath = "Apps/VeriFone/", cabPath = "Apps/Castles/";
        foreach (Control ctrl in GroupForm.Controls)
        {
            if (ctrl is CheckBox)
            {
                cmd.Set((int)Enum.Parse(typeof(AppParameters.Commands), ctrl.ID), (ctrl as CheckBox).Checked);
            }
        }
        int[] newCommand = new int[1];
        cmd.CopyTo(newCommand, 0);
        if (UPDATE_APP.Checked)
        {
            if (update_app_cab.HasFile)
                if (!Methods.UploadToFtp(Path.GetFileName(update_app_cab.FileName), update_app_cab.PostedFile.ContentLength, cabPath, update_app_cab.PostedFile.InputStream))
                    rawHTMLError += "<li>" + Path.GetFileName(update_app_cab.FileName) + " cannot be uploaded</li>";
                else
                    rawHTMLSuccess += "<li>" + Path.GetFileName(update_app_cab.FileName) + " uploaded successfully</li>";

            if (update_app_apk.HasFile)
                if (!Methods.UploadToFtp(Path.GetFileName(update_app_apk.FileName), update_app_apk.PostedFile.ContentLength, apkPath, update_app_apk.PostedFile.InputStream))
                    rawHTMLError += "<li>" + Path.GetFileName(update_app_apk.FileName) + " cannot be uploaded</li>";
                else
                    rawHTMLSuccess += "<li>" + Path.GetFileName(update_app_apk.FileName) + " uploaded successfully</li>";
        }
        if (PULL_FILE.Checked)
        {
            if (update_hotlist.HasFile)
                if (!Methods.UploadToFtp(Path.GetFileName(update_hotlist.FileName), update_hotlist.PostedFile.ContentLength, string.Empty, update_hotlist.PostedFile.InputStream))
                    rawHTMLError += "<li>" + Path.GetFileName(update_hotlist.FileName) + " cannot be uploaded</li>";
                else
                    rawHTMLSuccess += "<li>" + Path.GetFileName(update_hotlist.FileName) + " uploaded successfully</li>";   
        }
        for (int i = 0; i < groupTerminals.Rows.Count; i++)
        {
            oldCommand = DB.Instance.GetCommandToSend(Convert.ToInt32(groupTerminals.Rows[i][0]));
            if (Convert.ToInt32(oldCommand["Command"]) != 0)
                newCommand[0] = newCommand[0] | Convert.ToInt32(oldCommand["Command"]);
            updateAppParams = oldCommand["UpdateAppParameters"].ToString();
            pullFileParams = oldCommand["PullFileParameters"].ToString();
            pushFileParams = oldCommand["PushFileParameters"].ToString();
            vendor = groupTerminals.Rows[i][2].ToString();

            if (PUSH_FILE.Checked)
                pushFileParams += "pub/transaction.txt;";
            if (PULL_FILE.Checked)
                pullFileParams += update_hotlist.FileName +";";

            if (UPDATE_APP.Checked)
            {
                if (vendor.Equals("Castles"))
                    updateAppParams += update_app_cab.FileName + ";";
                else if (vendor.Equals("VeriFone"))
                    updateAppParams += update_app_apk.FileName + ";";
            }
            if (!DB.Instance.UpdateCommandToSend(Convert.ToInt32(oldCommand["Id"]), newCommand[0], string.Empty, updateAppParams, string.Empty, pushFileParams, pullFileParams))
                commandSubmitted = false;
        }

        if (!commandSubmitted)
            rawHTMLError += "<li>" + "Database error, please contact Database adminstrator</li>";
        else
            rawHTMLSuccess += "<li>" + "Commands submitted successfully</li>";
        ClearCheckBoxes();
    }

}