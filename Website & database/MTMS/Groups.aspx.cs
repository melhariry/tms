using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Data;
using System.Net;
using System.Collections;

public partial class groups : System.Web.UI.Page
{
    private DataTable allGroups, groupTerminals;
    string rawHTMLError = string.Empty, rawHTMLSuccess = string.Empty;
    protected void Page_Load(object sender, EventArgs e)
    {
        allGroups = DB.Instance.GetPosGroups();
        if (!IsPostBack)
        {
            if (Session["id"] == null)
                Response.Redirect("~/Login.aspx");
            GroupsRep.DataSource = allGroups;
            GroupsRep.DataBind();
        }
    }
    protected void PrintGroups()
    {
        string rawHTML = string.Empty;
        for (int i = 0; i < allGroups.Rows.Count; i++)
        {
            rawHTML += "<tr><td>";
            rawHTML += "<a href='group.aspx?id=" + allGroups.Rows[i][0] + "&";
            rawHTML += "name=" + allGroups.Rows[i][1] + "'>";
            rawHTML += allGroups.Rows[i][0] + "</a></td>";
            for (int j = 1; j < allGroups.Columns.Count; j++)
                rawHTML += "<td>" + allGroups.Rows[i][j] + "</td>";
            rawHTML += "</tr>";
        }
        Response.Write(rawHTML);
    }

    protected void PrintStatus()
    {
        if (!rawHTMLError.Equals(string.Empty))
            Response.Write(Methods.ReturnHTMLStatus(rawHTMLError, "error", false));
        if (!rawHTMLSuccess.Equals(string.Empty))
            Response.Write(Methods.ReturnHTMLStatus(rawHTMLSuccess, "success", false));
        rawHTMLError = string.Empty;
        rawHTMLSuccess = string.Empty;
    }

    private void ClearTextBoxes()
    {
        NewNameTxt.Text = string.Empty;
        NewBranchTxt.Text = string.Empty;
        ContactNumTxt.Text = string.Empty;
        ContactPersonTxt.Text = string.Empty;
    }
    protected void CreateGroupBtn_Click(object sender, EventArgs e)
    {
        string contact = ContactPersonTxt.Text;
        if (!CheckGroupDetails())
        {
            rawHTMLError = NewNameTxt.Text + " already exists with the same branch";
            return;
        }
        if (contact.Equals(string.Empty))
            contact= "N/A";
        int GroupId = -1;
        if (DB.Instance.CreatePosGroup(NewNameTxt.Text, NewBranchTxt.Text, contact, ContactNumTxt.Text, out GroupId))
            rawHTMLSuccess = NewNameTxt.Text + " created successfully";
        else
            rawHTMLError = "Group was not created, please contact database admin";
        GroupsRep.DataSource = DB.Instance.GetPosGroups();
        GroupsRep.DataBind();
        ClearTextBoxes();
    }

    private bool CheckGroupDetails()
    {
        for (int i = 0; i < allGroups.Rows.Count; i++)
            if (NewNameTxt.Text.Equals(allGroups.Rows[i][1]) && NewBranchTxt.Text.Equals(allGroups.Rows[i][2]))
                return false;
        return true;
    }

    protected void DeleteGroupBtn_Click(object sender, EventArgs e)
    {
        int groupId;
        bool selected = false;
        for (int i = 0; i < GroupsRep.Items.Count; i++)
        {
            if ((GroupsRep.Items[i].Controls[1] as CheckBox).Checked)
            {
                groupId = Convert.ToInt32(allGroups.Rows[i][0]);
                if (groupId == 1)
                    rawHTMLError += "<li>Cannot delete the 'unassigned group'</li>";
                else
                {
                    DB.Instance.UpdateGroupTerminalsGroup(groupId, 1);
                    DB.Instance.DeleteGroup(groupId);
                    rawHTMLSuccess += "<li>" + allGroups.Rows[i][1] + " deleted successfully</li>";
                }
                selected = true;
            }
        }
        if (!selected)
        {
            rawHTMLError += "<li>Please select at least one group</li>";
            return;
        }
        for (int i = 0; i < GroupsRep.Items.Count; i++)
            (GroupsRep.Items[i].Controls[1] as CheckBox).Checked = false;
        SelectAllGroups.Checked = false;

        GroupsRep.DataSource = DB.Instance.GetPosGroups();
        GroupsRep.DataBind();
    }

    private void ClearCheckBoxes()
    {
        foreach (Control ctrl in GroupsForm.Controls)
        {
            if (ctrl is CheckBox)
            {
                (ctrl as CheckBox).Checked = false;
            }
        }
        Schedule.Checked = false;
    }
    protected void FreezeGroupBtn_Click(object sender, EventArgs e)
    {
        bool selected = false;
        for (int i = 0; i < GroupsRep.Items.Count; i++)
        {
            if ((GroupsRep.Items[i].Controls[1] as CheckBox).Checked)
            {
                if (Convert.ToInt32(allGroups.Rows[i][6]) == 0)
                    rawHTMLError += "<li>" + allGroups.Rows[i][1] + " already empty</li>";

                else if (DB.Instance.FreezeGroup(Convert.ToInt32(allGroups.Rows[i][0])))
                    rawHTMLSuccess += "<li>" + allGroups.Rows[i][1] + " terminals' frozen successfully</li>";
                else
                    rawHTMLError += "<li>Database error, please contact database admin</li>";
                selected = true;
            }
        }
        if (!selected)
        {
            rawHTMLError += "<li>Please select at least one group</li>";
            return;
        }

        for (int i = 0; i < GroupsRep.Items.Count; i++)
            (GroupsRep.Items[i].Controls[1] as CheckBox).Checked = false;
        SelectAllGroups.Checked = false;

        GroupsRep.DataSource = DB.Instance.GetPosGroups();
        GroupsRep.DataBind();
    }
    protected void GroupClearBtn_Click(object sender, EventArgs e)
    {
        bool commandSubmitted = true, selected = false;
        for (int j = 0; j < GroupsRep.Items.Count; j++)
        {
            if ((GroupsRep.Items[j].Controls[1] as CheckBox).Checked)
            {
                selected = true;
                (GroupsRep.Items[j].Controls[1] as CheckBox).Checked = false;
                groupTerminals = DB.Instance.GetGroupTerminals(Convert.ToInt32(allGroups.Rows[j][0]));

                for (int i = 0; i < groupTerminals.Rows.Count; i++)
                {
                    if (!DB.Instance.ClearCommandToSend(groupTerminals.Rows[i][1].ToString(), false))
                        commandSubmitted = false;
                }
            }

        }
        if (!selected)
        {
            rawHTMLError += "<li>Please select at least one group</li>";
            return;
        }
        if (!commandSubmitted)
            rawHTMLError += "<li>" + "Database error, please contact Database adminstrator</li>";
        else
            rawHTMLSuccess += "<li>" + "Commands cleared sucessfully</li>";
        ClearCheckBoxes();
    }
    protected void GroupSubmitBtn_Click(object sender, EventArgs e)
    {
        BitArray cmd = new BitArray(32, false);
        DataRow oldCommand;
        bool commandSubmitted = true, selected = false;
        int ongoingCommand = 0;
        string updateAppParams, pullFileParams, vendor, pushFileParams;
        string uploadFiles = string.Empty, uploadCabs = string.Empty, uploadApks = string.Empty;
        string apkPath = "Apps/VeriFone/", cabPath = "Apps/Castles/";
        foreach (Control ctrl in GroupsForm.Controls)
        {
            if (ctrl is CheckBox)
            {
                if (!ctrl.ID.Equals("SelectAllGroups"))
                    cmd.Set((int)Enum.Parse(typeof(AppParameters.Commands), ctrl.ID), (ctrl as CheckBox).Checked);
            }
        }
        int[] newCommand = new int[1];
        cmd.CopyTo(newCommand, 0);

        if (newCommand[0] == 0)
        {
            rawHTMLError += "<li>Please select at least one command</li>";
            return;
        }

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
            for (int j = 0; j < GroupsRep.Items.Count; j++)
            {
                if ((GroupsRep.Items[j].Controls[1] as CheckBox).Checked)
                {
                    selected = true;
                    (GroupsRep.Items[j].Controls[1] as CheckBox).Checked = false;
                    groupTerminals = DB.Instance.GetGroupTerminals(Convert.ToInt32(allGroups.Rows[j][0]));
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
                }
            }
            if (!selected)
            {
                rawHTMLError += "<li>Please select at least one group</li>";
                return;
            }
            if (!commandSubmitted)
                rawHTMLError += "<li>" + "Database error, please contact Database adminstrator</li>";
            else
                rawHTMLSuccess += "<li>" + "Commands submitted successfully</li>";
            SelectAllGroups.Checked = false;
            ClearCheckBoxes();
        }
        catch (WebException ex)
        {
            rawHTMLError += "<li>" + ex.Message + "</li>";
        }
    }
}