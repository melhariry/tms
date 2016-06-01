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
using System.Net;

public partial class Group : System.Web.UI.Page
{
    DataTable groupTerminals, outerTerminals, lastHealthTest;
    DataRow groupInfo;
    string rawHTMLError = string.Empty, rawHTMLSuccess = string.Empty;
    protected void Page_Load(object sender, EventArgs e)
    {
        groupTerminals = DB.Instance.GetGroupTerminals(Convert.ToInt32(Context.Request.Params["id"]));
        outerTerminals = DB.Instance.GetOuterTerminals(Convert.ToInt32(Context.Request.Params["id"]));
        groupInfo = DB.Instance.GetGroupInfo(Convert.ToInt32(Context.Request.Params["id"]));
        if (!IsPostBack)
        {
            if (Session["id"] == null)
                Response.Redirect("~/Login.aspx");
            OutersRep.DataSource = outerTerminals;
            OutersRep.DataBind();
            NewNameTxt.Text = groupInfo["Name"].ToString();
            NewBranchTxt.Text = groupInfo["Branch"].ToString();
            ContactNumTxt.Text = groupInfo["ContactNumber"].ToString();
            ContactPersonTxt.Text = groupInfo["ContactPerson"].ToString();
        }
    }
    protected void PrintGroupTerminals()
    {
        string rawHTML = string.Empty;
        double totalDays;
        for (int i = 0; i < groupTerminals.Rows.Count; i++)
        {
            lastHealthTest = DB.Instance.GetPosLastHealthTest(Convert.ToInt32(groupTerminals.Rows[i][0]));
            rawHTML += "<tr> <td class=\"sorting_1\">";
            rawHTML += "<a href='terminal.aspx?id=" + groupTerminals.Rows[i][0] + "'>";
            rawHTML += groupTerminals.Rows[i][0] + "</a></td>";
            for (int j = 1; j < groupTerminals.Columns.Count; j++)
            {
                if (j == 4)
                {
                    totalDays = DateTime.Now.Subtract((DateTime)groupTerminals.Rows[i][j]).TotalDays;
                    if (totalDays > 7)
                        rawHTML += "<td style=\"background-color:#F51616; color:white;\">" + groupTerminals.Rows[i][j] + "</td>";
                    else
                        rawHTML += "<td style=\"background-color:#10EB14; color:white;\">" + groupTerminals.Rows[i][j] + "</td>";
                }
                else
                    rawHTML += "<td>" + groupTerminals.Rows[i][j] + "</td>";
            }
            if (GetHealthStatus())
                rawHTML += "<td style=\"background-color:#10EB14; color:white;\">Running</td>";
            else
                rawHTML += "<td style=\"background-color:#F51616; color:white;\">Error</td>";

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

    private bool GetHealthStatus()
    {
        if (lastHealthTest.Rows.Count != 0)
            for (int i = 1; i < lastHealthTest.Columns.Count; i++)
                if (Convert.ToInt32(lastHealthTest.Rows[0][i]) != 0)
                    return false;
        return true;
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
        Schedule.Checked = false;
    }
    
    protected void GroupSubmitBtn_Click(object sender, EventArgs e)
    {
        BitArray cmd = new BitArray(32, false);
        DataRow oldCommand;
        bool commandSubmitted = true;
        int ongoingCommand = 0;
        string updateAppParams, pullFileParams, vendor, pushFileParams;
        string uploadFiles = string.Empty, uploadCabs = string.Empty, uploadApks = string.Empty;
        string apkPath = "Apps/VeriFone/", cabPath = "Apps/Castles/";
        foreach (Control ctrl in GroupForm.Controls)
        {
            if (ctrl is CheckBox)
            {
                if (!ctrl.ID.Equals("SelectAllOuters"))
                    cmd.Set((int)Enum.Parse(typeof(AppParameters.Commands), ctrl.ID), (ctrl as CheckBox).Checked);
            }
        }
        int[] newCommand = new int[1];
        cmd.CopyTo(newCommand, 0);
        if (UPDATE_APP.Checked)
        {
            if (update_app_cab.HasFiles)
            {
                foreach (HttpPostedFile uploadedCab in update_app_cab.PostedFiles)
                {
                    if (!Methods.UploadToFtp(uploadedCab.FileName, uploadedCab.ContentLength, cabPath, uploadedCab.InputStream))
                        rawHTMLError += "<li>" + uploadedCab.FileName + " cannot be uploaded</li>";
                    else
                    {
                        uploadCabs += uploadedCab.FileName + ";";
                        rawHTMLSuccess += "<li>" + uploadedCab.FileName + " uploaded successfully</li>";
                    }
                }
            }
            else
                rawHTMLError += "<li>No or empty .cab files</li>";
            if (update_app_apk.HasFiles)
            {
                foreach (HttpPostedFile uploadedApk in update_app_apk.PostedFiles)
                {
                    if (!Methods.UploadToFtp(uploadedApk.FileName, uploadedApk.ContentLength, apkPath, uploadedApk.InputStream))
                        rawHTMLError += "<li>" + uploadedApk.FileName + " cannot be uploaded</li>";
                    else
                    {
                        uploadApks += uploadedApk.FileName + ";";
                        rawHTMLSuccess += "<li>" + uploadedApk.FileName + " uploaded successfully</li>";
                    }
                }
            }
            else
                rawHTMLError += "<li>No or empty .apk files</li>";
        }
        if (PULL_FILE.Checked)
        {
            if (update_hotlist.HasFile)
                if (!Methods.UploadToFtp(update_hotlist.FileName, update_hotlist.PostedFile.ContentLength, string.Empty, update_hotlist.PostedFile.InputStream))
                    rawHTMLError += "<li>" + update_hotlist.FileName + " cannot be uploaded</li>";
                else
                {
                    uploadFiles += update_hotlist.FileName + ";";
                    rawHTMLSuccess += "<li>" + update_hotlist.FileName + " uploaded successfully</li>";
                }
            else
                rawHTMLError += "<li>No or empty hotlist file</li>";
            if (update_config.HasFile)
                if (!Methods.UploadToFtp(update_config.FileName, update_config.PostedFile.ContentLength, string.Empty, update_config.PostedFile.InputStream))
                    rawHTMLError += "<li>" + update_config.FileName + " cannot be uploaded</li>";
                else
                {
                    uploadFiles += update_config.FileName + ";";
                    rawHTMLSuccess += "<li>" + update_config.FileName + " uploaded successfully</li>";
                }
            else
                rawHTMLError += "<li>No or empty config file</li>";
        }
        try
        {
            for (int i = 0; i < groupTerminals.Rows.Count; i++)
            {
                oldCommand = DB.Instance.GetCommandToSend(Convert.ToInt32(groupTerminals.Rows[i][0]));
                if (Convert.ToInt32(oldCommand["Command"]) != 0)
                    ongoingCommand = newCommand[0] | Convert.ToInt32(oldCommand["Command"]);
                else
                    ongoingCommand = newCommand[0];
                updateAppParams = oldCommand["UpdateAppParameters"].ToString();
                pullFileParams = oldCommand["PullFileParameters"].ToString();
                pushFileParams = oldCommand["PushFileParameters"].ToString();
                vendor = groupTerminals.Rows[i][2].ToString();

                if (PUSH_FILE.Checked)
                    pushFileParams += "pub/transaction.txt;";
                if (PULL_FILE.Checked)
                    pullFileParams += uploadFiles;

                if (UPDATE_APP.Checked)
                {
                    if (vendor.Equals("Castles"))
                        updateAppParams += uploadCabs;
                    else if (vendor.Equals("VeriFone"))
                        updateAppParams += uploadApks;
                }
                if (!DB.Instance.UpdateCommandToSend(Convert.ToInt32(oldCommand["Id"]), ongoingCommand, string.Empty, updateAppParams, string.Empty, pushFileParams, pullFileParams, Schedule.Checked))
                    commandSubmitted = false;
            }

            if (!commandSubmitted)
                rawHTMLError += "<li>" + "Database error, please contact Database adminstrator</li>";
            else
                rawHTMLSuccess += "<li>" + "Commands submitted successfully</li>";
            ClearCheckBoxes();
        }
        catch (WebException ex)
        {
            rawHTMLError += "<li>" + ex.Message + "</li>";
        }
    }

    protected void GroupClearBtn_Click(object sender, EventArgs e)
    {
        bool commandSubmitted = true;
        for (int i = 0; i < groupTerminals.Rows.Count; i++)
        {
            if (!DB.Instance.ClearCommandToSend(groupTerminals.Rows[i][1].ToString(), false))
                commandSubmitted = false;
        }
        if (!commandSubmitted)
            rawHTMLError += "<li>" + "Database error, please contact Database adminstrator</li>";
        else
            rawHTMLSuccess += "<li>" + "Commands cleared sucessfully</li>";
        ClearCheckBoxes();
    }
    protected void MoveOuterToGroup_Click(object sender, EventArgs e)
    {
        for (int i = 0; i < OutersRep.Items.Count; i++)
        {
            if ((OutersRep.Items[i].Controls[1] as CheckBox).Checked)
            {
                if (DB.Instance.UpdateTerminalGroup(Convert.ToInt32(outerTerminals.Rows[i][0]), Convert.ToInt32(Context.Request.Params["id"])))
                    rawHTMLSuccess += "<li>" + outerTerminals.Rows[i][0] + " added successfully</li>";
                else
                    rawHTMLError += "<li>Database error, please contact database admin</li>";
                (OutersRep.Items[i].Controls[1] as CheckBox).Checked = false;
            }
        }
        SelectAllOuters.Checked = false;
        groupTerminals = DB.Instance.GetGroupTerminals(Convert.ToInt32(Context.Request.Params["id"]));
        OutersRep.DataSource = DB.Instance.GetOuterTerminals(Convert.ToInt32(Context.Request.Params["id"]));
        OutersRep.DataBind();
    }
    protected void EditGroupBtn_Click(object sender, EventArgs e)
    {
        string contact = ContactPersonTxt.Text;
        int status = DB.Instance.UpdateGroup(Convert.ToInt32(Context.Request.Params["id"]), NewNameTxt.Text, NewBranchTxt.Text, contact, ContactNumTxt.Text);
        if (contact.Equals(string.Empty))
            contact = "N/A";
        if (status == -1)
            rawHTMLError += NewNameTxt.Text + " already exists with the same branch";
        else if (status == 0)
            rawHTMLError = "Group was not created, please contact database admin";
        else
            rawHTMLSuccess = NewNameTxt.Text + " edited successfully";
    }
    protected void FreezeGroupBtn_Click(object sender, EventArgs e)
    {
        if (groupTerminals.Rows.Count == 0)
        {
            rawHTMLError += "<li>Group is empty</li>";
            return;
        }

        if (!DB.Instance.FreezeGroup(Convert.ToInt32(Context.Request.Params["id"])))
            rawHTMLError += "<li>Database error, please contact database admin</li>";
        else
            rawHTMLSuccess += "<li>Terminals frozen successfully</li>";
        groupTerminals = DB.Instance.GetGroupTerminals(Convert.ToInt32(Context.Request.Params["id"]));
    }
}