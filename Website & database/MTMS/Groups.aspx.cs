﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Data;

public partial class groups : System.Web.UI.Page
{
    private DataTable allGroups;
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
                    rawHTMLSuccess += "<li>"+ allGroups.Rows[i][1] +" deleted successfully</li>";
                }
            }
        }
        for (int i = 0; i < GroupsRep.Items.Count; i++)
        {
            (GroupsRep.Items[i].Controls[1] as CheckBox).Checked = false;
        }
        GroupsRep.DataSource = DB.Instance.GetPosGroups();
        GroupsRep.DataBind();
    }
}